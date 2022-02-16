#include "../base.hpp"

void gui::objects::base_object::identify( base_parent* parent ) {
	m_abs_idx = s_object_count++;
	m_parent  = parent;

	if ( m_parent ) {
		// we can set idx to size here because we haven't been added to the list yet
		m_idx       = m_parent->children( ).size( );
		m_cfg_entry = m_parent->cfg_entry( ) + CFG_SEPARATOR;
	}

	m_cfg_entry += m_label;

	// set our id
	{
		if ( !m_label.empty( ) )
			m_id = HASH_RT( m_cfg_entry );

		m_id <<= 32;
		m_id |= m_abs_idx;
	}
}

void gui::objects::base_object::reposition( const render::point& delta ) {
	m_static_area.add_pos( delta );
	m_dynamic_area.add_pos( delta );
	m_label_pos += delta;
}

void gui::objects::base_object::resize( const render::size& delta ) {
	m_static_area.add_size( delta );
	m_dynamic_area.add_size( delta );
}

void gui::objects::base_object::update_flags( ) {
	m_flags.set( flags::ACTIVE, false );
	m_flags.set( flags::DISABLED, false );
	m_flags.set( flags::HOVERED, m_static_area.hovered( ) );
}

void gui::objects::base_object::render_debug_bounds( ) const {
	g_render.m_safe.draw_shape_front< render::geometry::rect >( m_static_area, style::palette::debug_bounds_stt );
	g_render.m_safe.draw_shape_front< render::geometry::rect >( m_dynamic_area, style::palette::debug_bounds_dyn );
}