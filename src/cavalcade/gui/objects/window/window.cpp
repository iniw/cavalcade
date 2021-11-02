#include "window.hpp"

gui::objects::window::window( std::string_view label, const render::size& size ) {
	// NOTE(wini): a bit of a hack but kind of required since
	// this is the only object that doesn't get added through the add() function
	s_object_count++;

	m_type = type::WINDOW;

	// TODO(wini): center it to the middle of the screen
	m_static_rect = render::rect( 250, 250, size[ X ], size[ Y ] );

	// TODO(wini): this "10" is the harcoded margin
	// refer to the comment about a styling system
	m_dynamic_rect = m_static_rect.shrink( 10 );

	// position our cursor right below the label
	m_cursor = m_dynamic_rect.pos( );
	m_cursor[ Y ] += g_render.text_size< render::font::MENU >( m_label )[ Y ];
}

// TODO(wini): a nice pallete/styling system that gives us the proper color based
// on our current flags, e.g: darker when DISABLED, brigther when ACTIVE
void gui::objects::window::render( ) {
	g_render.rectangle_filled( m_static_rect, render::color::black( ) ).outline( render::color::red( ) );

	g_render.rectangle_filled( m_dynamic_rect, render::color::black( ) ).outline( render::color::gray( ) );

	auto rect_pos  = m_static_rect.pos( );
	auto rect_size = m_static_rect.size( );

	// TODO(wini): cache this value, it's pointless to calculate it every frame
	auto center = render::point( rect_pos[ X ] + rect_size[ X ] / 2, rect_pos[ Y ] );

	// render our label
	g_render.text< render::font::MENU >( center, m_label, render::color::white( ), render::align::X_CENTER );

	// NOTE(wini): purely for debugging purposes
	// render our cursor pos
	g_render.rectangle( m_cursor, { 10, 10 }, render::color::white( ) );

	return render_children( );
}

bool gui::objects::window::think( ) {
	// should maybe reset HOVERED and ACTIVE?
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_rect ) );

	return think_children( );
}