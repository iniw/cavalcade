#include "../hooks.hpp"

bool cavalcade::hooks::base_player::create_move( sdk::cs_player* ecx, unk, f32 input_sample_time, sdk::user_cmd* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< create_move_fn >( HASH_CT( "C_BasePlayer::CreateMove" ) );

	// NOTE(para): we should probably consider a grace period for createmove from spawntime because there's some retarded stuff that might intersect

	// NOTE(para): don't do local checks in features ty
	// sidenote: when I see pointer checks I want to cry myself to Rust now, honestly
	if ( !g_ctx.m_local || ( g_ctx.m_local && !g_ctx.m_local.get( ).is_alive( ) ) )
		return og( ecx, input_sample_time, cmd );

	// NOTE(para): IIRC, in mouse shittery, input sample time is an identifier. if we will store any datamap backups, we should prevent our cheat
	// create-move from running there.
	if ( !cmd || !cmd->m_command_number || !input_sample_time )
		return og( ecx, input_sample_time, cmd );

	if ( og( ecx, input_sample_time, cmd ) )
		ecx->set_local_view_angles( cmd->m_view_angles );

	g_ctx.m_cmd = cmd;

	// masturbation mode
	g_ctx.m_cvars.viewmodel_offset_z->set_value( sin( g_csgo.m_globals->m_curtime * 3 ) * 2.f );

	if ( g_io.key_state< io::key_state::RELEASED >( VK_XBUTTON1 ) ) {
		g_ctx.m_trainer.m_checkpoint = { g_ctx.m_local.get( ).get_origin( ), g_ctx.m_local.get( ).get_abs_angles( ) };
		g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
			0, 0, "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Updated checkpoint..." );
	}

	if ( g_io.key_state< io::key_state::RELEASED >( VK_XBUTTON2 ) ) {
		if ( g_ctx.go_to_checkpoint( ) ) {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Teleported to checkpoint..." );
		}
	}

	g_hack.m_prediction.update( );

	g_hack.m_prediction.start( );

	g_hack.m_prediction.end( );

	cmd->m_view_angles.sanitize( );

	return false;
}