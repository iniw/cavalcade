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

static bool trace_to_exit( const sdk::trace& enter_trace, const math::v3f& start, const math::v3f& direction, math::v3f& end,
                           sdk::trace& exit_trace ) {
	bool result     = false;
	static auto tte = g_mem[ CLIENT_DLL ].get_address< uintptr_t >( HASH_CT( "TraceToExit" ) );

	auto dirx = direction[ 0 ];
	auto diry = direction[ 1 ];
	auto dirz = direction[ 2 ];

	auto stax = start[ 0 ];
	auto stay = start[ 1 ];
	auto staz = start[ 2 ];

	// custom calling convention moment
	__asm {
		push 0
        push 0
        push 0
        push exit_trace
        push dirz
        push diry
        push dirx
        push staz
        push stay
        push stax
        mov edx, enter_trace
        mov ecx, end
        call tte
        add esp, 40
        mov result, al
	}
	return result;
}

static f32 handle_bullet_penetration( sdk::surface_data* enter_surface_data, const sdk::trace& enter_trace, const math::v3f& direction,
                                      math::v3f& result, f32 penetration, f32 damage ) {
	math::v3f end;
	sdk::trace exit_trace;

	if ( !trace_to_exit( enter_trace, enter_trace.m_end, direction, end, exit_trace ) )
		return -1.F;

	const auto exit_surface_data = g_csgo.m_physics_surface_props->get_surface_data( exit_trace.m_surface.m_surface_props );

	f32 damage_modifier      = 0.16F;
	f32 penetration_modifier = ( enter_surface_data->m_penetration_modifier + exit_surface_data->m_penetration_modifier ) / 2.F;

	if ( enter_surface_data->m_material == 71 || enter_surface_data->m_material == 89 ) {
		damage_modifier      = 0.05F;
		penetration_modifier = 3.F;
	} else if ( enter_trace.m_contents >> 3 & 1 || enter_trace.m_surface.m_flags >> 7 & 1 ) {
		penetration_modifier = 1.F;
	}

	if ( enter_surface_data->m_material == exit_surface_data->m_material ) {
		if ( exit_surface_data->m_material == 85 || exit_surface_data->m_material == 87 )
			penetration_modifier = 3.F;
		else if ( exit_surface_data->m_material == 76 )
			penetration_modifier = 2.F;
	}

	damage -= 11.25F / penetration / penetration_modifier + damage * damage_modifier +
	          ( exit_trace.m_end - enter_trace.m_end ).length_sqr( ) / 24.F / penetration_modifier;

	result = exit_trace.m_end;
	return damage;
}

static bool can_scan( sdk::cs_player* e, const math::v3f& dest, sdk::cs_weapon_info* info, i32 min_dmg, bool allow_friendly_fire ) {
	auto dmg = ( f32 )info->m_damage;

	auto start{ g_ctx.m_local.get( ).get_eye_position( ) };
	auto direction = dest - start;
	direction /= direction.length( );

	i32 hits_left = 4;

	while ( dmg >= 1.F && hits_left ) {
		sdk::trace trace{ };
		g_csgo.m_engine_trace->trace_ray( { start, dest }, 0x4600400B, sdk::trace_filter( g_ctx.m_local ), trace );

		if ( !allow_friendly_fire && trace.m_entity && ( sdk::cs_player* )( trace.m_entity )->is_player( ) &&
		     !g_ctx.m_local.get( ).is_other_enemy( ( sdk::cs_player* )( trace.m_entity ) ) )
			return false;

		if ( trace.m_fraction == 1.F )
			break;

		if ( trace.m_entity == e && trace.m_hitgroup > sdk::hit_group::GENERIC && trace.m_hitgroup <= sdk::hit_group::RIGHT_LEG ) {
			dmg = sdk::hit_group::get_damage_multiplier( trace.m_hitgroup ) * dmg *
			      std::pow( info->m_range_modifier, trace.m_fraction * info->m_range / 500.F );

			auto p = ( ( sdk::cs_player* )( trace.m_entity ) );
			if ( f32 armor_ratio{ info->m_armor_ratio / 2.F }; sdk::hit_group::is_armored( trace.m_hitgroup, p->has_helmet( ) ) )
				dmg -= ( p->armor_value( ) < dmg * armor_ratio / 2.F ? p->armor_value( ) * 4.F : dmg ) * ( 1.F - armor_ratio );

			return dmg >= min_dmg;
		}

		const auto surface_data = g_csgo.m_physics_surface_props->get_surface_data( trace.m_surface.m_surface_props );
		if ( surface_data->m_penetration_modifier < 0.1F )
			break;

		dmg = handle_bullet_penetration( surface_data, trace, direction, start, info->m_penetration, dmg );
		hits_left--;
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

			auto dis = get_fov( view_angles, aim_angle );
			if ( m_best_fov > dis ) {
				if ( !can_scan( p, hitbox_pos, info, 5, false ) )
					return;

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

						auto dis = get_fov( view_angles, aim_angle );
						if ( m_best_fov > dis ) {
							if ( !can_scan( m_best_player, hitbox_pos, info, 5, false ) )
								continue;

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