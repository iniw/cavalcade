

#pragma once

#include "rect.hpp"

namespace render::geometry {
	struct rect_grad : rect {
		constexpr rect_grad( ) = default;

		constexpr rect_grad( const point& point1, const point& point2, render::color cola, render::color colb, bool horizontal = false ) {
			m_point1     = point1;
			m_point2     = point2;
			m_thickness  = 1.f;
			m_color      = cola;
			m_color2     = colb;
			m_horizontal = horizontal;
		}

		void draw( ) override;

	protected:

		render::color m_color2;
		bool m_horizontal;
	};
} // namespace render::geometry