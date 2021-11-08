#include "base.hpp"

void gui::objects::base_parent::on_add_child( base_ptr child ) {
	return;
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