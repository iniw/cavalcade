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
	};
} // namespace sdk