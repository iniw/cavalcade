#include "text.hpp"

#include "../../types/types.hpp"

void render::geometry::text::draw( ) {
	m_font->draw( m_point, m_text, m_color );
}

render::size render::geometry::text::calc_size( std::string_view text ) const {
	return m_font->calc_size( text );
}