#include "button.hpp"

gui::objects::button::button( std::string_view label, callback_fn fn ) : base_object::base_object( label ) {
	m_callback = fn;
}

void gui::objects::button::init( ) {
	m_static_area.set_pos( m_parent->cursor( ) );
	m_static_area.set_size( { m_parent->dyn_area( ).w, style::sizing::button.h } );

	m_dynamic_area = m_static_area;

	m_label_pos = utils::center_label( m_label, m_static_area );

	m_parent->push_cursor( m_static_area.h );
}

void gui::objects::button::render( ) const {
	g_render.rectangle_filled( m_static_area, style::palette::background ).stroke( style::palette::stroke );

	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );
}

bool gui::objects::button::think( ) {
	return m_static_area.hovered( ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON );
}

void gui::objects::button::resize( const render::size& delta ) {
	m_static_area.w += delta.w;
	m_dynamic_area.w += delta.w;
	m_label_pos = utils::center_label( m_label, m_static_area );
}