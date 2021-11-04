#include "groupbox.hpp"

gui::objects::groupbox::groupbox( std::string_view name, std::string_view label, i32 height ) {
	// basic identification
	m_name  = name;
	m_label = label;
	m_type  = type::GROUPBOX;

	m_height = height;
}

void gui::objects::groupbox::init( ) {
	i32 init_height = m_height == -1 ? 0 : m_height;

	render::size label_size = g_render.text_size< render::font::MENU >( m_label );

	m_static_area.pos( m_parent->get_cursor( ) );
	m_static_area.size( { m_parent->m_dynamic_area[ WIDTH ], general::padding::margin * 4 + init_height } );
	m_static_area[ HEIGHT ] += label_size[ Y ];

	m_dynamic_area = m_static_area.shrink( general::padding::margin );
	// account for our label
	m_dynamic_area[ Y ] += label_size[ Y ];
	m_dynamic_area[ HEIGHT ] -= label_size[ Y ];

	m_cursor = m_dynamic_area.pos( );

	m_label_pos = render::point( ( m_static_area[ X ] + m_static_area[ WIDTH ] / 2 ) - label_size[ X ] / 2, m_static_area[ Y ] );
}

void gui::objects::groupbox::render( ) {
	auto outline_color = m_flags.test( flags::HOVERED ) ? general::pallete::highlight : general::pallete::secondary;

	g_render.rectangle_filled( m_static_area, general::pallete::primary ).outline( outline_color );

	g_render.rectangle_filled( m_dynamic_area, general::pallete::primary ).outline( outline_color );

	g_render.text< render::font::MENU >( m_label_pos, m_label, render::color::white( ) );

	return render_children( );
}

bool gui::objects::groupbox::think( ) {
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	// reset all flags
	m_flags.reset( );

	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_static_area ) );

	return think_children( );
}

void gui::objects::groupbox::on_add_child( base_ptr child ) {
	if ( m_height == -1 ) {
		m_static_area[ HEIGHT ] += child->m_static_area[ HEIGHT ];
		m_dynamic_area[ HEIGHT ] += child->m_static_area[ HEIGHT ];
	}
}