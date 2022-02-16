#include "../base.hpp"
#include "../../column/column.hpp"

void gui::objects::base_parent::animate( ) {
	base_object::animate( );
	m_children_mgr.animate( );
}

void gui::objects::base_parent::reposition( const render::point& delta ) {
	base_object::reposition( delta );
	m_cursor += delta;
	// also reposition our children
	m_children_mgr.reposition( delta );
}

void gui::objects::base_parent::resize( const render::size& delta ) {
	base_object::resize( delta );

	if ( m_parent ) {
		for ( auto& child : m_parent->children( ) ) {
			if ( child.get( ) == this )
				continue;

			// horizontal check
			if ( m_static_area.x < child->stt_area( ).x ) {
				auto diff = child->stt_area( ).x - m_static_area.x2( );
				if ( diff < style::padding::obj_spacing ) {
					child->reposition( { delta.w, 0 } );
				}
			}

			// vertical check
			if ( m_static_area.y < child->stt_area( ).y ) {
				auto diff = child->stt_area( ).y - m_static_area.y2( );
				if ( diff < style::padding::obj_spacing ) {
					child->reposition( { 0, delta.h } );
				}
			}
		}
	}

	// also resize our children
	m_children_mgr.resize( delta );
}

void gui::objects::base_parent::on_add_child( base_object* child ) {
	bool gone_past = child->stt_area( ).y2( ) > m_dynamic_area.y2( );
	if ( !gone_past )
		return;

	{
		// find our column if we have one
		objects::column* col = nullptr;
		for ( auto& child : m_children_mgr ) {
			if ( child->is< objects::column >( ) ) {
				col = child->as< column >( );
				break;
			}
		}

		if ( col ) {
			auto col_scroll = col->scrollbar( );
			if ( col_scroll ) {
				// if the column already has a scrollbar, we wanna check if our elements
				// take up more space than their's
				// if so, disable their scrollbar and use ours instead
				if ( m_cursor.y > col->cursor( ).y )
					col_scroll->disable( );
				else
					return;
			}
		}
	}

	if ( m_scrollbar ) {
		// enable it again if it already existed
		m_scrollbar->enable( );
		// now update
		m_scrollbar->update_height( );
	} else {
		// create it if it doesn't exist
		create_scrollbar( );
	}
}

void gui::objects::base_parent::create_scrollbar( ) {
	auto scroll = std::make_unique< objects::scrollbar >( );

	m_scrollbar = scroll.get( );

	m_scrollbar->identify( this );

	m_scrollbar->init( );

	// scrollbar gets added to the front instead of the back
	m_children_mgr.list( ).push_front( std::move( scroll ) );
}

void gui::objects::base_parent::render_debug_bounds( ) const {
	base_object::render_debug_bounds( );
	g_render.m_safe.draw_shape_front< render::geometry::rect >( render::rect{ m_cursor.x, m_cursor.y, 2, 2 }, style::palette::debug_bounds_other );
}

void gui::objects::base_parent::push_cursor( int offset ) {
	m_cursor.y += offset + style::padding::obj_spacing;
}

void gui::objects::base_parent::add_to_cursor( const render::size& offset ) {
	m_cursor += offset;
}

void gui::objects::base_parent::add_to_cursor( int offset ) {
	m_cursor.y += offset;
}