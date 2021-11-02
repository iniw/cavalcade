#ifndef RECT_FILLED_HPP
#define RECT_FILLED_HPP

#include "rect.hpp"

namespace render::geometry {
	struct rect_filled : rect {
		constexpr rect_filled( ) = default;

		constexpr rect_filled( const point& point1, const point& point2, u32 col ) {
			m_point1    = point1;
			m_point2    = point2;
			m_thickness = 1.f;
			m_color     = col;
		}

		void draw( ) override;
	};
} // namespace render::geometry

#endif // RECT_FILLED_HPP