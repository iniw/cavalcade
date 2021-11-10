#include "io.hpp"

#include "../../cavalcade/gui/gui.hpp"

bool io::impl::init( ) {
	MOCKING_TRY;

	MOCK m_console.init( );
	MOCK m_input.init( reinterpret_cast< LONG_PTR >( wnd_proc ) );
	MOCK m_files.init( );

	MOCKING_CATCH( return false );

	log( XOR( "initialized io" ) );

	return true;
}

void io::impl::unload( ) {
	m_input.unload( );
	m_files.unload( );
	m_console.unload( );
}

std::filesystem::path& io::impl::directory( dirs id ) {
	return m_files.m_directories.at( id );
}

const math::v2i& io::impl::mouse_pos( ) {
	return m_input.m_mouse.pos;
}

i32 io::impl::mouse_scroll( ) {
	return m_input.m_mouse.scroll_amt;
}

std::string_view io::impl::key_name( u8 key_id ) {
	return m_input.m_key_names[ key_id ];
}

BOOL io::impl::think( UINT msg, WPARAM w_param, LPARAM l_param ) {
	BOOL ret = m_input.think( msg, w_param, l_param );

	g_gui.think( );

	return ret;
}

BOOL CALLBACK io::wnd_proc( HWND window, UINT msg, WPARAM w_param, LPARAM l_param ) {
	return g_io.think( msg, w_param, l_param );
}