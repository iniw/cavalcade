#include "scrollbar.hpp"
#include "detail.hpp"
#include "../column/column.hpp"
#include "../../../ctx/ctx.hpp"

void gui::objects::scrollbar::init( ) {
	m_static_area.set_pos( { m_parent->stt_area( ).x2( ) - style::padding::margin, m_parent->dyn_area( ).y } );
	m_static_area.set_size( { style::padding::margin, m_parent->dyn_area( ).h } );

	m_dynamic_area.set_pos( { m_parent->stt_area( ).x2( ) - detail::sizing::main, m_parent->dyn_area( ).y } );
	m_dynamic_area.set_size( { detail::sizing::main, 0 } );

	m_bar_width = detail::sizing::main;

	update_height( );
}

void gui::objects::scrollbar::render( ) const {
	g_render.rectangle_filled( m_dynamic_area, style::palette::highlight );
}

bool gui::objects::scrollbar::think( ) {
	bool active = false;

	auto move_scrollbar_to = [ & ]( int new_scrollbar_pos ) {
		m_drag_scroll_goal = new_scrollbar_pos / m_scrollbar_ratio;
		m_drag_scroll_goal = std::clamp( m_drag_scroll_goal, 0.f, static_cast< float >( m_max_scroll ) ); // limit scroll amount. prevent overscroll.
		m_drag_scrolling   = true;
	};

	if ( g_io.key_state( VK_LBUTTON ) ) {
		if ( !m_dragging ) {
			if ( m_dynamic_area.hovered( ) ) {
				// store the fact that we've clicked the scrollbar
				m_clicking = true;
			}

			if ( m_clicking ) {
				// clicked the scrollbar, now waiting for mouse movement to start dragging
				if ( !g_io.mouse_delta( ).is_zero( ) ) {
					// start dragging when we've clicked the scrollbar and start moving the mouse
					m_dragging = true;

					m_start_drag_y             = g_io.mouse_pos( ).y;
					m_start_drag_scrollbar_pos = m_scrollbar_pos;
				}
			} else if ( m_static_area.hovered( ) ) {
				// scroll to clicked point when clicking outside the scrollbar
				int delta = g_io.mouse_pos( ).y - m_static_area.y;
				move_scrollbar_to( delta - m_dynamic_area.h / 2 );
			}
		}
	} else {
		m_clicking = false;
		m_dragging = false;
	}

	if ( m_dragging ) {
		// dragging scrollbar
		int delta = g_io.mouse_pos( ).y - m_start_drag_y;
		move_scrollbar_to( m_start_drag_scrollbar_pos + delta );

		m_scroll_speed = 0.f;

		active = true;
	} else if ( g_io.mouse_scroll( ) && m_parent->flags( ).test( flags::HOVERED ) ) {
		// scrolling with scroll wheel
		m_scroll_speed += g_io.mouse_scroll( ) * detail::options::amount;
		m_drag_scrolling = false;

		active = true;
	}

	// we don't update_scroll here because we'll do it in animate()

	return active || m_dragging || m_clicking || m_drag_scrolling;
}

void gui::objects::scrollbar::animate( ) {
	// expand the scrollbar on hover
	utils::animate( m_bar_width, ( m_static_area.hovered( ) || m_dragging ) ? detail::sizing::main_hovered : detail::sizing::main,
	                detail::anim::bar_width );

	int bar_width    = round( m_bar_width );
	m_dynamic_area.x = m_parent->stt_area( ).x2( ) - bar_width;
	m_dynamic_area.w = bar_width;

	// do scrolling
	update_scroll( );
}

void gui::objects::scrollbar::update_flags( ) {
	base_object::update_flags( );
	// ouverride hovered
	m_flags.set( flags::HOVERED, m_dynamic_area.hovered( ) );
}

