#pragma once

#include "rect.hpp"

namespace render::geometry {
	struct rect_filled : rect {
		constexpr rect_filled( ) = default;

		constexpr rect_filled( const point& point1, const point& point2, render::color col ) {
			m_point1    = point1;
			m_point2    = point2;
			m_thickness = 1.f;
			m_color     = col;
		}

		constexpr rect_filled( const render::rect& rect, render::color col, f32 thickness = 0.f ) {
			m_point1    = rect.pos( );
			m_point2    = rect.max( );
			m_thickness = 1.f;
			m_color     = col;
		}

		void draw( ) override;
	};
} // namespace render::geometry