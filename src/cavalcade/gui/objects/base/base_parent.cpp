#include "base.hpp"

void gui::objects::base_parent::on_add_child( base_ptr child ) {
	return;
}

bool gui::objects::base_parent::think_children( ) {
	std::ranges::sort( m_children, [ & ]( base_ptr a, base_ptr b ) { return a->m_time > b->m_time; } );

	for ( auto& child : m_children )
		if ( child->think( ) )
			return child->m_time = m_time = GetTickCount64( );

	return false;
}

void gui::objects::base_parent::render_children( ) {
	std::ranges::sort( m_children, [ & ]( base_ptr a, base_ptr b ) { return a->m_time < b->m_time; } );

	for ( auto& child : m_children )
		child->render( );
}

const render::point& gui::objects::base_parent::get_cursor( ) {
	return m_cursor;
}

void gui::objects::base_parent::push_cursor( i32 offset ) {
	m_cursor[ Y ] += offset + general::padding::obj_margin;
}

void gui::objects::base_parent::set_cursor( const render::point& val ) {
	m_cursor = val;
}