#include "window.hpp"

gui::objects::window::window( std::string_view name, std::string_view label, const render::size& size ) {
	// basic identification
	m_name  = name;
	m_label = label;

	m_size     = size;
	m_dragging = false;

	// NOTE(wini): a bit of a hack but required since
	// this is the only object that gets created manually
	identify( nullptr );

	// center ourselves
	auto center   = ( g_render.get_screen_size( ) - m_size ) / 2;
	m_static_area = render::rect( center[ X ], center[ Y ], m_size[ X ], m_size[ Y ] );

	render::size label_size = g_render.text_size< render::font::MENU >( m_label );

	m_dynamic_area = m_static_area.shrink( general::padding::margin );
	// account for our label
	m_dynamic_area[ Y ] += label_size[ Y ] * 2;
	m_dynamic_area[ HEIGHT ] -= label_size[ Y ] * 2;

	m_cursor = m_dynamic_area.pos( );

	m_label_pos =
		render::point( ( m_static_area[ X ] + m_static_area[ WIDTH ] / 2 ) - label_size[ X ] / 2, m_static_area[ Y ] + label_size[ Y ] / 2 );

	// the drag area is basically the header of the window
	m_drag_area           = m_static_area;
	m_drag_area[ HEIGHT ] = label_size[ Y ] * 2;

	m_resize_area.size( { 50, 50 } );
	m_resize_area.pos( ( m_static_area.pos( ) + m_static_area.size( ) ) - m_resize_area.size( ) );
}

void gui::objects::window::render( ) {
	if ( m_flags.test( flags::DISABLED ) )
		return;

	auto outline_color = m_flags.test( flags::HOVERED ) ? general::pallete::highlight : general::pallete::secondary;

	g_render.rectangle_filled( m_static_area, general::pallete::primary ).outline( outline_color );

	g_render.text< render::font::MENU >( m_label_pos, m_label, general::pallete::text );

	return m_children.render( m_static_area );
}

// TODO(wini): resizing
bool gui::objects::window::think( ) {
	if ( g_io.key_state< io::key_state::RELEASED >( VK_INSERT ) )
		m_flags.flip( flags::DISABLED );

	// we have to handle our flags manually since we are fatherless :(
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	auto mouse_pos = g_io.mouse_pos( );

	m_flags.reset( );

	m_flags.set( flags::HOVERED, mouse_pos.in_rect( m_static_area ) );

	m_flags.set( flags::ACTIVE, m_children.think( ) );

	// return if we have activity before handling dragging
	if ( m_flags.test( flags::ACTIVE ) )
		return true;

	if ( m_dragging || mouse_pos.in_rect( m_drag_area ) && g_io.key_state( VK_LBUTTON ) )
		m_dragging = g_io.key_state( VK_LBUTTON );

	if ( m_dragging ) {
		auto delta = mouse_pos - m_previous_mouse_pos;
		if ( !delta.empty( ) )
			reposition( delta );
	}

	m_previous_mouse_pos = mouse_pos;

	/* TODO(wini): come back to this later
	if ( mouse_pos.in_rect( m_resize_area ) ) {
	    auto delta = mouse_pos - m_previous_mouse_pos;
	    if ( !delta.empty( ) )
	        resize( delta );
	}
	*/

	// it doesn't matter what we return here
	return true;
}

void gui::objects::window::reposition( const render::point& delta ) {
	m_drag_area.pos( m_drag_area.pos( ) + delta );
	m_resize_area.pos( m_resize_area.pos( ) + delta );
	base_parent::reposition( delta );
}

void gui::objects::window::resize( const render::point& delta ) {
	auto size = m_static_area.size( );
	if ( ( size[ X ] + delta[ X ] ) <= m_size[ X ] || ( size[ Y ] + delta[ Y ] ) <= m_size[ Y ] )
		return;

	base_parent::resize( delta );

	// add the new width to the drag area too
	m_drag_area[ WIDTH ] += delta[ X ];

	// add to the position of the resize area, we don't wanna modify it's size
	m_resize_area.pos( m_resize_area.pos( ) + delta );
}