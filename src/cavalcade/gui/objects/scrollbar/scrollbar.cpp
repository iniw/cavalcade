#include "scrollbar.hpp"

void gui::objects::scrollbar::identify( const parent_ptr& parent ) {
	m_parent = parent;
}

void gui::objects::scrollbar::init( ) {
	m_static_area[ Y ]     = m_parent->m_static_area[ Y ];
	m_static_area[ X ]     = m_parent->m_static_area[ X ] + m_parent->m_static_area[ WIDTH ] - general::padding::margin;
	m_static_area[ WIDTH ] = general::padding::margin;

	update_height( );
}

void gui::objects::scrollbar::render( ) {
	auto color = m_flags.test( flags::HOVERED ) ? general::pallete::highlight : general::pallete::secondary;
	g_render.rectangle_filled( m_static_area, general::pallete::secondary );
}

bool gui::objects::scrollbar::think( ) {
	auto mouse_pos    = g_io.mouse_pos( );
	auto mouse_scroll = g_io.mouse_scroll( );

	auto mouse_delta = mouse_pos - m_previous_mouse_pos;

	if ( m_dragging || mouse_pos.in_rect( m_static_area ) && g_io.key_state( VK_LBUTTON ) )
		m_dragging = g_io.key_state( VK_LBUTTON );

	if ( m_dragging && !mouse_delta.empty( ) ) {
		math::v2i our_maxs{ m_static_area[ Y ], m_static_area[ Y ] + m_static_area[ HEIGHT ] };
		math::v2i parent_maxs{ m_parent->m_static_area[ Y ], m_parent->m_static_area[ Y ] + m_parent->m_static_area[ HEIGHT ] };

		math::v2i deltas = our_maxs - parent_maxs;

		// limit the amount we can move the scrollbar
		// i don't really know if this is the right way to do it
		auto clamped_delta = std::clamp( mouse_delta[ Y ], -deltas[ 0 ], -deltas[ 1 ] );

		for ( auto& child : m_parent->m_children ) {
			if ( child == get( ) ) {
				// we move in the direction of the mouse
				child->reposition( { 0, clamped_delta } );
				continue;
			}

			// the children move in the opposite way
			child->reposition( { 0, -clamped_delta } );
		}
	}

	m_previous_mouse_pos = mouse_pos;

	return m_dragging;
}

// this is all compeltely wrgong. am i to dumb? what have i done. where have i failed. my life..
void gui::objects::scrollbar::update_height( ) {
	auto cursor = m_parent->get_cursor( )[ Y ];

	f32 ratio = static_cast< f32 >( m_parent->m_static_area[ HEIGHT ] ) / static_cast< f32 >( cursor );

	m_static_area[ HEIGHT ] = static_cast< f32 >( m_parent->m_static_area[ HEIGHT ] ) * ratio;

	g_io.log( "cursor: {} | ratio: {} | height: {}", cursor, ratio, m_static_area[ HEIGHT ] );
}