#include "base.hpp"

void gui::objects::base_object::identify( const parent_ptr& parent ) {
	m_abs_idx = s_object_count++;

	if ( parent && parent->m_children.size( ) )
		m_idx = parent->m_children.size( ); // we set it to size here because we haven't added to the list yet

	// set our parent
	m_parent = parent;

	// add our parent's name, if we have one
	if ( m_parent && !m_parent->m_name.empty( ) )
		m_name = m_parent->m_name + NAME_SEPARATOR + m_name;

	// set our id
	m_id = HASH_RT( m_name );
	m_id <<= 32;
	m_id |= m_abs_idx;
}

void gui::objects::base_object::reposition( const render::point& delta ) {
	// holy cancer syntax!
	m_static_area.pos( m_static_area.pos( ) + delta );
	m_dynamic_area.pos( m_dynamic_area.pos( ) + delta );
	m_label_pos += delta;
}

void gui::objects::base_object::resize( const render::point& delta ) {
	// holy cancer syntax!
	m_static_area.size( m_static_area.size( ) + delta );
	m_dynamic_area.size( m_dynamic_area.size( ) + delta );
}