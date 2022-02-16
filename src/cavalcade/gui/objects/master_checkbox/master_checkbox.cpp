#include "master_checkbox.hpp"
#include "../scrollbar/scrollbar.hpp"
#include "../column/column.hpp"

void gui::objects::master_checkbox::init( ) {
	checkbox::init( );
	// store the checkbox's areas
	m_checkbox_area.stt = m_static_area;
	m_checkbox_area.dyn = m_dynamic_area;

	// now make our own
	m_static_area.set_size( { m_parent->dyn_area( ).w, m_checkbox_area.stt.h } );

	m_dynamic_area.set_pos( { m_static_area.x, m_static_area.y + m_checkbox_area.stt.h } );
	m_dynamic_area.set_size( { m_parent->dyn_area( ).w, 0 } );

	m_cursor = m_dynamic_area.pos( );
}

void gui::objects::master_checkbox::render( ) const {
	{ // checkbox::render
		// background
		g_render.rectangle_filled( m_checkbox_area.dyn, style::palette::background ).stroke( style::palette::stroke );
		// inner rect
		g_render.rectangle_filled( m_checkbox_area.dyn.shrink( 1 ), style::palette::highlight.mod_alpha( m_alpha ) );

		g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );
	}

	if ( *m_var )
		m_children_mgr.render( m_dynamic_area );
}

bool gui::objects::master_checkbox::think( ) {
	bool active = m_checkbox_area.stt.hovered( ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON );

	if ( active ) {
		*m_var = !*m_var;
		on_interaction( );
	}

	if ( *m_var ) {
		auto backup    = m_dynamic_area;
		m_dynamic_area = m_parent->dyn_area( );
		if ( m_children_mgr.think( ) )
			active = true;
		m_dynamic_area = backup;
	}

	return active;
}

void gui::objects::master_checkbox::animate( ) {
	checkbox::animate( );
	m_children_mgr.animate( );
}

void gui::objects::master_checkbox::on_add_child( base_object* child ) {
	m_dynamic_area.h += m_cursor.y - m_dynamic_area.y2( );
}

void gui::objects::master_checkbox::reposition( const render::point& delta ) {
	base_parent::reposition( delta );
	m_checkbox_area.stt.add_pos( delta );
	m_checkbox_area.dyn.add_pos( delta );
}

void gui::objects::master_checkbox::resize( const render::size& delta ) {
	// only resize our children
	m_children_mgr.resize( delta );
}

void gui::objects::master_checkbox::on_interaction( ) {
	if ( !m_children_mgr.size( ) )
		return;

	{ // reposition everyone added after us

		// go down if we are active, go up if are not
		int reposition_delta = m_dynamic_area.h * ( *m_var ? 1 : -1 );

		// no point in looping if we are the last child
		if ( m_idx != m_parent->children( ).size( ) - 1 ) {
			for ( auto& child : m_parent->children( ) ) {
				if ( child->is< objects::column >( ) || child->is< objects::scrollbar >( ) )
					continue;

				if ( child->idx( ) > m_idx && !child->stt_area( ).pos( ).in_rect( m_static_area ) )
					child->reposition( { 0, reposition_delta } );
			}
		}

		m_parent->add_to_cursor( reposition_delta );
	}

	{ // now handle scrollbar stuff
		auto scroll = m_parent->scrollbar( );

		bool gone_past = m_parent->cursor( ).y > m_parent->dyn_area( ).y2( );
		if ( gone_past ) {
			if ( scroll ) {
				// it could've just been disabled by us, so let's make sure it's enabled
				scroll->enable( );
				// now update
				scroll->update_height( );
			} else {
				m_parent->create_scrollbar( );
			}
		} else {
			if ( scroll ) {
				// if this gets reached the scrollbar is guaranteed to have been created by us
				// that means we don't have to worry about updating it's height

				// reset the scrollbar to the beginning
				// @tekno thanks for making it work in a simple way
				scroll->set_scroll( 0.f );
				scroll->update_scroll( );

				// now disable it
				scroll->disable( );
			}
		}
	}
}