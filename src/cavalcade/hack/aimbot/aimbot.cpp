#include "aimbot.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"
#include "../../gui/cfg/cfg.hpp"

const static std::map< hack::aimbot::e_hitboxes, std::vector< sdk::e_hitbox > > g_aim_hitboxes = {
	{ hack::aimbot::e_hitboxes::HEAD, { sdk::e_hitbox::HEAD } },

	{ hack::aimbot::e_hitboxes::NECK, { sdk::e_hitbox::NECK } },

	{ hack::aimbot::e_hitboxes::BODY, { sdk::e_hitbox::BELLY, sdk::e_hitbox::THORAX, sdk::e_hitbox::LOWER_CHEST, sdk::e_hitbox::UPPER_CHEST } },

	{ hack::aimbot::e_hitboxes::ARMS,
	  { sdk::e_hitbox::LEFT_HAND, sdk::e_hitbox::RIGHT_HAND, sdk::e_hitbox::RIGHT_UPPER_ARM, sdk::e_hitbox::RIGHT_FOREARM,
	    sdk::e_hitbox::LEFT_UPPER_ARM, sdk::e_hitbox::LEFT_FOREARM } },

	{ hack::aimbot::e_hitboxes::LEGS,
	  { sdk::e_hitbox::LEFT_THIGH, sdk::e_hitbox::RIGHT_THIGH, sdk::e_hitbox::LEFT_CALF, sdk::e_hitbox::RIGHT_CALF, sdk::e_hitbox::LEFT_FOOT,
	    sdk::e_hitbox::RIGHT_FOOT } }
};

math::ang hack::aimbot::pixels_to_angle( float x, float y ) {
	float px = x * g_ctx.m_cvars.m_pitch->get_float( );
	float py = y * g_ctx.m_cvars.m_yaw->get_float( );

	return math::ang( -x, -y, 0.F ).clamp_angle( );
}

math::ang hack::aimbot::angle_to_pixels( const math::ang& angle ) {
	float x = angle.pitch / g_ctx.m_cvars.m_pitch->get_float( );
	float y = angle.yaw / g_ctx.m_cvars.m_yaw->get_float( );

	return math::ang( -y, x, 0.F );
}

math::v3f angle_vectors( const math::ang& angles ) {
	constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };

	f32 sp{ }, sy{ }, cp{ }, cy{ };

	sy = sin( DEG2RAD( angles.yaw ) );
	cy = cos( DEG2RAD( angles.yaw ) );

	sp = sin( DEG2RAD( angles.pitch ) );
	cp = cos( DEG2RAD( angles.pitch ) );

	return { cp * cy, cp * sy, -sp };
}

static auto get_fov( const math::ang& view, const math::ang& aim ) {
	constexpr auto RAD2DEG = []( const f32 x ) -> f32 { return x * ( 180.f / M_PI ); };

	math::v3f _aim = angle_vectors( view );
	math::v3f _ang = angle_vectors( aim );

	return RAD2DEG( acos( _aim.dot_product( _ang ) / _aim.length_sqr( ) ) );
}

struct fire_bullet_data {
	fire_bullet_data( const math::v3f& eye ) : m_src( eye ) { }

	math::v3f m_src;
	sdk::trace m_enter_trace;
	math::v3f m_direction;
	sdk::trace_filter m_filter;
	f32 m_trace_length;
	f32 m_trace_length_remaining;
	f32 m_current_damage;
	i32 m_penetrate_count;
};

constexpr f32 get_damage_multiplier( i32 hitgroup ) {
	switch ( hitgroup ) {
	case sdk::hit_group::HEAD:
		return 4.0f;
	case sdk::hit_group::STOMACH:
		return 1.25f;
	case sdk::hit_group::LEFT_LEG:
	case sdk::hit_group::RIGHT_LEG:
		return 0.75f;
	default:
		return 1.0f;
	}
}

