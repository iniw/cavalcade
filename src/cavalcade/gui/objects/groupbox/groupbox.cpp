#include "groupbox.hpp"

gui::objects::groupbox::groupbox( std::string_view label, uint32_t height ) : base_object::base_object( label ) {
	m_height = height;
}

gui::objects::groupbox::groupbox( std::string_view label, preset preset ) : base_object::base_object( label ) {
	m_preset = preset;
}

void gui::objects::groupbox::init( ) {
	switch ( m_preset ) {
	case preset::FULL:
		m_height = m_parent->dyn_area( ).h;
		break;
	case preset::HALF: {
		m_height = m_parent->dyn_area( ).h / 2 - style::padding::obj_spacing / 2;
		break;
	}
	case preset::DYNAMIC:
		m_height = style::padding::margin * 2 + m_label_size.h;
		break;
	}

	m_static_area.set_pos( m_parent->cursor( ) );
	m_static_area.set_size( { m_parent->dyn_area( ).w, m_height } );

	m_dynamic_area = m_static_area;
	// account for our label
	m_dynamic_area.vertical_cut( m_label_size.h );
	// shrink it down a bit
	m_dynamic_area = m_dynamic_area.shrink( style::padding::margin );

	m_label_pos.x = utils::center_label( m_label, m_static_area ).x;
	m_label_pos.y = m_static_area.y + m_label_size.h / 4;

	m_cursor = m_dynamic_area.pos( );

	m_parent->push_cursor( m_static_area.h );
}

void gui::objects::groupbox::render( ) const {
	g_render.rectangle_filled( m_static_area, style::palette::secondary );

	// outline
	g_render.rectangle( m_static_area, style::palette::stroke );

	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );

	m_children_mgr.render( m_dynamic_area );
}

bool gui::objects::groupbox::think( ) {
	return m_children_mgr.think( ) || ( m_flags.test( flags::HOVERED ) && m_scrollbar && !m_scrollbar->disabled( ) );
}

void gui::objects::groupbox::on_add_child( base_object* child ) {
	// update our height if we are dynamic
	if ( m_preset == preset::DYNAMIC ) {
		auto delta = m_cursor.y - m_dynamic_area.y2( );
		if ( delta > 0 ) {
			m_static_area.h += delta;
			m_dynamic_area.h += delta;
			m_height += delta;
			if ( m_parent->cursor( ).in_rect( m_static_area ) ) {
				m_parent->add_to_cursor( delta );
				m_parent->on_add_child( child );
			}
		}
	} else
		base_parent::on_add_child( child );
}

void gui::objects::groupbox::resize( const render::size& delta ) {
	m_label_pos.x = utils::center_label( m_label, m_static_area ).x;
	m_label_pos.y = m_static_area.y + m_label_size.h / 4;
	base_parent::resize( delta );
}