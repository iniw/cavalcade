#pragma once

#include "../shape.hpp"

namespace render::geometry {
	struct rect : base_shape {
	protected:

		point m_point1;
		point m_point2;
		f32 m_thickness;

	public:

		constexpr rect( ) = default;

		constexpr rect( const point& point1, const point& point2, u32 col, f32 thickness )
			: m_point1{ point1 }, m_point2{ point2 }, m_thickness{ thickness } {
			m_color = col;
		}

		void draw( ) override;

		virtual void outline( color col = color::black( ), bool innerline = false );
	};
} // namespace render::geometry