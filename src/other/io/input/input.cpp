#include "input.hpp"

#include "../../math/types/vector.hpp"

bool io::input::init( LONG_PTR wnd_proc ) {
	s_og = reinterpret_cast< WNDPROC >( LI_FN( SetWindowLongA )( cavalcade::window, GWLP_WNDPROC, wnd_proc ) );

	// key names
	char buffer[ 50 ];
	for ( i32 i = 1; i < 255; i++ ) {
		u32 scan_code = MapVirtualKey( i, MAPVK_VK_TO_VSC );
		u32 l_param   = scan_code << 16;

		u32 size = GetKeyNameText( l_param, buffer, 50 );

		if ( size )
			utils::str::to_lower( m_key_names[ i ].assign( &buffer[ 0 ], size ) );
	}

	return s_og;
}

void io::input::unload( ) {
	if ( s_og )
		LI_FN( SetWindowLongA )( cavalcade::window, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( s_og ) );
}

BOOL io::input::think( UINT msg, WPARAM w_param, LPARAM l_param ) {
	u8 key_id           = 0;
	key_state key_state = key_state::UP;

	m_mouse.scroll_amt = 0;

	switch ( msg ) {
	case WM_KEYDOWN:
		key_id    = w_param;
		key_state = key_state::DOWN;
		break;
	case WM_KEYUP:
		key_id    = w_param;
		key_state = key_state::UP;
		break;
	case WM_LBUTTONDOWN:
		key_id    = VK_LBUTTON;
		key_state = key_state::DOWN;
		break;
	case WM_LBUTTONUP:
		key_id    = VK_LBUTTON;
		key_state = key_state::UP;
		break;
	case WM_LBUTTONDBLCLK:
		key_id    = VK_LBUTTON;
		key_state = key_state::DOWN;
		break;
	case WM_RBUTTONDOWN:
		key_id    = VK_RBUTTON;
		key_state = key_state::DOWN;
		break;
	case WM_RBUTTONUP:
		key_id    = VK_RBUTTON;
		key_state = key_state::UP;
		break;
	case WM_RBUTTONDBLCLK:
		key_id    = VK_RBUTTON;
		key_state = key_state::DOWN;
		break;
	case WM_MBUTTONDOWN:
		key_id    = VK_MBUTTON;
		key_state = key_state::DOWN;
		break;
	case WM_MBUTTONUP:
		key_id    = VK_MBUTTON;
		key_state = key_state::UP;
		break;
	case WM_MBUTTONDBLCLK:
		key_id    = VK_MBUTTON;
		key_state = key_state::DOWN;
		break;
	case WM_XBUTTONDOWN:
		key_id    = GET_XBUTTON_WPARAM( w_param ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
		key_state = key_state::DOWN;
		break;
	case WM_XBUTTONUP:
		key_id    = GET_XBUTTON_WPARAM( w_param ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
		key_state = key_state::UP;
		break;
	case WM_XBUTTONDBLCLK:
		key_id    = GET_XBUTTON_WPARAM( w_param ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
		key_state = key_state::DOWN;
		break;
	case WM_MOUSEMOVE:
		m_mouse.pos = math::v2i( GET_X_LPARAM( l_param ), GET_Y_LPARAM( l_param ) );
		break;
	case WM_MOUSEWHEEL:
		m_mouse.scroll_amt = -( GET_WHEEL_DELTA_WPARAM( w_param ) / ( WHEEL_DELTA / 4 ) );
		break;
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
		for ( auto& key : m_keys )
			key.m_state = key_state::UP;
		break;
	default:
		break;
	}

	u64 time = LI_FN( GetTickCount64 )( );

	if ( key_id ) {
		if ( key_state == key_state::UP && m_keys[ key_id ].m_state == key_state::DOWN )
			m_keys[ key_id ] = { key_state::RELEASED, time };
		else
			m_keys[ key_id ] = { key_state, time };
	}

	return LI_FN( CallWindowProcA )( s_og, cavalcade::window, msg, w_param, l_param );
}