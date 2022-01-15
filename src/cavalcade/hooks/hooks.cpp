#include "hooks.hpp"

bool cavalcade::hooks::init( ) {
	MOCKING_TRY;

	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::FrameStageNotify" ), &chlc_client::frame_stage_notify );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::LevelInitPreEntity" ), &chlc_client::level_init_pre_entity );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::LevelInitPostEntity" ), &chlc_client::level_init_post_entity );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "PushNotice" ), &push_notice_::push_notice );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CEngineClient::ClientCmd" ), &client_cmd_::client_cmd );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "C_BasePlayer::CreateMove" ), &base_player::create_move );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CEntityListener::OnAddEntity" ), &entity_listener::on_add_entity );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CEntityListener::OnRemoveEntity" ), &entity_listener::on_remove_entity );
	MOCK g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::EndScene" ), &d3d9_device::end_scene );
	MOCK g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::Reset" ), &d3d9_device::reset );

	MOCKING_CATCH( return false );

	return true;
}