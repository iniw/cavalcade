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

		constexpr rect( const point& point1, const point& point2, render::color col, f32 thickness )
			: m_point1{ point1 }, m_point2{ point2 }, m_thickness{ thickness } {
			m_color = col;
		}

		constexpr rect( const render::rect& rect, render::color col, f32 thickness = 0.f )
			: m_point1{ rect.pos( ) }, m_point2{ rect.max( ) }, m_thickness{ thickness } {
			m_color = col;
		}

		void draw( ) override;

		virtual void outline( color col = color::black( ), bool innerline = false );

		virtual void stroke( color col = color::black( ) );
	};
} // namespace render::geometry