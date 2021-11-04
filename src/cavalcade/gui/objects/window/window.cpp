#include "window.hpp"

// screen size
#include "../../../../sdk/csgo/csgo.hpp"

gui::objects::window::window( std::string_view name, std::string_view label, const render::size& size ) {
	// basic identification
	m_name = name;
	m_type = type::WINDOW;

	// NOTE(wini): a bit of a hack but required since
	// this is the only object that gets created manually
	identify( );

	m_label = label;

	render::point center = g_csgo.m_engine->get_screen_size( ) / 2;
	center -= size / 2;

	m_static_area = render::rect( center[ X ], center[ Y ], size[ X ], size[ Y ] );

	m_dynamic_area = m_static_area.shrink( general::padding::margin );

	m_cursor = m_dynamic_area.pos( );
	m_cursor[ Y ] += g_render.text_size< render::font::MENU >( m_label )[ Y ];
	m_cursor[ X ] += general::padding::margin;

	m_label_pos = render::point( m_dynamic_area[ X ] + m_dynamic_area[ WIDTH ] / 2, m_dynamic_area[ Y ] );
}

void gui::objects::window::render( ) {
	auto outline_color = m_flags.test( flags::HOVERED ) ? general::pallete::highlight : general::pallete::secondary;

	g_render.rectangle_filled( m_static_area, general::pallete::primary ).outline( outline_color );

	g_render.rectangle_filled( m_dynamic_area, general::pallete::primary ).outline( general::pallete::secondary );

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

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_area ) );

	return think_children( );
}