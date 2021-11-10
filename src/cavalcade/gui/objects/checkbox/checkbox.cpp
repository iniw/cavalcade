#include "checkbox.hpp"
#include "styling.hpp"

gui::objects::checkbox::checkbox( std::string_view name, std::string_view label ) {
	// basic identification
	m_name  = name;
	m_label = label;
}

void gui::objects::checkbox::init( ) {
	auto& cursor      = m_parent->get_cursor( );
	auto label_offset = personal::padding::label + g_render.text_size< render::font::MENU >( m_label )[ X ];

	m_static_area.pos( cursor );
	m_static_area.size( personal::sizing::main );

	m_dynamic_area = m_static_area.shrink( 2 );

	m_static_area[ WIDTH ] += label_offset;

	m_label_pos = cursor;
	m_label_pos[ X ] += personal::sizing::main[ X ] + personal::padding::label;

	m_parent->push_cursor( m_static_area[ HEIGHT ] );
}

void gui::objects::checkbox::render( ) {
	auto color = m_var ? general::pallete::highlight : general::pallete::primary;

	g_render.rectangle_filled( m_dynamic_area, color ).outline( general::pallete::secondary );

	g_render.text< render::font::MENU >( m_label_pos, m_label, general::pallete::text );
}

bool gui::objects::checkbox::think( ) {
	bool active = m_flags.test( flags::HOVERED ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON );

	if ( active )
		m_var = !m_var;

	return active;
}