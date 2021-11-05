#include "window.hpp"

// screen size
#include "../../../../sdk/csgo/csgo.hpp"

gui::objects::window::window( std::string_view name, std::string_view label, const render::size& size ) {
	// basic identification
	m_name  = name;
	m_label = label;
	m_type  = type::WINDOW;

	// NOTE(wini): a bit of a hack but required since
	// this is the only object that gets created manually
	identify( nullptr );

	// center ourselves
	auto center   = ( g_render.get_screen_size( ) - size ) / 2;
	m_static_area = render::rect( center[ X ], center[ Y ], size[ X ], size[ Y ] );

	render::size label_size = g_render.text_size< render::font::MENU >( m_label );

	m_dynamic_area = m_static_area.shrink( general::padding::margin );
	// account for our label
	m_dynamic_area[ Y ] += label_size[ Y ] + label_size[ Y ] / 2;
	m_dynamic_area[ HEIGHT ] -= label_size[ Y ];

	m_cursor = m_dynamic_area.pos( );

	m_label_pos =
		render::point( ( m_static_area[ X ] + m_static_area[ WIDTH ] / 2 ) - label_size[ X ] / 2, m_static_area[ Y ] + label_size[ Y ] / 2 );
}

void gui::objects::window::render( ) {
	auto outline_color = m_flags.test( flags::HOVERED ) ? general::pallete::highlight : general::pallete::secondary;

	g_render.rectangle_filled( m_static_area, general::pallete::primary ).outline( outline_color );

	g_render.text< render::font::MENU >( m_label_pos, m_label, render::color::white( ) );

	return render_children( );
}

// TODO(wini): moving and resizing
bool gui::objects::window::think( ) {
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	// reset all flags
	m_flags.reset( );

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_area ) );

	return think_children( );
}