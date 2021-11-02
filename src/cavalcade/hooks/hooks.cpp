#include "hooks.hpp"

bool cavalcade::hooks::init( ) {
	MOCKING_REGION( MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::FrameStageNotify" ), &chlc_client::frame_stage_notify );
	                MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "C_BasePlayer::CreateMove" ), &base_player::create_move );
	                MOCK g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::EndScene" ), &d3d9_device::end_scene );
	                MOCK g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::Reset" ), &d3d9_device::reset ); );

	return true;
}