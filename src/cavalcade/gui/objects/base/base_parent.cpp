#include "base.hpp"

void gui::objects::base_parent::reposition( const render::point& delta ) {
	base_object::reposition( delta );
	m_cursor += delta;
	// also reposition our children
	m_children.reposition( delta );
}

void gui::objects::base_parent::resize( const render::point& delta ) {
	base_object::resize( delta );
	// also resize our children
	m_children.resize( delta );
}

// TODO(wini): horizontal scrollbar maybe?
void gui::objects::base_parent::on_add_child( const base_ptr& child ) {
	auto child_end_point = child->m_static_area[ Y ] + child->m_static_area[ HEIGHT ];
	auto our_end_point   = m_dynamic_area[ Y ] + m_dynamic_area[ HEIGHT ];
	bool gone_past       = child_end_point > our_end_point;

	if ( gone_past ) {
		if ( !m_scrollbar ) { // create our scrollbar if we don't have one
			m_scrollbar = std::make_shared< scrollbar >( );
			m_scrollbar->identify( get( ) );
			m_scrollbar->init( );
			m_children.add( m_scrollbar );
		} else // update it if we already do
			m_scrollbar->update_height( );
	}
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