bool gui::objects::scrollbar::update_scroll( ) {
	if ( m_drag_scrolling )
		utils::animate( m_scroll, m_drag_scroll_goal, detail::anim::drag_speed );

	if ( !m_dragging ) {
		// handle overscroll
		float top    = m_scroll;
		float bottom = m_scroll + m_visible_height;
		if ( top < 0 ) {
			// smoothly scroll to the top
			utils::animate( m_scroll, 0.f, detail::anim::overscroll_reset_speed );
			m_scroll = std::min( m_scroll, 0.f ); // safety
		} else if ( bottom > m_total_height ) {
			// smoothly scroll to the bottom
			utils::animate( m_scroll, m_max_scroll, detail::anim::overscroll_reset_speed );
			m_scroll = std::max( m_scroll, ( float )( m_max_scroll ) ); // safety
		}

		// dampen scrolling when about to overscroll
		float next_top    = m_scroll + m_scroll_speed * g_csgo.m_globals->m_frametime;
		float next_bottom = m_scroll + m_scroll_speed * g_csgo.m_globals->m_frametime + m_visible_height;
		if ( next_top < 0 || next_bottom > m_total_height )
			m_scroll_speed *= detail::options::overscroll_multiplier;

		// smooth scroll
		m_scroll += m_scroll_speed * g_csgo.m_globals->m_frametime;
		utils::animate( m_scroll_speed, 0.f, detail::anim::slowdown );
	}

	float scroll_percent = m_scroll / m_max_scroll;
	int scrollbar_pos    = scroll_percent * ( m_static_area.h - m_dynamic_area.h );
	scrollbar_pos        = std::clamp( scrollbar_pos, 0, m_static_area.h - m_dynamic_area.h ); // stop overscroll from affecting scrollbar

	// move the scrollbar
	if ( scrollbar_pos != m_scrollbar_pos ) {
		int scrollbar_move = scrollbar_pos - m_scrollbar_pos;
		m_scrollbar_pos    = scrollbar_pos;
		m_dynamic_area.add_pos( { 0, scrollbar_move } );
	}

	// now do the actual scrolling
	int scroll_pos = m_scroll;
	if ( scroll_pos != m_scroll_pos ) {
		// inveresed scroll, don't want y to go up since then everything would move up. invert it.
		render::point children_scroll = { 0, -( scroll_pos - m_scroll_pos ) };
		m_scroll_pos                  = scroll_pos;

		managers::children* children = nullptr;
		// if our parent is a column we need to instead iterate our grandparent's children
		if ( m_parent->is< objects::column >( ) )
			children = &m_parent->parent( )->children( );
		else
			children = &m_parent->children( );

		// move children
		for ( auto& child : *children ) {
			// don't move ourselves, we've already done that
			if ( child->id( ) == m_id )
				continue;

			// columns need to be handled in a special way
			// their children should be scrolled, but the column itself shouldn't
			if ( child->is< objects::column >( ) ) {
				auto col = child->as< column >( );
				for ( auto& col_child : col->children( ) ) {
					// don't move ourselves
					if ( col_child->id( ) == m_id )
						continue;

					col_child->reposition( children_scroll );
				}
			} else {
				// it's a normal child
				child->reposition( children_scroll );
			}
		}

		return true;
	} else {
		m_drag_scrolling = false;
	}

	return false;
}

void gui::objects::scrollbar::update_height( ) {
	m_visible_height = m_parent->dyn_area( ).h;
	m_total_height   = m_parent->cursor( ).y - m_parent->dyn_area( ).y;

	// the ratio of how much smaller the parent's height is compared to the total height
	float parent_ratio = m_visible_height / static_cast< float >( m_total_height );

	// our height should be the parent's height * that ratio
	m_dynamic_area.h = m_visible_height * parent_ratio;

	// store the maximum amount we can scroll
	m_max_scroll = m_total_height - m_visible_height;

	// store the ratio of how much we should move the scrollbar compared to the objects
	m_scrollbar_ratio = m_dynamic_area.h / static_cast< float >( m_visible_height );
}

void gui::objects::scrollbar::resize( const render::size& delta ) {
	base_object::reposition( { delta.w, delta.h } );
}