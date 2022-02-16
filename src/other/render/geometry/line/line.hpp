

#pragma once

#include "../shape.hpp"

#include "../../types/types.hpp"

namespace render::geometry {
	struct line : base_shape {
		constexpr line( ) = default;

		constexpr line( const point& point1, const point& point2, render::color col, f32 thickness = 1.F ) {
			m_point1    = point1;
			m_point2    = point2;
			m_thickness = 1.f;
			m_color     = col;
			m_thickness = thickness;
		}

		void draw( ) override;

	protected:

		point m_point1;
		point m_point2;
		f32 m_thickness;
	};
} // namespace render::geometry