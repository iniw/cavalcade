#include "io.hpp"

#include "../../cavalcade/gui/gui.hpp"

bool io::impl::init( ) {
	MOCKING_TRY;
	MOCK m_input.init( reinterpret_cast< LONG_PTR >( wnd_proc ) );

#if _DEBUG
	MOCK m_console.init( );
	MOCK m_files.init( );
#endif

	MOCKING_CATCH( return false );

	log( XOR( "initialized io" ) );
	return true;
}

void io::impl::unload( ) {
#if _DEBUG
	m_input.unload( );
	m_files.unload( );
	m_console.unload( );
#endif
}

std::filesystem::path& io::impl::directory( dirs id ) {
	return m_files.m_directories.at( id );
}

render::point io::impl::mouse_pos( ) {
	return { m_input.m_mouse.pos[ X ], m_input.m_mouse.pos[ Y ] };
}

render::point io::impl::mouse_delta( ) {
	return { m_input.m_mouse.delta[ X ], m_input.m_mouse.delta[ Y ] };
}

i32 io::impl::mouse_scroll( ) {
	return m_input.m_mouse.scroll_amt;
}

void io::impl::update_input( ) {
	auto& mouse_info = m_input.m_mouse;

	static auto last_mouse_pos = mouse_info.pos;

	mouse_info.delta = mouse_info.pos - last_mouse_pos;
	last_mouse_pos   = mouse_info.pos;

	mouse_info.scroll_amt    = mouse_info.scroll_buffer;
	mouse_info.scroll_buffer = 0;
}

std::string_view io::impl::key_name( u8 key_id ) {
	return m_input.m_key_names[ key_id ];
}

BOOL io::impl::think( UINT msg, WPARAM w_param, LPARAM l_param ) {
	BOOL ret = m_input.think( msg, w_param, l_param );

	return ret;
}

BOOL CALLBACK io::wnd_proc( HWND window, UINT msg, WPARAM w_param, LPARAM l_param ) {
	return g_io.think( msg, w_param, l_param );
}