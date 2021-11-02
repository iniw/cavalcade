#include "checkbox.hpp"

gui::objects::checkbox::checkbox( std::string_view name, std::string_view label ) {
	// basic identification
	m_name = name;
	m_type = type::CHECKBOX;

	m_label = label;
}

void gui::objects::checkbox::init( ) {
	m_static_rect.pos( m_parent->get_cursor( ) );
	// HARDCODED STYLING!! AAAAAAAAAAAAAAAAAAAAAAAAAA !!
	m_static_rect.size( { 20, 20 } );

	m_dynamic_rect = m_static_rect.shrink( 10 );

	m_parent->push_cursor( m_static_rect[ HEIGHT ] );
}

void gui::objects::checkbox::render( ) {
	g_render.rectangle_filled( m_static_rect, render::color::white( ) ).outline( render::color::gray( ) );

	g_render.rectangle_filled( m_dynamic_rect, m_var ? render::color::red( ) : render::color::gray( ) );
}

bool gui::objects::checkbox::think( ) {
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	// reset all flags
	m_flags.reset( );

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_rect ) );

	if ( m_flags.test( flags::HOVERED ) )
		m_flags.set( flags::ACTIVE, g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON ) );

	if ( m_flags.test( flags::ACTIVE ) ) {
		m_var = !m_var;
		return true;
	}

	return false;
}