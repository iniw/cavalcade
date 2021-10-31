#include "csgo.hpp"

bool sdk::csgo::init( ) {
	{ // interfaces
		m_client_dll = find_interface< interfaces::client_dll* >( g_mem[ HASH_CT( "client.dll" ) ], HASH_CT( "VClient" ) );
		if ( !m_client_dll )
			return false;

		m_engine = find_interface< interfaces::engine* >( g_mem[ HASH_CT( "engine.dll" ) ], HASH_CT( "VEngineClient" ) );
		if ( !m_engine )
			return false;

		m_cvars = find_interface< interfaces::cvars* >( g_mem[ HASH_CT( "vstdlib.dll" ) ], HASH_CT( "VEngineCvar" ) );
		if ( !m_cvars )
			return false;

		m_ent_list = find_interface< interfaces::ent_list* >( g_mem[ HASH_CT( "client.dll" ) ], HASH_CT( "VClientEntityList" ) );
		if ( !m_ent_list )
			return false;
	}
	m_globals = mem::get_v_func( m_client_dll, 11 ).add( 0xA ).get< globals* >( 2 );
	if ( !m_globals )
		return false;

	g_io.log( XOR( "initialized csgo" ) );

	return true;
}