#include "window.hpp"
#include "detail.hpp"

gui::objects::window::window( std::string_view label, const render::size& size ) : base_object::base_object( label ) {
	m_size     = size;
	m_dragging = false;

	// NOTE(wini): a bit of a hack but required since
	// this is the only object that gets created manually
	identify( nullptr );

	auto center = g_render.get_screen_center( ) - m_size / 2;

	// center ourselves
	m_static_area.set_pos( center );
	m_static_area.set_size( m_size );

	m_dynamic_area = m_static_area.shrink( style::padding::margin );
	m_dynamic_area.vertical_cut( m_label_size.h );

	m_cursor = m_dynamic_area.pos( );

	// the drag area is basically the header of the window
	m_reposition_area   = m_static_area;
	m_reposition_area.h = m_label_size.h + m_label_size.h / 2;

	m_label_pos = utils::center_label( m_label, m_reposition_area );

	m_resize_area.set_pos( m_static_area.max( ) - detail::sizing::resize_area );
	m_resize_area.set_size( detail::sizing::resize_area );
}

void gui::objects::window::render( ) const {
	if ( m_flags.test( flags::DISABLED ) )
		return;

	g_render.rectangle_filled( m_static_area, style::palette::primary ).outline( style::palette::stroke );

	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );

	m_children_mgr.render( m_dynamic_area );
}

bool gui::objects::window::think( ) {
	if ( m_flags.test( flags::DISABLED ) )
		return false;

	render::point mouse_pos = g_io.mouse_pos( );

	// return if one of our kids had activity, before handling dragging
	if ( m_flags.test( flags::ACTIVE ) ) {
		m_previous_mouse_pos = mouse_pos;
		return false;
	}

	if ( m_dragging || m_reposition_area.hovered( ) )
		m_dragging = g_io.key_state( VK_LBUTTON );

	if ( m_dragging ) {
		auto delta = mouse_pos - m_previous_mouse_pos;
		if ( !delta.is_zero( ) )
			reposition( delta );
	}

	if ( m_resize_area.hovered( ) && g_io.key_state( VK_LBUTTON ) ) {
		auto delta = mouse_pos - m_previous_mouse_pos;
		if ( !delta.is_zero( ) )
			resize( { delta.x, delta.y } );
	}

	m_previous_mouse_pos = mouse_pos;

	return true;
}

void gui::objects::window::reposition( const render::point& delta ) {
	base_parent::reposition( delta );
	m_reposition_area.add_pos( delta );
	m_resize_area.add_pos( delta );
}

void gui::objects::window::resize( const render::size& delta ) {
	auto size = m_static_area.size( );
	// don't go below our initial size
	if ( ( size.w + delta.w ) <= m_size.w || ( size.h + delta.h ) <= m_size.h )
		return;

	base_parent::resize( delta );
	m_reposition_area.w += delta.w;
	m_label_pos = utils::center_label( m_label, m_reposition_area );
	// resize area should maitain it's size
	m_resize_area.add_pos( { delta.w, delta.h } );
}

void gui::objects::window::update_flags( ) {
	m_flags.set( flags::HOVERED, m_static_area.hovered( ) );

	m_flags.set( flags::ACTIVE, m_children_mgr.think( ) );

	if ( g_io.key_state< io::key_state::RELEASED >( VK_INSERT ) )
		m_flags.flip( flags::DISABLED );
}

void gui::objects::window::render_debug_bounds( ) const {
	base_parent::render_debug_bounds( );
	g_render.m_safe.draw_shape_front< render::geometry::rect >( m_reposition_area, style::palette::debug_bounds_other );
	g_render.m_safe.draw_shape_front< render::geometry::rect >( m_resize_area, style::palette::debug_bounds_other );
}