static void scale_damage( i32 hitgroup, sdk::cs_player* e, f32 weapon_armor_ratio, f32& dmg ) {
	dmg *= get_damage_multiplier( hitgroup );

	if ( e->armor_value( ) > 0 ) {
		if ( hitgroup == sdk::hit_group::HEAD ) {
			if ( e->has_helmet( ) )
				dmg *= ( weapon_armor_ratio * .5F );
		} else {
			dmg *= ( weapon_armor_ratio * .5F );
		}
	}
}

static void UTIL_clip_trace_to_players( const math::v3f& abs_start, const math::v3f& abs_end, unsigned int mask, sdk::trace_filter& filter,
                                        sdk::trace& enter_trace, f32 min_range = -60.F ) {
	sdk::trace trace;
	auto smallest_fraction = trace.m_fraction;

	auto ray = sdk::ray( abs_start, abs_end );
	g_entity_cacher.for_each( [ & ]( auto& p ) {
		if ( !p || !p.get( ).is_alive( ) || p.get( ).is_dormant( ) )
			return;

		if ( filter.m_skip == p )
			return;

		auto collideable = p.get( ).get_collideable( );
		if ( collideable )
			return;

		const auto min = collideable->obb_mins( );
		const auto max = collideable->obb_maxs( );

		const auto center = ( max + min ) * .5F;
		const auto pos    = center + p.get( ).get_origin( );

		const auto to  = pos - abs_start;
		auto dir       = abs_end - abs_start;
		const auto len = dir.length( );
		dir.normalize( );

		const auto range_along = dir.dot_product( to );
		auto range             = .0F;

		if ( range_along < .0F )
			range = -to.length( );
		else if ( range_along > len )
			range = -( pos - abs_end ).length( );
		else
			range = ( pos - ( dir * range_along + abs_start ) ).length( );

		if ( range < min_range || range > 60.F )
			return;

		g_csgo.m_engine_trace->clip_ray_to_entity( ray, mask | 0x40000000, p, trace );

		if ( trace.m_fraction < smallest_fraction ) {
			enter_trace       = trace;
			smallest_fraction = trace.m_fraction;
		}
	} );
}

static void trace_line( const math::v3f& abs_start, const math::v3f& abs_end, unsigned int mask, sdk::base_entity* filter_ent, sdk::trace& trace ) {
	auto ray      = sdk::ray( abs_start, abs_end );
	auto filter   = sdk::trace_filter{ };
	filter.m_skip = filter_ent;

	g_csgo.m_engine_trace->trace_ray( ray, mask, filter, trace );
}

static bool is_breakable_entity( sdk::base_entity* e ) {
	static auto og = g_mem[ CLIENT_DLL ].get_address< bool( __thiscall* )( sdk::base_entity* ) >( HASH_CT( "IsBreakableEntity" ) );
	return og( e );
}

static bool trace_to_exit( sdk::base_entity* target, const sdk::trace& enter_trace, const math::v3f& start, const math::v3f& direction,
                           math::v3f& end, sdk::trace& exit_trace ) {
	auto distance       = 0.F;
	auto first_contents = 0;

	while ( distance <= 90.F ) {
		distance += 4.F;

		end = start + ( direction * distance );

		if ( !first_contents )
			first_contents = g_csgo.m_engine_trace->get_point_concents( end, 0x600400b | 0x40000000 );

		auto point_contents = g_csgo.m_engine_trace->get_point_concents( end, 0x600400b | 0x40000000 );

		if ( !( point_contents & 0x600400b ) || ( point_contents & 0x40000000 && point_contents != first_contents ) ) {
			auto new_end = end - ( direction * 4.F );

			trace_line( end, new_end, 0x4600400b, nullptr, exit_trace );

			sdk::trace_filter filter{ target };

			UTIL_clip_trace_to_players( end, new_end, 0x4600400b, filter, exit_trace );

			if ( exit_trace.m_start_solid && exit_trace.m_surface.m_flags & 0x8000 ) {
				trace_line( end, start, 0x600400b, exit_trace.m_entity, exit_trace );

				if ( ( exit_trace.m_fraction < 1.F || exit_trace.m_all_solid || exit_trace.m_start_solid ) && !exit_trace.m_start_solid ) {
					end = exit_trace.m_end;
					return true;
				}

				continue;
			}

			if ( ( exit_trace.m_fraction < 1.F || exit_trace.m_all_solid || exit_trace.m_start_solid ) && !exit_trace.m_start_solid ) {
				if ( enter_trace.m_entity && is_breakable_entity( enter_trace.m_entity ) && exit_trace.m_entity &&
				     is_breakable_entity( exit_trace.m_entity ) )
					return true;

				if ( enter_trace.m_surface.m_flags & 0x80 ||
				     ( !( enter_trace.m_surface.m_flags & 0x80 ) && exit_trace.m_plane.m_normal.dot_product( direction ) <= 1.F ) ) {
					end -= direction * ( exit_trace.m_fraction * 4.F );
					return true;
				}

				continue;
			}

			if ( !( exit_trace.m_fraction < 1.F || exit_trace.m_all_solid || exit_trace.m_start_solid ) || exit_trace.m_start_solid ) {
				if ( enter_trace.m_entity && enter_trace.m_entity->get_networkable_index( ) != 0 && is_breakable_entity( enter_trace.m_entity ) ) {
					exit_trace       = enter_trace;
					exit_trace.m_end = end + direction;
					return true;
				}

				continue;
			}
		}
	}

	return false;
}

