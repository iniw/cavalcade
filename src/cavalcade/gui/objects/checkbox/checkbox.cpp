#include "checkbox.hpp"
#include "styling.hpp"

gui::objects::checkbox::checkbox( std::string_view name, std::string_view label ) {
	// basic identification
	m_name  = name;
	m_label = label;
	m_type  = type::CHECKBOX;
}

void gui::objects::checkbox::init( ) {
	auto cursor       = m_parent->get_cursor( );
	auto label_offset = personal::padding::label + g_render.text_size< render::font::MENU >( m_label )[ X ];

	m_static_area.pos( cursor );
	m_static_area.size( personal::sizing::main );
	m_static_area[ WIDTH ] += label_offset;

	m_dynamic_area.pos( cursor );
	m_dynamic_area.size( personal::sizing::main );
	m_dynamic_area = m_dynamic_area.shrink( 2 );

	m_label_pos = cursor;
	m_label_pos[ X ] += personal::sizing::main[ X ] + personal::padding::label;

	m_parent->push_cursor( m_static_area[ HEIGHT ] );
}

void gui::objects::checkbox::render( ) {
	g_render.rectangle_filled( m_static_area, general::pallete::primary )
		.outline( m_flags.test( flags::HOVERED ) ? general::pallete::highlight : general::pallete::secondary );

	g_render.rectangle_filled( m_dynamic_area, m_var ? general::pallete::highlight : general::pallete::primary )
		.outline( general::pallete::secondary );

	g_render.text< render::font::MENU >( m_label_pos, m_label, render::color::white( ) );
}

bool gui::objects::checkbox::think( ) {
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	m_flags.reset( );

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_area ) );

	m_flags.set( flags::ACTIVE, m_flags.test( flags::HOVERED ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON ) );

	bool active = m_flags.test( flags::ACTIVE );

	if ( active )
		m_var = !m_var;

	return active;
}