#include "../hooks.hpp"

bool cavalcade::hooks::base_player::create_move( sdk::cs_player* ecx, unk, f32 input_sample_time, sdk::user_cmd* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< create_move_fn >( HASH_CT( "C_BasePlayer::CreateMove" ) );

	// NOTE(para): don't do local checks in features ty
	// sidenote: when I see pointer checks I want to cry myself to Rust now, honestly
	if ( !g_ctx.m_local || ( g_ctx.m_local && !g_ctx.m_local.get( ).is_alive( ) ) )
		return og( ecx, input_sample_time, cmd );

	// NOTE(para): IIRC, in mouse shittery, input sample time is an identifier. if we will store any datamap backups, we should prevent our cheat
	// create-move from running there.
	if ( !cmd->m_command_number || !input_sample_time )
		return og( ecx, input_sample_time, cmd );

	if ( og( ecx, input_sample_time, cmd ) )
		ecx->set_local_view_angles( cmd->m_view_angles );

	// masturbation mode
	g_ctx.m_cvars.viewmodel_offset_z->set_value( sin( g_csgo.m_globals->m_curtime * 3 ) * 2.f );

	// g_hack.m_prediction.update( );

	// g_hack.m_prediction.start( );

	// g_hack.m_prediction.end( );

	cmd->m_view_angles.sanitize( );

	return false;
}