#include "../hooks.hpp"

void cavalcade::hooks::chlc_client::frame_stage_notify( unk ecx, unk, sdk::frame_stage stage ) {
	static auto og = g_mem[ HASH_CT( "client.dll" ) ].get_og< frame_stage_notify_fn >( HASH_CT( "CHLClient::FrameStageNotify" ) );
	og( ecx, stage );
}