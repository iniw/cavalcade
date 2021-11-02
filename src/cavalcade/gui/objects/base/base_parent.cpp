#include "base.hpp"

gui::objects::parent_ptr gui::objects::base_parent::get( ) {
	return shared_from_this( );
}

void gui::objects::base_parent::on_add_child( base_ptr child ) { }

bool gui::objects::base_parent::think_children( ) {
	for ( auto& child : m_children )
		if ( child->think( ) )
			return child->m_time = m_time = GetTickCount64( );

	return false;
}

void gui::objects::base_parent::render_children( ) {
	for ( auto& child : m_children )
		child->render( );
}

const render::point& gui::objects::base_parent::get_cursor( ) {
	return m_cursor;
}

void gui::objects::base_parent::push_cursor( i32 offset ) {
	// HARDCODED STYLING!! AAAAAAAAAAAAAAAAAAAAA
	m_cursor[ Y ] += offset + 2;
}