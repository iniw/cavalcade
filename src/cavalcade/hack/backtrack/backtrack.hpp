#pragma once

#include "../../../other/other.hpp"
#include "../../../sdk/enums/frame_stage.hpp"

namespace sdk {
	struct cs_player;
} // namespace sdk

namespace hack {
	struct backtrack {
		static f32 get_interpolation( );

		struct tick {
			tick( ) { }
			tick( sdk::cs_player* p );

			f32 m_old_simulation_time{ 0.F };
			f32 m_simulation_time{ 0.F };
			math::v3f m_origin{ };
			math::ang m_abs_angles{ };
			math::v3f m_velocity{ };
			math::v3f m_mins{ };
			math::v3f m_maxs{ };
			math::ang m_ang_eye_pos{ };
			i32 m_flags{ };
			i32 m_eflags{ };
			f32 m_duck_amount{ };
			i32 m_bone_count{ };
			std::array< f32, 24 > m_pose_parameters{ };
			math::matrix_3x4 m_matrix[ 256 ] = { };
		};

		// NOTE(para): maybe move to sdk::player
		std::unordered_map< i32, std::deque< tick > > m_records;

		sdk::cs_player* m_best_player{ nullptr };
		i32 m_best_tick{ std::numeric_limits< i32 >::min( ) };

		bool is_tick_valid( const tick& rec, f32 bt_time );
		void store_records( );
		void apply_record( sdk::cs_player* p, i32 record, bool set_tickcount = false );
		void run( );
		void clear( );
	};
} // namespace hack