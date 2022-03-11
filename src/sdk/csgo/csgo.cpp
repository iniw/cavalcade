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

		m_model_info = find_interface< interfaces::model_info* >( g_mem[ ENGINE_DLL ], HASH_CT( "VModelInfoClient" ) );
		if ( !m_model_info )
			return false;

		m_input = g_mem[ CLIENT_DLL ].find_pattern( STB( "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) ).add( 0x1 ).get< interfaces::input* >( 1 );
		if ( !m_input )
			return false;

		m_debug_overlay = find_interface< interfaces::debug_overlay* >( g_mem[ ENGINE_DLL ], HASH_CT( "VDebugOverlay" ) );
		if ( !m_debug_overlay )
			return false;

		m_engine_trace = find_interface< interfaces::engine_trace* >( g_mem[ ENGINE_DLL ], HASH_CT( "EngineTraceClient" ) );
		if ( !m_engine_trace )
			return false;

		m_physics_surface_props = find_interface< interfaces::physics_surface_props* >( g_mem[ PHYSICS_DLL ], HASH_CT( "VPhysicsSurfaceProps" ) );
		if ( !m_physics_surface_props )
			return false;

		m_engine_sound = find_interface< interfaces::engine_sound* >( g_mem[ ENGINE_DLL ], HASH_CT( "IEngineSoundClient" ) );
		if ( !m_engine_sound )
			return false;

		m_effects = find_interface< interfaces::effects* >( g_mem[ ENGINE_DLL ], HASH_CT( "VEngineEffects" ) );
		if ( !m_effects )
			return false;

		m_material_system = find_interface< interfaces::material_system* >( g_mem[ MATERIALSYSTEM_DLL ], HASH_CT( "VMaterialSystem" ) );
		if ( !m_material_system )
			return false;

		m_panorama = g_mem[ PANORAMA_DLL ]
		                 .find_pattern( STB( "B9 ? ? ? ? 56 FF 50 40 8B F0 68 ? ? ? ? 56 8B 0E FF 91 ? ? ? ? 0F 10 05 ? ? ? ? 68 ? "
		                                     "? ? ? 56 0F 11 05 ? ? ? ? F3 0F 7E 05 ? ? ? ? 66 0F D6 05" ) )
		                 .add( 1 )
		                 .get< interfaces::panorama_ui_engine* >( 1 );
		if ( !m_panorama )
			return false;

		m_physics_collisions = find_interface< interfaces::physics_collison* >( g_mem[ PHYSICS_DLL ], HASH_CT( "VPhysicsCollision" ) );
		if ( !m_physics_collisions )
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

	// fun fact: you can get prediction and fsn from one scan!
	m_prediction =
		g_mem[ CLIENT_DLL ].find_pattern( STB( "A1 ? ? ? ? B9 ? ? ? ? FF 50 14 5F C6 05" ) ).add( 0x1 ).get< interfaces::prediction* >( 1 );
	if ( !m_prediction )
		return false;

	m_main_view_origin = g_mem[ ENGINE_DLL ].find_pattern( STB( "B9 ? ? ? ? 56 57 83 EC 08 F3 0F 58" ) ).add( 0x1 ).get< math::v3f* >( );
	if ( !m_main_view_origin )
		return false;

	g_io.log( XOR( "initialized csgo" ) );

	return true;
}