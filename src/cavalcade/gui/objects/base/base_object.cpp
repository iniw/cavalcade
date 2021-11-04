#include "base.hpp"

void gui::objects::base_object::identify( parent_ptr parent ) {
	// add to object count and set it to our idx
	m_abs_idx = s_object_count++;

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