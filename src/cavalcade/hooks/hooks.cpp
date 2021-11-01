#include "hooks.hpp"

bool cavalcade::hooks::init( ) {
	if ( !g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::FrameStageNotify" ), &chlc_client::frame_stage_notify ) )
		return false;

	if ( !g_mem[ CLIENT_DLL ].hook( HASH_CT( "C_BasePlayer::CreateMove" ), &base_player::create_move ) )
		return false;

	if ( !g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::EndScene" ), &d3d9_device::end_scene ) )
		return false;

	if ( !g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::Reset" ), &d3d9_device::reset ) )
		return false;

	return true;
}