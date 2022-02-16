#include "popup.hpp"
#include "detail.hpp"

bool gui::helpers::popup::think( ) {
	if ( !m_open ) {
		return m_open = m_static_area.hovered( ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON );
	} else {
		if ( !m_popup_area.hovered( ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON ) ) {
			return m_open = false;
		}
	}

	return false;
}

void gui::helpers::popup::animate( ) {
	utils::animate( m_open_alpha, m_open ? 1.f : 0.f, m_open ? detail::anim::open_speed : detail::anim::close_speed );
}

void gui::helpers::popup::reposition( const render::point& delta ) {
	m_popup_area.add_pos( delta );
}

void gui::helpers::popup::resize( const render::size& delta ) {
	m_popup_area.add_size( delta );
}