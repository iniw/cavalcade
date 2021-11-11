#include "column.hpp"

// TODO(wini): support for more than 2 columns
void gui::objects::column::init( ) {
	auto& parent_dyn_area = m_parent->m_dynamic_area;
	auto& parent_sta_area = m_parent->m_static_area;

	// cut our parent's area in half and add a bit of padding
	parent_dyn_area[ WIDTH ] /= 2;
	parent_dyn_area[ WIDTH ] -= general::padding::margin;

	parent_sta_area[ WIDTH ] /= 2;
	parent_sta_area[ WIDTH ] -= general::padding::margin;

	// copy th area, offset it and account for padding
	m_dynamic_area = parent_dyn_area;
	m_dynamic_area[ X ] += parent_dyn_area[ WIDTH ] + general::padding::margin * 2;

	m_static_area = parent_sta_area;
	m_static_area[ X ] += m_static_area[ WIDTH ] + general::padding::margin * 2;

	m_cursor = m_dynamic_area.pos( );

	// handle resizing of children that already exist
	for ( auto& child : m_parent->m_children ) {
		if ( m_parent->m_scrollbar && child == m_parent->m_scrollbar ) {
			m_parent->m_scrollbar->reposition( { -m_dynamic_area[ WIDTH ] - general::padding::margin * 2, 0 } );
			continue;
		}
		child->resize( { -m_dynamic_area[ WIDTH ] - general::padding::margin * 2, 0 } );
	}
}

// we don't care about rendering/thinking, that is handled by our parent
void gui::objects::column::render( ) const {
	return m_children.render( m_dynamic_area, this );
}

bool gui::objects::column::think( ) {
	return m_children.think( this );
}

void gui::objects::column::identify( base_parent* parent ) {
	m_abs_idx = s_object_count++;
	m_parent  = parent;

	// instead of creating our own identification, we will instead steal our parent's
	m_name = m_parent->m_name;
	m_id   = m_parent->m_id;
}