#include "checkbox.hpp"
#include "detail.hpp"

void gui::objects::checkbox::init( ) {
	base_child::init( );

	auto size = style::sizing::button / 2;

	m_static_area.set_pos( m_parent->cursor( ) );
	m_static_area.set_size( { size.w + style::padding::obj_spacing + m_label_size.w, size.h + size.h / 2 } );

	m_dynamic_area.set_pos( { m_static_area.x, ( m_static_area.y + m_static_area.h / 2 ) - size.h / 2 } );
	m_dynamic_area.set_size( size );

	m_label_pos = m_static_area.pos( );
	m_label_pos.x += size.w + style::padding::obj_spacing;

	m_parent->push_cursor( m_static_area.h );
}

void gui::objects::checkbox::render( ) const {
	// background & outline
	g_render.rectangle_filled( m_dynamic_area, style::palette::background );

	// inner rect
	g_render.rectangle_filled( m_dynamic_area.shrink( 1 ), style::palette::highlight.mod_alpha( m_alpha ) );

	// outline
	g_render.rectangle( m_dynamic_area, style::palette::stroke );

	// label
	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );
}

bool gui::objects::checkbox::think( ) {
	bool active = m_flags.test( flags::HOVERED ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON );

	if ( active )
		*m_var = !*m_var;

	return active;
}

void gui::objects::checkbox::animate( ) {
	utils::animate( m_alpha, *m_var ? 1.f : 0.f, detail::anim::alpha_speed );
}