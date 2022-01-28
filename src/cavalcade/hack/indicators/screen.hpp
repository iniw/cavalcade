#ifndef SCREEN_HPP
#define SCREEN_HPP

#pragma once

#include "../../../other/lib/include/animator/animator.hpp"

namespace hack::indicators {
	struct screen {
		void gather( );
		void draw( );
		void clear( );

		std::optional< i32 > m_last_vel;
		bool m_previously_on_ground{ false };
		animator m_anim_last_vel{ };
		f32 m_time{ 0 };
	};
} // namespace hack::indicators

#endif /* SCREEN_HPP */
