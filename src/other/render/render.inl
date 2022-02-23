#pragma once

template< render::align alignment >
render::geometry::rect render::impl::rectangle( const point& pos, const size& size, color col, f32 thickness ) {
	auto correct_pos = handle_alignment( alignment, pos, size );
	return draw_shape< geometry::rect >( pos, correct_pos, col, thickness );
}

template< render::align alignment >
render::geometry::rect render::impl::rectangle( const rect& rect, color col, f32 thickness ) {
	auto correct_pos = handle_alignment( alignment, rect.pos( ), rect.size( ) );
	return draw_shape< geometry::rect >( rect.pos( ), correct_pos, col, thickness );
}

template< render::align alignment >
render::geometry::rect_filled render::impl::rectangle_filled( const point& pos, const size& size, color col ) {
	auto correct_pos = handle_alignment( alignment, pos, size );
	return draw_shape< geometry::rect_filled >( pos, correct_pos, col );
}

template< render::align alignment >
render::geometry::rect_filled render::impl::rectangle_filled( const rect& rect, color col ) {
	auto correct_pos = handle_alignment( alignment, rect.pos( ), rect.size( ) );
	return draw_shape< geometry::rect_filled >( rect.pos( ), correct_pos, col );
}

template< render::font font_choice >
render::size render::impl::text( const point& pos, std::string_view text, color col, align alignment ) {
	static auto& font = m_fonts.at( font_choice );

	auto size = font.calc_size( text );

	auto correct_pos = handle_alignment( alignment, pos, size );

	font.draw( correct_pos, text, col );

	return size;
}

template< render::font font_choice >
render::size render::impl::text_size( std::string_view text ) {
	static auto& font = m_fonts.at( font_choice );

	return font.calc_size( text );
}