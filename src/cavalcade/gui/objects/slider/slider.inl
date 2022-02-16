#pragma once

#include "detail.hpp"

// NOTE(wini): i'm obligated to write this-> on this file because of template bullshitery
// https://stackoverflow.com/questions/6592512/templates-parent-class-member-variables-not-visible-in-inherited-class

template< math::Number T >
gui::objects::slider< T >::slider( std::string_view label, T min, T max, uint8_t precision ) : base_object::base_object( label ) {
	this->m_min       = min;
	this->m_max       = max;
	this->m_precision = precision;
}

template< math::Number T >
void gui::objects::slider< T >::init( ) {
	base_child< T >::init( );

	int static_height = 0;
	static_height += this->m_label_size.h;
	static_height += style::padding::obj_spacing;
	static_height += style::sizing::button.h / 2;

	this->m_static_area.set_pos( this->m_parent->cursor( ) );
	this->m_static_area.set_size( { this->m_parent->dyn_area( ).w, static_height } );

	this->m_dynamic_area.set_pos( { this->m_static_area.x, this->m_static_area.y + this->m_label_size.h + style::padding::obj_spacing } );
	this->m_dynamic_area.set_size( { this->m_parent->dyn_area( ).w, style::sizing::button.h / 2 } );

	this->m_label_pos = this->m_static_area.pos( );

	if constexpr ( std::is_floating_point_v< T > )
		this->m_var_text = fmt::format( "{:.{}f}", *this->m_var, this->m_precision );
	else
		this->m_var_text = fmt::to_string( *this->m_var );

	this->m_var_text_pos = { this->m_label_pos.x + this->m_dynamic_area.w, this->m_label_pos.y };

	this->m_parent->push_cursor( this->m_static_area.h );
}

template< math::Number T >
void gui::objects::slider< T >::render( ) const {
	// total bar
	g_render.rectangle_filled( this->m_dynamic_area, style::palette::background );

	// value bar
	auto value_rect = this->m_dynamic_area.shrink( 1 );
	value_rect.w    = std::roundl( m_val_width );
	g_render.rectangle_filled( value_rect, style::palette::highlight );

	// outline
	g_render.rectangle( this->m_dynamic_area, style::palette::stroke );

	// slider text
	g_render.text< render::font::MENU >( this->m_label_pos, this->m_label, style::palette::text );

	// slider value
	g_render.text< render::font::MENU >( this->m_var_text_pos, this->m_var_text, style::palette::text, render::align::RIGHT );
}

template< math::Number T >
bool gui::objects::slider< T >::think( ) {
	if ( m_dragging || this->m_dynamic_area.hovered( ) )
		m_dragging = g_io.key_state( VK_LBUTTON );

	T last_val    = *this->m_var;
	bool uncapped = g_io.key_state( VK_SHIFT );

	// dragging slider
	if ( m_dragging )
		*this->m_var = m_min + this->m_dynamic_area.mouse_percent_x( uncapped ) * m_max;

	// precise value adjustment with arrow keys
	if ( this->m_dynamic_area.hovered( ) ) {
		if ( g_io.key_state< io::key_state::RELEASED >( VK_RIGHT ) ) {
			if ( *this->m_var != std::ceil( *this->m_var ) )
				*this->m_var = std::ceil( *this->m_var );
			else
				*this->m_var = int( *this->m_var + 1 );
		}

		if ( g_io.key_state< io::key_state::RELEASED >( VK_LEFT ) ) {
			if ( *this->m_var != std::floor( *this->m_var ) )
				*this->m_var = std::floor( *this->m_var );
			else
				*this->m_var = int( *this->m_var - 1 );
		}
	}

	if ( *this->m_var != last_val ) {
		// clamp value
		if ( !uncapped )
			*this->m_var = std::clamp( *this->m_var, m_min, m_max );

		// format value
		if constexpr ( std::is_floating_point_v< T > )
			this->m_var_text = fmt::format( "{:.{}f}", *this->m_var, this->m_precision );
		else
			this->m_var_text = fmt::to_string( *this->m_var );
	}

	return m_dragging;
}

template< math::Number T >
void gui::objects::slider< T >::animate( ) {
	// animate value bar
	int goal_value_width = *this->m_var / static_cast< float >( m_max ) * this->m_dynamic_area.w;
	goal_value_width     = std::clamp( goal_value_width, 0, this->m_dynamic_area.w );
	utils::animate( this->m_val_width, goal_value_width, detail::anim::slider_lerp_speed );
}

template< math::Number T >
void gui::objects::slider< T >::reposition( const render::point& delta ) {
	base_object::reposition( delta );
	this->m_var_text_pos += delta;
}

template< math::Number T >
void gui::objects::slider< T >::resize( const render::size& delta ) {
	this->m_dynamic_area.w += delta.w;
	this->m_static_area.w += delta.w;
	this->m_var_text_pos.x += delta.w;
}