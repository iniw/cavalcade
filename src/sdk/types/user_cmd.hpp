#pragma once

namespace sdk {
	struct user_cmd {
		PAD( 0x4 );
		i32 m_command_number;
		i32 m_tick_count;
		math::ang m_view_angles;
		math::v3f m_aim_direction;
		f32 m_forward_move;
		f32 m_side_move;
		f32 m_up_move;
		i32 m_buttons;
		i32 m_impulse;
		i32 m_weapon_select;
		i32 m_weapon_sub_type;
		i32 m_random_seed;
		i16 m_moused_x;
		i16 m_moused_y;
		bool m_has_been_predicted;
	};
} // namespace sdk