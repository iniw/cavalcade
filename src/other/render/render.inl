#ifndef RENDER_INL
#define RENDER_INL

template< render::align alignment >
render::geometry::rect render::render::rectangle( const point& pos, const size& size, color col, f32 thickness ) {
	auto correct_pos = handle_alignment( alignment, pos, size );
	return draw_shape< geometry::rect >( pos, correct_pos, col, thickness );
}

template< render::align alignment >
render::geometry::rect render::render::rectangle( const rect& rect, color col, f32 thickness ) {
	auto correct_pos = handle_alignment( alignment, rect.pos( ), rect.size( ) );
	return draw_shape< geometry::rect >( rect.pos( ), correct_pos, col, thickness );
}

template< render::font font_choice >
render::size render::render::text( const point& pos, std::string_view text, color col ) {
	auto& font = m_fonts.at( ENUM_IDX( font_choice ) );

	font.draw( pos, text, col );

	return font.calc_size( text );
}

#endif // RENDER_INL