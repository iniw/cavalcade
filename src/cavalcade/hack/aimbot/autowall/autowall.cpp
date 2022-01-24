#include "autowall.hpp"
#include "../../../ctx/ctx.hpp"
#include "../../../entity_cacher/entity_cacher.hpp"

void autowall::scale_damage( i32 hitgroup, sdk::cs_player* e, f32 weapon_armor_ratio, f32& dmg ) {
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

void autowall::UTIL_clip_trace_to_players( const math::v3f& abs_start, const math::v3f& abs_end, unsigned int mask, sdk::trace_filter& filter,
                                           sdk::trace& enter_trace, f32 min_range ) {
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

void autowall::trace_line( const math::v3f& abs_start, const math::v3f& abs_end, unsigned int mask, sdk::base_entity* filter_ent,
                           sdk::trace& trace ) {
	auto ray      = sdk::ray( abs_start, abs_end );
	auto filter   = sdk::trace_filter{ };
	filter.m_skip = filter_ent;

	g_csgo.m_engine_trace->trace_ray( ray, mask, filter, trace );
}

bool autowall::is_breakable_entity( sdk::base_entity* e ) {
	static auto og = g_mem[ CLIENT_DLL ].get_address< bool( __thiscall* )( sdk::base_entity* ) >( HASH_CT( "IsBreakableEntity" ) );
	return og( e );
}

bool autowall::trace_to_exit( sdk::base_entity* target, const sdk::trace& enter_trace, const math::v3f& start, const math::v3f& direction,
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

bool autowall::handle_bullet_penetration( sdk::cs_weapon_info* info, fire_bullet_data& data ) {
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

bool autowall::simulate_fire_bullet( sdk::base_player* target, sdk::weapon_cs_base* weap, sdk::cs_weapon_info* info, fire_bullet_data& data ) {
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

bool autowall::can_hit( sdk::base_player* t, sdk::weapon_cs_base* weap, sdk::cs_weapon_info* info, const math::v3f& dest, f32& dmg ) {
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