static bool handle_bullet_penetration( sdk::cs_weapon_info* info, fire_bullet_data& data ) {
	auto enter_surface_data              = g_csgo.m_physics_surface_props->get_surface_data( data.m_enter_trace.m_surface.m_surface_props );
	auto enter_material                  = enter_surface_data->m_material;
	auto enter_surf_penetration_modifier = enter_surface_data->m_penetration_modifier;

	data.m_trace_length += data.m_enter_trace.m_fraction * data.m_trace_length_remaining;
	data.m_current_damage *= pow( ( info->m_range_modifier ), ( data.m_trace_length * 0.002F ) );

	// NOTE(para): this implies new server penetration type.
	// we could essentially break most autowalls by using the old one on a server (kek)
	if ( !( data.m_enter_trace.m_contents >> 3 & 1 ) )
		data.m_penetrate_count = 0;

	if ( ( data.m_trace_length > 3000.f ) || ( enter_surf_penetration_modifier < 0.1f ) )
		data.m_penetrate_count = 0;

	if ( data.m_penetrate_count <= 0 )
		return false;

	math::v3f dummy;
	sdk::trace exit_trace;

	if ( !trace_to_exit( g_ctx.m_local, data.m_enter_trace, data.m_enter_trace.m_end, data.m_direction, dummy, exit_trace ) ) {
		if ( ( g_csgo.m_engine_trace->get_point_concents( data.m_enter_trace.m_end, 0x600400b ) & 0x600400b ) == 0 )
			return false;
	}

	auto exit_surface_data              = g_csgo.m_physics_surface_props->get_surface_data( exit_trace.m_surface.m_surface_props );
	auto exit_material                  = exit_surface_data->m_material;
	auto exit_surf_penetration_modifier = exit_surface_data->m_penetration_modifier;
	auto final_damage_modifier          = 0.16F;
	auto combined_penetration_modifier  = 0.F;

	if ( ( ( data.m_enter_trace.m_contents & 8 ) != 0 ) || ( enter_material == 89 ) || ( enter_material ) == 71 ) {
		combined_penetration_modifier = 3.F;
		final_damage_modifier         = 0.05F;
	} else
		combined_penetration_modifier = ( enter_surf_penetration_modifier + exit_surf_penetration_modifier ) * 0.5F;

	if ( enter_material == exit_material ) {
		if ( exit_material == 87 || exit_material == 85 )
			combined_penetration_modifier = 3.F;
		else if ( exit_material == 76 )
			combined_penetration_modifier = 2.F;
	}

	auto v34       = fmaxf( 0.F, 1.F / combined_penetration_modifier );
	auto v35       = ( data.m_current_damage * final_damage_modifier ) + v34 * 3.F * fmaxf( 0.F, ( 3.F / info->m_penetration ) * 1.25F );
	auto thickness = ( exit_trace.m_end - data.m_enter_trace.m_end ).length( );

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.F;

	auto lost_damage = fmaxf( 0.F, v35 + thickness );

	if ( lost_damage > data.m_current_damage )
		return false;

	if ( lost_damage > 0.F )
		data.m_current_damage -= lost_damage;

	if ( data.m_current_damage < 1.F )
		return false;

	data.m_src = exit_trace.m_end;
	data.m_penetrate_count--;

	return true;
}

