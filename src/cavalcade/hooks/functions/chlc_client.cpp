#include "../hooks.hpp"

void cavalcade::hooks::chlc_client::frame_stage_notify( unk ecx, unk, sdk::frame_stage stage ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< frame_stage_notify_fn >( HASH_CT( "CHLClient::FrameStageNotify" ) );
	og( ecx, stage );

	if ( stage == sdk::frame_stage::RENDER_END ) {
		g_hack.m_scaleform.update( );
		g_render.m_safe.frame( [ & ]( ) {
			// std::unique_lock lock( g_lua.m_mutex );

			if ( g_csgo.m_engine->is_in_game( ) && g_ctx.m_local ) {
				g_hack.m_esp.run( );

				g_hack.m_fog.run( );
				g_hack.m_sunset.run( );

				if ( !g_ctx.m_in_deathcam && g_ctx.m_local.get( ).is_alive( ) ) {
					g_hack.m_velgraph.draw( );
					g_hack.m_indscreen.draw( );
					g_hack.m_hitmarker.draw( );
					g_hack.m_movement.pixelsurf_calculator( );
				}
			}

			for ( const auto& [ state, callbacks ] : g_lua.m_callbacks ) {
				for ( const auto& callback : callbacks.at( XOR( "Paint" ) ) ) {
					if ( callback.valid( ) ) {
						sol::protected_function_result result = callback( );

						if ( !result.valid( ) ) {
							sol::error err = result;
							g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
							g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
							g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
							g_csgo.m_cvars->console_color_printf( render::color( 255, 0, 0, 255 ), XOR( "ERROR: " ) );
							g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ),
							                                      io::format( XOR( "{}\n" ), err.what( ) ).c_str( ) );
						}
					}
				}
			}
		} );
	}

	for ( auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		state[ "g_FrameStage" ] = ( i32 )stage;
		for ( const auto& callback : callbacks[ XOR( "FrameStageNotify" ) ] ) {
			if ( callback.valid( ) ) {
				sol::protected_function_result result = callback( stage );

				if ( !result.valid( ) ) {
					sol::error err = result;
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 0, 0, 255 ), XOR( "ERROR: " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), io::format( XOR( "{}\n" ), err.what( ) ).c_str( ) );
				}
			}
		}
	}
}

void cavalcade::hooks::chlc_client::level_init_pre_entity( const char* name ) {
	// std::unique_lock lock( g_lua.m_mutex );

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
	g_hack.m_prediction.m_prevent_sounds = false;

	// NOTE(para): it's particularly important for this to be here, before og
	g_hack.m_nightmode.clear( );
	g_hack.m_fog.reset( );
	g_hack.m_sunset.reset( );
	g_hack.m_scaleform.reset( );

	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_pre_entity_fn >( HASH_CT( "CHLClient::LevelInitPreEntity" ) );
	og( name );

	g_csgo.m_map_name = std::string( name );
	g_csgo.m_sky_name = g_ctx.m_cvars.sv_skyname->get_string( );

	for ( const auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		for ( const auto& callback : callbacks.at( XOR( "LevelInitPreEntity" ) ) ) {
			if ( callback.valid( ) ) {
				sol::protected_function_result result = callback( std::string{ name } );

				if ( !result.valid( ) ) {
					sol::error err = result;
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 0, 0, 255 ), XOR( "ERROR: " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), io::format( XOR( "{}\n" ), err.what( ) ).c_str( ) );
				}
			}
		}
	}
}

void cavalcade::hooks::chlc_client::level_init_post_entity( ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_post_entity_fn >( HASH_CT( "CHLClient::LevelInitPostEntity" ) );
	og( );
	g_hack.m_scaleform.install( );
	for ( const auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		for ( const auto& callback : callbacks.at( XOR( "LevelInitPostEntity" ) ) ) {
			if ( callback.valid( ) ) {
				sol::protected_function_result result = callback( );

				if ( !result.valid( ) ) {
					sol::error err = result;
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 0, 0, 255 ), XOR( "ERROR: " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), io::format( XOR( "{}\n" ), err.what( ) ).c_str( ) );
				}
			}
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
	g_hack.m_prediction.m_prevent_sounds = false;

	for ( auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		state.set( XOR( "g_Cmd" ), sol::lua_nil );
		for ( const auto& callback : callbacks[ XOR( "LevelShutdown" ) ] ) {
			if ( callback.valid( ) ) {
				sol::protected_function_result result = callback( );

				if ( !result.valid( ) ) {
					sol::error err = result;
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 0, 0, 255 ), XOR( "ERROR: " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), io::format( XOR( "{}\n" ), err.what( ) ).c_str( ) );
				}
			}
		}
	}

	// NOTE(para): CSGOHudRadar gets destroyed and reallocated every game join . . .
	g_csgo.m_csgo_radar = nullptr;

	static auto og = g_mem[ CLIENT_DLL ].get_og< level_shutdown_fn >( HASH_CT( "CHLClient::LevelShutdown" ) );
	og( ecx, edx );
}

void cavalcade::hooks::chlc_client::on_override_mouse_input( unk ecx, unk edx, i32 slot, f32& x, f32& y ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< on_override_mouse_input_fn >( HASH_CT( "OnOverrideMouseInput" ) );

	og( ecx, edx, slot, std::ref( x ), std::ref( y ) );

	static auto& silent = gui::cfg::get< bool >( HASH_CT( "main:group1:silent" ) );

	if ( !silent )
		g_hack.m_aimbot.run( x, y );
	g_hack.m_movement.edgebug_scale_mouse( x );
	if ( g_hack.m_movement.m_pixelsurf.m_lock_mouse ) {
		x = 0;
		y = 0;
	}
}