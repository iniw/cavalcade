#include "csgo.hpp"

bool sdk::csgo::init( ) {
{ // interfaces
		m_client = find_interface< interfaces::client* >( g_mem[ CLIENT_DLL ], HASH_CT( "VClient" ) );
		if ( !m_client )
			return false;

		m_ent_list = find_interface< interfaces::ent_list* >( g_mem[ CLIENT_DLL ], HASH_CT( "VClientEntityList" ) );
		if ( !m_ent_list )
			return false;

		m_game_movement = find_interface< interfaces::game_movement* >( g_mem[ CLIENT_DLL ], HASH_CT( "GameMovement" ) );
		if ( !m_game_movement )
			return false;

		m_engine = find_interface< interfaces::engine* >( g_mem[ ENGINE_DLL ], HASH_CT( "VEngineClient" ) );
		if ( !m_engine )
			return false;

		m_cvars = find_interface< interfaces::cvars* >( g_mem[ VSTDLIB_DLL ], HASH_CT( "VEngineCvar" ) );
		if ( !m_cvars )
			return false;

		m_mdl_cache = find_interface< interfaces::mdl_cache* >( g_mem[ DATACACHE_DLL ], HASH_CT( "MDLCache" ) );
		if ( !m_mdl_cache )
			return false;
	}

	m_globals = mem::get_v_func( m_client, 11 ).add( 0xA ).get< globals* >( 2 );
	if ( !m_globals )
		return false;

	m_move_helper = g_mem[ CLIENT_DLL ].find_pattern( STB( "8B 0D ? ? ? ? 8B 46 08 68" ) ).add( 0x2 ).get< move_helper* >( 2 );
	if ( !m_move_helper )
		return false;

	m_client_state = g_mem[ ENGINE_DLL ].find_pattern( STB( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07 " ) ).add( 0x1 ).get< client_state* >( 2 );
	if ( !m_client_state )
		return false;

	m_client_mode_shared = mem::get_v_func( m_client, 10 ).add( 0x5 ).get< client_mode_shared* >( 2 );
	if ( !m_client_mode_shared )
		return false;

	m_d3d9_device = g_mem[ SHADERAPIDX9_DLL ].find_pattern( STB( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ).add( 0x1 ).get< IDirect3DDevice9* >( 2 );
	if ( !m_d3d9_device )
		return false;

	g_io.log( XOR( "initialized csgo" ) );

	return true;
}
