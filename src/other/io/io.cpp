#include "io.hpp"

bool io::io::init( ) {
	if ( !m_console.init( ) )
		return false;

	if ( !m_input.init( reinterpret_cast< LONG_PTR >( wnd_proc ) ) )
		return false;

	if ( !m_files.init( ) )
		return false;

	log( XOR( "initialized io" ) );

	return true;
}

void io::io::unload( ) {
	m_console.unload( );

	m_input.unload( );

	m_files.unload( );
}

std::filesystem::path& io::io::get_dir( dirs id ) {
	return m_files.m_directories[ id ];
}

BOOL io::io::think( UINT msg, WPARAM w_param, LPARAM l_param ) {
	return m_input.think( msg, w_param, l_param );
}

BOOL CALLBACK io::wnd_proc( HWND window, UINT msg, WPARAM w_param, LPARAM l_param ) {
	return g_io.think( msg, w_param, l_param );
}