#include "../hooks.hpp"

bool cavalcade::hooks::base_player::create_move( sdk::cs_player* ecx, unk, f32 input_sample_time, sdk::user_cmd* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< create_move_fn >( HASH_CT( "C_BasePlayer::CreateMove" ) );

	// NOTE(para): we should probably consider a grace period for createmove from spawntime because there's some retarded stuff that might intersect

	// NOTE(para): don't do local checks in features ty
	// sidenote: when I see pointer checks I want to cry myself to Rust now, honestly
	if ( !g_ctx.m_local || ( g_ctx.m_local && !g_ctx.m_local.get( ).is_alive( ) ) )
		return og( ecx, input_sample_time, cmd );

	if ( !cmd || !cmd->m_command_number )
		return og( ecx, input_sample_time, cmd );

	// NOTE(para): IIRC, in mouse shittery, input sample time is an identifier. if we will store any datamap backups, we should prevent our cheat
	// create-move from running there.
	// we though want to update our global cmd for it ***(FOR READS ONLY!!!)***
	g_ctx.m_cmd = cmd;

	if ( !input_sample_time ) {
		return og( ecx, input_sample_time, cmd );
	}

	if ( og( ecx, input_sample_time, cmd ) )
		ecx->set_local_view_angles( cmd->m_view_angles );

	// NOTE(para): retarded,,, leaving it just in case for now
	if ( g_ctx.m_cmd )
		return og( ecx, input_sample_time, cmd );

	// masturbation mode
	g_ctx.m_cvars.viewmodel_offset_z->set_value( sin( g_csgo.m_globals->m_curtime * 3 ) * 2.f );

	g_hack.m_trainer.run( );

	g_hack.m_prediction.update( );

	g_hack.m_prediction.start( );

	g_hack.m_prediction.end( );

	// NOTE(para): should we even do this? wouldn't it be better if we didnt touch these, ever?
	// outside of buttons of course, but that's going to be marked as untrusted, anyway.
	// it'd be stupid if Valve didn't verify user commands on server.
	cmd->m_view_angles.sanitize( );
	cmd->m_view_angles.clamp_angle( );

	return false;
}