static bool simulate_fire_bullet( sdk::base_player* target, sdk::weapon_cs_base* weap, sdk::cs_weapon_info* info, fire_bullet_data& data ) {
	data.m_penetrate_count = 4;
	data.m_trace_length    = 0.F;
	data.m_current_damage  = info->m_damage;

	while ( data.m_penetrate_count > 0 && data.m_current_damage >= 1.F ) {
		data.m_trace_length_remaining = info->m_range - data.m_trace_length;

		auto end = data.m_src + data.m_direction * data.m_trace_length_remaining;

		trace_line( data.m_src, end, 0x4600400B, g_ctx.m_local, data.m_enter_trace );

		UTIL_clip_trace_to_players( data.m_src, end + data.m_direction * 40.F, 0x4600400B, data.m_filter, data.m_enter_trace );

		if ( data.m_enter_trace.m_fraction == 1.F )
			break;

		if ( data.m_enter_trace.m_hitgroup != sdk::hit_group::GENERIC && data.m_enter_trace.m_hitgroup != sdk::hit_group::GEAR &&
		     data.m_enter_trace.m_entity && data.m_enter_trace.m_entity->get_networkable_index( ) != 0 && data.m_enter_trace.m_entity == target &&
		     g_ctx.m_local.get( ).is_enemy( ( sdk::cs_player* )data.m_enter_trace.m_entity ) ) {
			data.m_trace_length += data.m_enter_trace.m_fraction * data.m_trace_length_remaining;
			data.m_current_damage *= pow( info->m_range_modifier, data.m_trace_length * 0.002F );

			scale_damage( data.m_enter_trace.m_hitgroup, ( sdk::cs_player* )data.m_enter_trace.m_entity, info->m_armor_ratio, data.m_current_damage );

			return true;
		}

		if ( !handle_bullet_penetration( info, data ) )
			break;
	}

	return false;
}

static bool can_hit( sdk::base_player* t, sdk::weapon_cs_base* weap, sdk::cs_weapon_info* info, const math::v3f& dest, f32& dmg ) {
	auto data     = fire_bullet_data( g_ctx.m_local.get( ).get_eye_position( ) );
	data.m_filter = sdk::trace_filter( g_ctx.m_local );

	auto angles      = data.m_src.calculate_angle( dest );
	data.m_direction = angle_vectors( *( math::ang* )&angles );
	data.m_direction.normalize( );

	if ( simulate_fire_bullet( t, weap, info, data ) ) {
		dmg = data.m_current_damage;

		return true;
	}

	return false;
}

