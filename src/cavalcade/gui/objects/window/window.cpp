#include "window.hpp"

gui::objects::window::window( std::string_view name, std::string_view label, const render::size& size ) {
	// basic identification
	m_name  = name;
	m_label = label;

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
	m_dynamic_area[ HEIGHT ] -= label_size[ Y ] + label_size[ Y ] / 2;

	m_cursor = m_dynamic_area.pos( );

	m_label_pos =
		render::point( ( m_static_area[ X ] + m_static_area[ WIDTH ] / 2 ) - label_size[ X ] / 2, m_static_area[ Y ] + label_size[ Y ] / 2 );

	m_drag_area.pos( m_static_area.pos( ) );
	m_drag_area[ WIDTH ]  = m_static_area[ WIDTH ];
	m_drag_area[ HEIGHT ] = label_size[ Y ] + label_size[ Y ] / 2;
}

void gui::objects::window::render( ) {
	if ( m_flags.test( flags::DISABLED ) )
		return;

	auto outline_color = m_flags.test( flags::HOVERED ) ? general::pallete::highlight : general::pallete::secondary;

	g_render.rectangle_filled( m_static_area, general::pallete::primary ).outline( outline_color );

	g_render.text< render::font::MENU >( m_label_pos, m_label, render::color::white( ) );

	return m_children.render( );
}

// TODO(wini): moving and resizing
bool gui::objects::window::think( ) {
	if ( g_io.key_state< io::key_state::RELEASED >( VK_INSERT ) )
		m_flags.flip( flags::DISABLED );

	// we have to handle our flags manually since we are fatherless :(
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	m_flags.reset( );

	if ( g_io.mouse_pos( ).in_rect( m_drag_area ) ) {
		if ( g_io.key_state( VK_LBUTTON ) ) {
			auto delta = g_io.mouse_pos( ) - m_previous_mouse_pos;
			if ( !delta.empty( ) ) {
				reposition( delta );
			}
		}
	}

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_area ) );

	m_flags.set( flags::ACTIVE, m_children.think( ) );

	m_previous_mouse_pos = g_io.mouse_pos( );

	return m_flags.test( flags::ACTIVE );
}

void gui::objects::window::reposition( const render::point& delta ) {
	m_drag_area.pos( m_drag_area.pos( ) + delta );
	base_parent::reposition( delta );
}