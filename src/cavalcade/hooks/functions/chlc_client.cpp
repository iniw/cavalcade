#include "../hooks.hpp"

void cavalcade::hooks::chlc_client::frame_stage_notify( unk ecx, unk, sdk::frame_stage stage ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< frame_stage_notify_fn >( HASH_CT( "CHLClient::FrameStageNotify" ) );
	og( ecx, stage );

	g_render.m_safe.clear( );

	g_render.m_safe.draw_shape< render::geometry::rect_filled >( render::point{ 10, 10 }, render::point{ 30, 30 }, 0xffffffff );
	static auto f = &g_render.m_fonts[ render::font::MENU ];
	g_render.m_safe.draw_shape< render::geometry::text >( f, render::point{ 10, 40 }, "hello world", 0xffffffff );
}

void cavalcade::hooks::chlc_client::level_init_pre_entity( const char* name ) {
	// begone
	g_hack.m_translator.m_pending_translations.clear( );
	g_entity_cacher.clear( );
	g_hack.m_trainer.m_checkpoint = std::nullopt;
	g_ctx.m_cmd                   = nullptr;

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