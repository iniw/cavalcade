#include "../hooks.hpp"

void cavalcade::hooks::chlc_client::frame_stage_notify( unk ecx, unk, sdk::frame_stage stage ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< frame_stage_notify_fn >( HASH_CT( "CHLClient::FrameStageNotify" ) );
	og( ecx, stage );

	g_render.m_safe.frame( [ & ]( ) {
		for ( auto& [ state, callbacks ] : g_ctx.m_lua.m_callbacks ) {
			state[ "g_FrameStage" ] = ( i32 )stage;
			for ( const auto& callback : callbacks[ "FrameStageNotify" ] ) {
				callback( );
			}
		}

		if ( !g_csgo.m_engine->is_in_game( ) )
			return;

		if ( !g_ctx.m_local )
			return;

		g_hack.m_esp.run( );

		if ( !g_ctx.m_in_deathcam && g_ctx.m_local.get( ).is_alive( ) ) {
			g_hack.m_velgraph.draw( );
			g_hack.m_indscreen.draw( );
			g_hack.m_hitmarker.draw( );
		}
	} );
}

void cavalcade::hooks::chlc_client::level_init_pre_entity( const char* name ) {
	// begone
	g_csgo.m_sky_name   = std::nullopt;
	g_ctx.m_in_deathcam = false;
	g_hack.m_translator.m_pending_translations.clear( );
	g_entity_cacher.clear( );
	g_hack.m_trainer.clear( );
	g_hack.m_esp.clear( );
	g_hack.m_velgraph.clear( );
	g_hack.m_indscreen.clear( );
	g_hack.m_hitmarker.clear( );
	g_hack.m_movement.clear( );
	for ( auto& [ state, callbacks ] : g_ctx.m_lua.m_callbacks ) {
		state.set( XOR( "g_Cmd" ), sol::lua_nil );
	}

	// NOTE(para): it's particularly important for this to be here, before og
	g_hack.m_nightmode.clear( );

	g_ctx.m_cmd = nullptr;

	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_pre_entity_fn >( HASH_CT( "CHLClient::LevelInitPreEntity" ) );
	og( name );

	g_csgo.m_sky_name = g_ctx.m_cvars.sv_skyname->get_string( );
}

void cavalcade::hooks::chlc_client::level_init_post_entity( ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_post_entity_fn >( HASH_CT( "CHLClient::LevelInitPostEntity" ) );
	og( );
}

void cavalcade::hooks::chlc_client::on_override_mouse_input( unk ecx, unk edx, i32 slot, f32& x, f32& y ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< on_override_mouse_input_fn >( HASH_CT( "OnOverrideMouseInput" ) );

	og( ecx, edx, slot, std::ref( x ), std::ref( y ) );

	g_hack.m_aimbot.run( x, y );
	g_hack.m_movement.edgebug_scale_mouse( x );
}