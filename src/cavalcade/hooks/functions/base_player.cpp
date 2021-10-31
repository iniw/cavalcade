#include "../hooks.hpp"

bool cavalcade::hooks::base_player::create_move( sdk::cs_player* ecx, unk, f32 input_sample_time, sdk::user_cmd* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< create_move_fn >( HASH_CT( "C_BasePlayer::CreateMove" ) );

	if ( !cmd->m_command_number )
		return og( ecx, input_sample_time, cmd );

	if ( og( ecx, input_sample_time, cmd ) )
		ecx->set_local_view_angles( cmd->m_view_angles );

	g_hack.m_prediction.update( );

	g_hack.m_prediction.start( );

	g_hack.m_prediction.end( );

	cmd->m_view_angles.sanitize( );

	return false;
}