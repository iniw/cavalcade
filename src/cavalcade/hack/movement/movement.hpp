#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#pragma once

#include "../../../other/other.hpp"

namespace hack {
	struct movement {
		void pre( );

		void no_duck_delay( );
		void bunnyhop( );
		void jumpbug( );
		void longjump( );

		void post( );

		i32 m_base_flags{ };
		bool m_in_jumpbug{ false };
		bool m_jumpbugged{ false };
		i32 m_lj_ducked_ticks{ 0 };

		f32 m_old_velocity_z;
	};
} // namespace hack

#endif /* MOVEMENT_HPP */
