

#pragma once

#include "../shape.hpp"

#include "../../types/types.hpp"

namespace render::geometry {
	struct polyfill : base_shape {
		constexpr polyfill( ) = default;
		polyfill( std::vector< render::point >&& ps, render::color clr ) : m_points( std::move( ps ) ) {
			m_color = clr;
		}

		void draw( ) override;

		std::vector< render::point > m_points;
	};
} // namespace render::geometry