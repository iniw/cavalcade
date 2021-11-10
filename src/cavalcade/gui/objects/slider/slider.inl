#pragma once

#include "styling.hpp"

// NOTE(wini): i'm obligated to write this-> on this file because of template bullshitery
// https://stackoverflow.com/questions/6592512/templates-parent-class-member-variables-not-visible-in-inherited-class

template< math::Number T >
gui::objects::slider< T >::slider( std::string_view name, std::string_view label, T min, T max, i32 precision ) {
	// basic identification
	this->m_name  = name;
	this->m_label = label;

	this->m_dragging = false;

	this->m_min = min;
	this->m_max = max;

	this->m_precision = precision;
}

template< math::Number T >
void gui::objects::slider< T >::init( ) {
	this->m_var = cfg::add_entry< T >( this->m_id >> 32 );

	auto& cursor = this->m_parent->get_cursor( );

	auto label_size = g_render.text_size< render::font::MENU >( this->m_label );

	i32 static_height = 0;
	static_height += label_size[ Y ];
	static_height += personal::padding::label;
	static_height += personal::sizing::height;

	this->m_static_area.pos( cursor );
	this->m_static_area.size( { this->m_parent->m_dynamic_area[ WIDTH ], static_height } );

	this->m_dynamic_area.pos( { cursor[ X ], cursor[ Y ] + label_size[ Y ] + personal::padding::label } );
	this->m_dynamic_area.size( { this->m_parent->m_dynamic_area[ WIDTH ], personal::sizing::height } );

	this->m_label_pos = cursor;

	if constexpr ( std::is_floating_point_v< T > )
		this->m_var_text = io::format( "{:.{}f}", *this->m_var, this->m_precision );
	else
		this->m_var_text = fmt::to_string( *this->m_var );

	this->m_var_text_pos = render::point( this->m_label_pos[ X ] + this->m_dynamic_area[ WIDTH ], this->m_label_pos[ Y ] );

	this->m_parent->push_cursor( this->m_static_area[ HEIGHT ] );
}

template< math::Number T >
void gui::objects::slider< T >::render( ) {
	g_render.rectangle( this->m_dynamic_area, general::pallete::primary ).outline( general::pallete::secondary );

	f32 value_width = *this->m_var / static_cast< f32 >( m_max ) * this->m_dynamic_area[ WIDTH ];

	g_render.rectangle_filled( this->m_dynamic_area.pos( ), { value_width, this->m_dynamic_area[ HEIGHT ] }, general::pallete::highlight );

	g_render.text< render::font::MENU >( this->m_label_pos, this->m_label, general::pallete::text );

	g_render.text< render::font::MENU >( this->m_var_text_pos, this->m_var_text, general::pallete::text, render::align::LEFT );
}

template< math::Number T >
bool gui::objects::slider< T >::think( ) {
	if ( m_dragging || this->m_flags.test( flags::HOVERED ) && g_io.key_state( VK_LBUTTON ) )
		m_dragging = g_io.key_state( VK_LBUTTON );

	if ( !m_dragging )
		return false;

	auto mouse_pos = g_io.mouse_pos( );

	T delta = mouse_pos[ X ] - this->m_dynamic_area[ X ];
	T value = delta * m_max / static_cast< T >( this->m_dynamic_area[ WIDTH ] );

	*this->m_var = std::clamp( value, m_min, m_max );

	if constexpr ( std::is_floating_point_v< T > )
		this->m_var_text = io::format( "{:.{}f}", *this->m_var, this->m_precision );
	else
		this->m_var_text = fmt::to_string( *this->m_var );

	return true;
}

template< math::Number T >
void gui::objects::slider< T >::reposition( const render::point& delta ) {
	this->m_var_text_pos += delta;
	base_object::reposition( delta );
}

template< math::Number T >
void gui::objects::slider< T >::resize( const render::point& delta ) {
	base_object::resize( delta );

	// compensate for the added height
	this->m_dynamic_area[ HEIGHT ] -= delta[ Y ];
	this->m_static_area[ HEIGHT ] -= delta[ Y ];

	// add the new width to the text pos too
	this->m_var_text_pos[ X ] += delta[ X ];
}