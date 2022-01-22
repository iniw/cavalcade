#include "../hooks.hpp"

void cavalcade::hooks::chlc_client::frame_stage_notify( unk ecx, unk, sdk::frame_stage stage ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< frame_stage_notify_fn >( HASH_CT( "CHLClient::FrameStageNotify" ) );

	if ( stage >= sdk::frame_stage::START )
		g_render.m_safe.frame( [ & ]( ) {
			g_hack.m_esp.run( );
			g_hack.m_velgraph.draw( );
			g_hack.m_indscreen.draw( );
		} );

	og( ecx, stage );
}

void cavalcade::hooks::chlc_client::level_init_pre_entity( const char* name ) {
	// begone
	g_hack.m_translator.m_pending_translations.clear( );
	g_entity_cacher.clear( );
	g_hack.m_trainer.clear( );
	g_hack.m_esp.clear( );
	g_hack.m_velgraph.clear( );
	g_hack.m_indscreen.clear( );
	g_ctx.m_cmd = nullptr;

	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_pre_entity_fn >( HASH_CT( "CHLClient::LevelInitPreEntity" ) );
	og( name );
}

void cavalcade::hooks::chlc_client::level_init_post_entity( ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< level_init_post_entity_fn >( HASH_CT( "CHLClient::LevelInitPostEntity" ) );
	og( );
}

void cavalcade::hooks::chlc_client::on_override_mouse_input( unk ecx, unk edx, i32 slot, f32& x, f32& y ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< on_override_mouse_input_fn >( HASH_CT( "OnOverrideMouseInput" ) );

	og( ecx, edx, slot, std::ref( x ), std::ref( y ) );

	g_hack.m_aimbot.run( x, y );
}