#pragma once

namespace sdk {
	struct cs_weapon_info {
		PAD( 0x14 );
		i32 m_max_clip1;
		i32 m_max_clip2;
		PAD( 0x70 );
		cstr m_hud_name;
		cstr m_weapon_name;
		PAD( 0x34 );
		weapon_type m_type;
		PAD( 4 );
		i32 m_price;
		PAD( 8 );
		f32 m_cycle_time;
		PAD( 12 );
		bool m_full_auto;
		PAD( 3 );
		i32 m_damage;
		f32 m_headshot_multiplier;
		f32 m_armor_ratio;
		i32 m_bullets;
		f32 m_penetration;
		PAD( 8 );
		f32 m_range;
		f32 m_range_modifier;
	};
} // namespace sdk