void hack::aimbot::run( f32& x, f32& y ) {
	m_aiming = false;

	sdk::weapon_cs_base* weap = nullptr;
	sdk::cs_weapon_info* info = nullptr;
	if ( g_ctx.m_local.valid( ) && g_ctx.m_local.get( ).is_alive( ) ) {
		if ( weap = g_csgo.m_ent_list->get_handle< sdk::weapon_cs_base* >( g_ctx.m_local.get( ).get_active_weapon( ) ); weap )
			if ( info = weap->get_cs_weapon_info( ); info ) {
				if ( info->m_max_clip1 <= 0 || info->m_type == sdk::weapon_type::KNIFE ||
				     ( info->m_type >= sdk::weapon_type::C4 && info->m_type < sdk::weapon_type::MELEE ) )
					return;
			} else
				return;
		else
			return;

		static auto& fov       = gui::cfg::get< i32 >( HASH_CT( "main:group1:fov" ) );
		static auto& smooth    = gui::cfg::get< i32 >( HASH_CT( "main:group1:smoothing" ) );
		static auto& on_attack = gui::cfg::get< bool >( HASH_CT( "main:group1:on attack" ) );

		auto local_pos   = g_ctx.m_local.get( ).get_eye_position( );
		auto rcs_angle   = g_ctx.m_local.get( ).get_aim_punch_angle( ) * 2;
		auto view_angles = g_csgo.m_engine->get_view_angles( );

		m_best_player = nullptr;
		m_best_fov    = fov;

		if ( fov == 0 )
			return;

		if ( on_attack && !g_io.key_state< io::key_state::DOWN >( VK_LBUTTON ) )
			return;

		g_entity_cacher.for_each( [ & ]( auto& p ) {
			if ( !p.valid( ) )
				return;

			if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || p.get( ).is_dormant( ) || !p.get( ).is_enemy( g_ctx.m_local ) ||
			     p.get( ).is_immune( ) )
				return;

			auto hitbox_pos = p.get( ).get_hitbox_position( sdk::e_hitbox::HEAD );
			if ( hitbox_pos ==
			         math::v3f{ std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) } ||
			     hitbox_pos == math::v3f{ 0, 0, 0 } )
				return;

			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );
			if ( m_rcs )
				aim_angle -= rcs_angle;

			// f32 dmg   = 0;
			// auto scan = can_hit( p, weap, info, hitbox_pos, dmg );
			// g_io.log( XOR( "target: {} scan: {}" ), dmg, scan ? "true" : "false" );
			// if ( !scan )
			// 	return;

			auto dis = get_fov( view_angles, aim_angle );
			if ( m_best_fov > dis ) {
				m_best_player = p;
				m_best_fov    = dis;
			}
		} );

		if ( m_best_player ) {
			auto best_hitbox = [ & ]( ) -> math::v3f {
				auto best_fov    = fov;
				auto best_hitbox = sdk::e_hitbox::HEAD;

				for ( const auto& [ k, e ] : g_aim_hitboxes ) {
					// TODO(para, wini (menu)): verify if k selected, if not, continue

					for ( auto h : e ) {
						auto hitbox_pos = m_best_player->get_hitbox_position( h );
						if ( hitbox_pos == math::v3f{ std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ),
						                              std::numeric_limits< f32 >::max( ) } ||
						     hitbox_pos == math::v3f{ 0, 0, 0 } )
							continue;
						auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
						auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

						if ( m_rcs )
							aim_angle -= rcs_angle;

						// f32 dmg   = 0;
						// auto scan = can_hit( m_best_player, weap, info, hitbox_pos, dmg );
						// g_io.log( XOR( "target: {} scan: {}" ), dmg, scan ? "true" : "false" );
						// if ( !scan )
						// 	continue;

						auto dis = get_fov( view_angles, aim_angle );
						if ( m_best_fov > dis ) {
							best_hitbox = h;
							best_fov    = dis;
						}
					}
				}

				return m_best_player->get_hitbox_position( best_hitbox );
			};

			auto hitbox_pos = best_hitbox( );
			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

			if ( m_rcs )
				aim_angle -= rcs_angle;

			auto _view_delta = ( aim_angle - view_angles ).clamp_angle( );
			auto view_delta  = *( math::v3f* )&_view_delta;
			m_aiming         = true;

			if ( smooth > 1 ) {
				auto move_ang = pixels_to_angle( x, y );

				move_ang /= smooth * 2;

				f32 delta_x = abs( move_ang.pitch );
				f32 delta_y = abs( move_ang.yaw );

				math::ang delta{ 0.f, 0.f, 0.f };
				delta.pitch = std::clamp( view_delta[ 0 ], -delta_x, delta_x );
				delta.yaw   = std::clamp( view_delta[ 1 ], -delta_y, delta_y );

				auto pixels = angle_to_pixels( delta );

				x += pixels.pitch;
				y += pixels.yaw;

			} else {
				auto position = angle_to_pixels( _view_delta );

				x = position.pitch;
				y = position.yaw;
			}
		}
	}
}