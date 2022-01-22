#ifndef TEXT_HPP
#define TEXT_HPP

#pragma once

#include "../font/font.hpp"

namespace render::geometry {
	struct text : base_shape {
		point m_point;

	protected:

		const font* m_font;
		std::string m_text;

	public:

		constexpr text( ) = default;

		text( const font* f, const point& p, std::string text, const render::color& col ) : m_point( p ), m_text( std::move( text ) ), m_font( f ) {
			m_color = col;
		}

		void draw( ) override;

		size calc_size( ) const;
	};
} // namespace render::geometry

#endif /* TEXT_HPP */
