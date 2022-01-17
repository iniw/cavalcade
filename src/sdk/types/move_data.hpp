#pragma once

#include "../../other.hpp"

namespace sdk {
	struct move_data {
		bool m_first_run_of_functions : 1;
		bool m_game_code_moved_player : 1;
		bool m_no_air_control         : 1;
		u32 m_player_handle;
		i32 m_impulse_command;
		math::ang m_view_angles;
		math::ang m_abs_view_angles;
		i32 m_buttons;
		i32 m_old_buttons;
		f32 m_forward_move;
		f32 m_side_move;
		f32 m_up_move;
		f32 m_max_speed;
		f32 m_client_max_speed;
		math::v3f m_velocity;
		math::v3f m_trailing_velocity;
		f32 m_trailing_velocity_time;
		math::v3f m_angles;
		math::v3f m_old_angles;
		f32 m_out_step_height;
		math::v3f m_out_wish_vel;
		math::v3f m_out_jump_vel;
		math::v3f m_constraint_center;
		f32 m_constraint_radius;
		f32 m_constraint_width;
		f32 m_constraint_speed_factor;
		bool m_constraint_past_radius;
		math::v3f m_abs_origin;
	};
} // namespace sdk