#ifndef HITMARKER_HPP
#define HITMARKER_HPP

#pragma once

#include "../../../other/other.hpp"
#include "../../../other/lib/include/animator/animator.hpp"

namespace hack {
	struct hitmark {
		math::v3f m_pos{ };
		f32 m_time{ 0 };
		i32 m_alpha{ 255 };
	};

	struct hitmarker {
		void push( hitmark&& );
		void draw( );
		void clear( );

		std::vector< hitmark > m_hits;
	};
} // namespace hack

#endif /* HITMARKER_HPP */
