#include "../hooks.hpp"

void cavalcade::hooks::chlc_client::frame_stage_notify( unk ecx, unk, sdk::frame_stage stage ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< frame_stage_notify_fn >( HASH_CT( "CHLClient::FrameStageNotify" ) );
	og( ecx, stage );

	g_render.m_safe.frame( [ & ]( ) {
		// std::unique_lock lock( g_lua.m_mutex );

		for ( auto& [ state, callbacks ] : g_lua.m_callbacks ) {
			state[ "g_FrameStage" ] = ( i32 )stage;
			for ( const auto& callback : callbacks[ XOR( "FrameStageNotify" ) ] ) {
				callback( );
			}
		}

		if ( !g_csgo.m_engine->is_in_game( ) )
			return;

		if ( !g_ctx.m_local )
			return;

		g_hack.m_esp.run( );
		g_hack.m_fog.run( );

		if ( !g_ctx.m_in_deathcam && g_ctx.m_local.get( ).is_alive( ) ) {
			g_hack.m_velgraph.draw( );
			g_hack.m_indscreen.draw( );
			g_hack.m_hitmarker.draw( );
			g_hack.m_movement.pixelsurf_calculator( );
		}
	} );
}

void cavalcade::hooks::chlc_client::level_init_pre_entity( const char* name ) {
	// std::unique_lock lock( g_lua.m_mutex );

	// NOTE(para): it's particularly important for this to be here, before og
	g_hack.m_nightmode.clear( );
	g_hack.m_fog.reset( );

	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_pre_entity_fn >( HASH_CT( "CHLClient::LevelInitPreEntity" ) );
	og( name );

	g_csgo.m_map_name = std::string( name );
	g_csgo.m_sky_name = g_ctx.m_cvars.sv_skyname->get_string( );

	for ( const auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		for ( const auto& callback : callbacks.at( XOR( "LevelInitPreEntity" ) ) ) {
			callback( );
		}
	}
}

void cavalcade::hooks::chlc_client::level_init_post_entity( ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_post_entity_fn >( HASH_CT( "CHLClient::LevelInitPostEntity" ) );
	og( );
	for ( const auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		for ( const auto& callback : callbacks.at( XOR( "LevelInitPostEntity" ) ) ) {
			callback( );
		}
	}
}

void cavalcade::hooks::chlc_client::level_shutdown( unk ecx, unk edx ) {
	g_csgo.m_map_name = std::nullopt;
	g_csgo.m_sky_name = std::nullopt;
	g_ctx.m_cmd       = nullptr;
	g_hack.m_translator.m_pending_translations.clear( );
	g_ctx.m_in_deathcam = false;
	g_entity_cacher.clear( );
	g_hack.m_trainer.clear( );
	g_hack.m_esp.clear( );
	g_hack.m_velgraph.clear( );
	g_hack.m_indscreen.clear( );
	g_hack.m_hitmarker.clear( );
	g_hack.m_movement.clear( );

	for ( auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		state.set( XOR( "g_Cmd" ), sol::lua_nil );
		for ( const auto& callback : callbacks[ XOR( "LevelShutdown" ) ] ) {
			callback( );
		}
	}

	static auto og = g_mem[ CLIENT_DLL ].get_og< level_shutdown_fn >( HASH_CT( "CHLClient::LevelShutdown" ) );
	og( ecx, edx );
}

void cavalcade::hooks::chlc_client::on_override_mouse_input( unk ecx, unk edx, i32 slot, f32& x, f32& y ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< on_override_mouse_input_fn >( HASH_CT( "OnOverrideMouseInput" ) );

	og( ecx, edx, slot, std::ref( x ), std::ref( y ) );

	g_hack.m_aimbot.run( x, y );
	g_hack.m_movement.edgebug_scale_mouse( x );
}