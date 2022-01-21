#pragma once

#include "rect.hpp"

namespace render::geometry {
	struct rect_filled : rect {
		constexpr rect_filled( ) = default;

		constexpr rect_filled( const point& point1, const point& point2, const render::color& col ) {
			m_point1    = point1;
			m_point2    = point2;
			m_thickness = 1.f;
			m_color     = col;
		}

		void draw( ) override;
	};
} // namespace render::geometry