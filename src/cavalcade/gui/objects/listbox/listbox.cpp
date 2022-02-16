#include "listbox.hpp"

gui::objects::listbox::listbox( std::string_view label, const options& list, uint32_t max_vis_elements ) : base_object::base_object( label ) {
	m_options = list;

	m_height = max_vis_elements * helpers::list_option::height( );
}

gui::objects::listbox::listbox( std::string_view label, const options& list, preset preset ) : base_object::base_object( label ) {
	m_options = list;

	switch ( preset ) {
	case preset::FULL: {
		m_height = m_options.size( ) * helpers::list_option::height( );
		break;
	}
	case preset::HALF: {
		m_height = m_options.size( ) / 2 * helpers::list_option::height( );
		break;
	}
	}
}

void gui::objects::listbox::init( ) {
	base_child::init( );

	m_static_area.set_pos( m_parent->cursor( ) );
	m_static_area.set_size( { m_parent->dyn_area( ).w, m_height + m_label_size.h + style::padding::obj_spacing } );

	m_dynamic_area.set_pos( { m_static_area.x, m_static_area.y + m_label_size.h + style::padding::obj_spacing } );
	m_dynamic_area.set_size( { m_parent->dyn_area( ).w, m_height } );

	m_cursor = m_dynamic_area.pos( );

	m_label_pos = m_static_area.pos( );

	for ( auto& opt : m_options )
		base_parent::add< helpers::list_option >( opt );

	m_parent->push_cursor( m_static_area.h );
}

void gui::objects::listbox::render( ) const {
	g_render.rectangle_filled( m_dynamic_area, style::palette::background );

	m_children_mgr.render( m_dynamic_area );

	// outline
	g_render.rectangle( m_dynamic_area, style::palette::stroke );

	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );
}

bool gui::objects::listbox::think( ) {
	// allow thinking when we the scrollbar is active if we are not hovered
	if ( !m_dynamic_area.hovered( ) && ( m_scrollbar && !m_scrollbar->flags( ).test( flags::ACTIVE ) ) )
		return false;

	bool active = m_children_mgr.think( );

	if ( active )
		*m_var = options_info::get_active_idx( this );

	return active;
}

void gui::objects::listbox::resize( const render::size& delta ) {
	m_static_area.w += delta.w;
	m_dynamic_area.w += delta.w;
	m_children_mgr.resize( delta );
}