#include "column.hpp"

// TODO(wini): support for more than 2 columns
void gui::objects::column::init( ) {
	auto& parent_area = m_parent->m_dynamic_area;

	// cut our parent's area in half and add a bit of padding
	parent_area[ WIDTH ] /= 2;
	parent_area[ WIDTH ] -= general::padding::margin;

	// copy that area, offset it and account for padding
	m_dynamic_area = parent_area;
	m_dynamic_area[ X ] += parent_area[ WIDTH ] + general::padding::margin * 2;

	m_cursor = m_dynamic_area.pos( );
}

// we don't care about rendering/thinking, that is handled by our parent
void gui::objects::column::render( ) {
	return m_children.render( );
}

bool gui::objects::column::think( ) {
	return m_children.think( );
}

void gui::objects::column::identify( const parent_ptr& parent ) {
	m_abs_idx = s_object_count++;
	m_parent  = parent;

	// instead of creating our own identification, we will instead steal our parent's
	m_name = m_parent->m_name;
	m_id   = m_parent->m_id;
}