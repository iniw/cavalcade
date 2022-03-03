#pragma once

#include "../../../other/other.hpp"

namespace sdk {
	struct cs_player;
} // namespace sdk

namespace hack {
	struct backtrack {
		struct tick {
			tick( ) : m_simulation_time( std::nullopt ) { }
			tick( sdk::cs_player* p );

			std::optional< f32 > m_simulation_time;
			math::matrix_3x4 m_matrix[ 128 ] = { };
		};

		// NOTE(para): maybe move to sdk::player
		std::unordered_map< i32, std::deque< tick > > m_records;

		sdk::cs_player* m_best_player{ nullptr };
		i32 m_best_tick{ std::numeric_limits< i32 >::min( ) };

		bool is_tick_valid( f32 sim_time, f32 bt_time );
		void store_records( );
		void run( );
		void clear( );
	};
} // namespace hack