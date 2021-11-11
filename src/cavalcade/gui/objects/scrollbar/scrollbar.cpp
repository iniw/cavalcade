#include "scrollbar.hpp"

void gui::objects::scrollbar::identify( const parent_raw_ptr parent ) {
	m_parent = parent;
}

void gui::objects::scrollbar::init( ) {
	m_static_area[ Y ]     = m_parent->m_dynamic_area[ Y ];
	m_static_area[ X ]     = m_parent->m_static_area[ X ] + m_parent->m_static_area[ WIDTH ] - general::padding::margin;
	m_static_area[ WIDTH ] = general::padding::margin;

	update_height( );
}

void gui::objects::scrollbar::render( ) const {
	auto color = m_flags.test( flags::HOVERED ) || m_dragging ? general::pallete::highlight : general::pallete::secondary;
	g_render.rectangle_filled( m_static_area, color );
}

bool gui::objects::scrollbar::think( ) {
	auto& mouse_pos   = g_io.mouse_pos( );
	auto mouse_scroll = g_io.mouse_scroll( );

	auto mouse_delta = mouse_pos - m_previous_mouse_pos;

	if ( m_dragging || mouse_pos.in_rect( m_static_area ) )
		m_dragging = g_io.key_state( VK_LBUTTON );

	if ( m_dragging && !mouse_delta.empty( ) ) {
		math::v2i our_maxs{ m_static_area[ Y ], m_static_area[ Y ] + m_static_area[ HEIGHT ] };
		math::v2i parent_maxs{ m_parent->m_dynamic_area[ Y ], m_parent->m_dynamic_area[ Y ] + m_parent->m_dynamic_area[ HEIGHT ] };
		math::v2i deltas = our_maxs - parent_maxs;

		auto ratio = static_cast< f32 >( m_parent->m_dynamic_area[ HEIGHT ] ) / static_cast< f32 >( m_static_area[ HEIGHT ] );

		auto bar_delta = std::clamp( mouse_delta[ Y ], -deltas[ 0 ], -deltas[ 1 ] );
		auto mod_delta = static_cast< f32 >( -bar_delta ) * ratio;

		for ( auto& child : m_parent->m_children ) {
			if ( child.get( ) == this ) {
				reposition( { 0, bar_delta } );
				continue;
			}
			child->reposition( { 0, mod_delta } );
		}
	}

	m_previous_mouse_pos = mouse_pos;

	return m_dragging;
}

void gui::objects::scrollbar::update_height( ) {
	auto cursor_y = m_parent->get_cursor( )[ Y ];
	// the total height of all elements
	auto total_height = cursor_y - m_parent->m_dynamic_area[ Y ];
	// the ratio of how much smaller the parent's height is compared to the total height
	auto ratio = static_cast< f32 >( m_parent->m_dynamic_area[ HEIGHT ] ) / static_cast< f32 >( total_height );
	// our height should be the parent's height x that ratio
	m_static_area[ HEIGHT ] = static_cast< f32 >( m_parent->m_dynamic_area[ HEIGHT ] ) * ratio;
}