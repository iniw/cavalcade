#include "io.hpp"

#include "../../cavalcade/gui/gui.hpp"

bool io::io::init( ) {
	MOCKING_REGION( MOCK m_console.init( ); MOCK m_input.init( reinterpret_cast< LONG_PTR >( wnd_proc ) ); MOCK m_files.init( ); );

	log( XOR( "initialized io" ) );

	return true;
}

void io::io::unload( ) {
	m_input.unload( );
	m_files.unload( );
	m_console.unload( );
}

std::filesystem::path& io::io::directory( dirs id ) {
	return m_files.m_directories.at( id );
}

math::v2i io::io::mouse_pos( ) {
	return m_input.m_mouse.pos;
}

f32 io::io::mouse_scroll( ) {
	return m_input.m_mouse.scroll_amt;
}

BOOL io::io::think( UINT msg, WPARAM w_param, LPARAM l_param ) {
	BOOL ret = m_input.think( msg, w_param, l_param );

	g_gui.think( );

	return ret;
}

BOOL CALLBACK io::wnd_proc( HWND window, UINT msg, WPARAM w_param, LPARAM l_param ) {
	return g_io.think( msg, w_param, l_param );
}