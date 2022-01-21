#include "../hooks.hpp"

#define MULTIPLAYER_BACKUP 150
bool cavalcade::hooks::base_player::create_move( sdk::cs_player* ecx, unk, f32 input_sample_time, sdk::user_cmd* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< create_move_fn >( HASH_CT( "C_BasePlayer::CreateMove" ) );

	// NOTE(para): don't do local checks in features ty
	// sidenote: when I see pointer checks I want to cry myself to Rust now, honestly
	if ( !g_ctx.m_local || ( g_ctx.m_local && !g_ctx.m_local.get( ).is_alive( ) ) )
		return og( ecx, input_sample_time, cmd );

	if ( !cmd || !cmd->m_command_number )
		return og( ecx, input_sample_time, cmd );

	// NOTE(para): IIRC, in mouse shittery, input sample time is an identifier. if we will store any datamap backups, we should prevent our cheat
	// create-move from running there.
	// we though want to update our global cmd for it ***(FOR READS ONLY!!!)***

	// NOTE(para:21-1-22): this is probably not needed.
	g_ctx.m_cmd = cmd;

	// NOTE(para): we don't want to verify if we have a cmd for aimbot because aimbot runs every frame
	// and if we verify for cmd it'll stutter (for obvious reasons, the cmd doesn't
	// get updated enough times), so we rely on a flag that should be accurate 99.9% of the time.
	// - Alternative is verifying IO but this feels better for some reason.
	g_hack.m_aimbot.m_is_attacking = g_ctx.m_cmd->m_buttons & 1;
	g_hack.m_aimbot.m_old_angles   = g_csgo.m_engine->get_view_angles( );

	if ( !input_sample_time ) {
		return og( ecx, input_sample_time, cmd );
	}

	if ( og( ecx, input_sample_time, cmd ) )
		ecx->set_local_view_angles( cmd->m_view_angles );

	// NOTE(para): update our command after modification, just in case
	auto slot         = cmd->m_command_number % MULTIPLAYER_BACKUP;
	auto verified_cmd = g_csgo.m_input->get_verified_cmd( slot );

	if ( !verified_cmd )
		return og( ecx, input_sample_time, cmd );

	// masturbation mode
	g_ctx.m_cvars.viewmodel_offset_z->set_value( sin( g_csgo.m_globals->m_curtime * 3 ) * 2.f );

	g_hack.m_trainer.run( );

	// NOTE(para): RED BLOCK
	{
		g_hack.m_prediction.update( );

		g_hack.m_prediction.start( );

		g_hack.m_prediction.end( );

		cmd->m_view_angles.sanitize( );
		cmd->m_view_angles.clamp_angle( );
	}

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->get_checksum( );

	return false;
}