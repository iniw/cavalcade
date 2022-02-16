

#pragma once

#include "../../../../other/other.hpp"
#include "../../../../sdk/types/interfaces/enginetrace.hpp"

namespace sdk {
	struct base_player;
	struct cs_player;
	struct weapon_cs_base;
	struct cs_weapon_info;
} // namespace sdk

static math::v3f angle_vectors( const math::ang& angles ) {
	constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };

	f32 sp{ }, sy{ }, cp{ }, cy{ };

	sy = sin( DEG2RAD( angles.yaw ) );
	cy = cos( DEG2RAD( angles.yaw ) );

	sp = sin( DEG2RAD( angles.pitch ) );
	cp = cos( DEG2RAD( angles.pitch ) );

	return { cp * cy, cp * sy, -sp };
}

namespace autowall {
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

	extern void scale_damage( i32 hitgroup, sdk::cs_player* e, f32 weapon_armor_ratio, f32& dmg );
	extern void UTIL_clip_trace_to_players( const math::v3f& abs_start, const math::v3f& abs_end, unsigned int mask, sdk::trace_filter& filter,
	                                        sdk::trace& enter_trace, f32 min_range = -60.F );
	extern void trace_line( const math::v3f& abs_start, const math::v3f& abs_end, unsigned int mask, sdk::base_entity* filter_ent,
	                        sdk::trace& trace );
	extern bool is_breakable_entity( sdk::base_entity* e );
	extern bool trace_to_exit( sdk::base_entity* target, const sdk::trace& enter_trace, const math::v3f& start, const math::v3f& direction,
	                           math::v3f& end, sdk::trace& exit_trace );
	extern bool handle_bullet_penetration( sdk::cs_weapon_info* info, fire_bullet_data& data );
	extern bool simulate_fire_bullet( sdk::base_player* target, sdk::weapon_cs_base* weap, sdk::cs_weapon_info* info, fire_bullet_data& data );
	extern bool can_hit( sdk::base_player* t, sdk::weapon_cs_base* weap, sdk::cs_weapon_info* info, const math::v3f& dest, f32& dmg );
} // namespace autowall