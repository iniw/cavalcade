#include "window.hpp"

gui::objects::window::window( std::string_view name, std::string_view label, const render::size& size ) {
	// basic identification
	m_name = name;
	m_type = type::WINDOW;

	// NOTE(wini): a bit of a hack but required since
	// this is the only object that gets created manually
	identify( );

	m_label = label;

	// TODO(wini): center it to the middle of the screen
	m_static_rect = render::rect( 100, 100, size[ X ], size[ Y ] );

	// TODO(wini): this "5" is the harcoded margin
	// refer to the comment about a styling system
	m_dynamic_rect = m_static_rect.shrink( 5 );

	// position our cursor right below the label
	m_cursor = m_dynamic_rect.pos( );
	m_cursor[ Y ] += g_render.text_size< render::font::MENU >( m_label )[ Y ];

	m_label_pos = render::point( m_dynamic_rect[ X ] + m_dynamic_rect[ WIDTH ] / 2, m_dynamic_rect[ Y ] );
}

// TODO(wini): a nice pallete/styling system that gives us the proper color based
// on our current flags, e.g: darker when DISABLED, brigther when ACTIVE
void gui::objects::window::render( ) {
	g_render.rectangle_filled( m_static_rect, render::color::black( ) ).outline( render::color::gray( ) );

	g_render.rectangle_filled( m_dynamic_rect, render::color::black( ) )
		.outline( m_flags.test( flags::HOVERED ) ? render::color::gray( ) : render::color::white( ) );

	g_render.text< render::font::MENU >( m_label_pos, m_label, render::color::white( ), render::align::X_CENTER );

	// NOTE(wini): purely for debugging purposes
	// render our cursor pos
	g_render.rectangle( m_cursor, { 10, 10 }, render::color::white( ) );

	return render_children( );
}

bool gui::objects::window::think( ) {
	// TODO(wini): moving and resizing (this is gonna be hard)

	// should maybe reset HOVERED and ACTIVE?
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	// reset all flags
	m_flags.reset( );

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_rect ) );

	return think_children( );
}