#ifndef CS_WEAPON_INFO_HPP
#define CS_WEAPON_INFO_HPP

namespace sdk {
	struct cs_weapon_info {
		PAD( 0x14 );
		i32 m_max_clip1;
		i32 m_max_clip2;
		PAD( 0x70 );
		cstr m_hud_name;
		cstr m_weapon_name;
		PAD( 0x3C );
		weapon_type m_type;
		PAD( 0x24 );
		bool m_full_auto;
		PAD( 0x3 );
		i32 m_damage;
		f32 m_headshot_mult;
		f32 m_armor_ratio;
		PAD( 0x4 );
		f32 m_penetration;
		PAD( 0xC );
		f32 m_range;
		f32 m_range_modifier;
	};
} // namespace sdk

#endif // CS_WEAPON_INFO_HPP