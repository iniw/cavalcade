#include "csgo.hpp"

bool sdk::csgo::init( ) {
	{ // interfaces
		m_client_dll = find_interface< interfaces::client_dll* >( g_mem[ CLIENT_DLL ], HASH_CT( "VClient" ) );
		if ( !m_client_dll )
			return false;

		m_engine = find_interface< interfaces::engine* >( g_mem[ ENGINE_DLL ], HASH_CT( "VEngineClient" ) );
		if ( !m_engine )
			return false;

		m_cvars = find_interface< interfaces::cvars* >( g_mem[ VSTDLIB_DLL ], HASH_CT( "VEngineCvar" ) );
		if ( !m_cvars )
			return false;

		m_ent_list = find_interface< interfaces::ent_list* >( g_mem[ CLIENT_DLL ], HASH_CT( "VClientEntityList" ) );
		if ( !m_ent_list )
			return false;

		m_mdl_cache = find_interface< interfaces::mdl_cache* >( g_mem[ CLIENT_DLL ], HASH_CT( "MDLCache" ) );
		if ( !m_mdl_cache )
			return false;

		m_game_movement = find_interface< interfaces::game_movement* >( g_mem[ CLIENT_DLL ], HASH_CT( "GameMovement" ) );
		if ( !m_game_movement )
			return false;
	}

	m_globals = mem::get_v_func( m_client_dll, 11 ).add( 0xA ).get< globals* >( 2 );
	if ( !m_globals )
		return false;

	m_move_helper = g_mem[ CLIENT_DLL ].find_pattern( STB( "8B 0D ? ? ? ? 8B 46 08 68" ) ).add( 0x2 ).get< move_helper* >( 2 );
	if ( !m_move_helper )
		return false;

	m_client_state = mem::get_v_func( m_engine, 12 ).add( 0x10 ).get< client_state* >( 2 );
	if ( !m_client_state )
		return false;

	g_io.log( XOR( "initialized csgo" ) );

	return true;
}