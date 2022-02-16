#include "column.hpp"
#include "../scrollbar/scrollbar.hpp"

// TODO(wini): support for more than 2 columns
void gui::objects::column::init( ) {
	auto& parent_dyn_area = m_parent->dyn_area( );

	// cut our parent's area in half and add a bit of padding
	parent_dyn_area.w /= 2;
	parent_dyn_area.w -= style::padding::obj_spacing / 2;

	// copy the area, offset it and account for padding
	m_dynamic_area = parent_dyn_area;
	m_dynamic_area.x += parent_dyn_area.w + style::padding::obj_spacing;
	// m_dynamic_area.w += style::padding::obj_spacing / 2;

	// we never use this but not having one causes issues
	m_static_area = m_dynamic_area;
	m_static_area.w += style::padding::margin;

	m_cursor = m_dynamic_area.pos( );

	// handle resizing of children that already exist
	for ( auto& child : m_parent->children( ) ) {
		if ( child->is< objects::scrollbar >( ) )
			continue;

		child->resize( { -m_dynamic_area.w - style::padding::obj_spacing, 0 } );
	}
}

void gui::objects::column::render( ) const {
	// HACK: i do this here to fix the rendering of our scrollbar (if we have one)
	// the issue is that since our parent's dynamic area gets cut in half by us
	// their clip rect doesn't reach our scrollbar, so when we pop OUR clip rect
	// before rendering the scrollbar, the clip rect that takes it's place
	// doesn't include the scrollbar (because we cut it in half)
	// so i basically just temporarily override the parent's clip_rect.
	if ( m_scrollbar )
		g_render.push_clip_rect( render::rect( { 0, 0 }, g_render.get_screen_size( ) ) );

	m_children_mgr.render( m_static_area, false );

	if ( m_scrollbar )
		g_render.pop_clip_rect( );
}

bool gui::objects::column::think( ) {
	return m_children_mgr.think( );
}

void gui::objects::column::on_add_child( base_object* child ) {
	bool gone_past = child->stt_area( ).y2( ) > m_dynamic_area.y2( );
	if ( !gone_past )
		return;

	{
		auto parent_scroll = m_parent->scrollbar( );
		if ( parent_scroll ) {
			// if our parent already has a scrollbar, we wanna check if our elements
			// take up more space than their's
			// if so, we disable their scrollbar and use ours instead
			if ( m_cursor.y > m_parent->cursor( ).y )
				parent_scroll->disable( );
			else
				return;
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