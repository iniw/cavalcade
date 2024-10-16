#include "prediction.hpp"

#include "../../ctx/ctx.hpp"

void hack::other::prediction::start( ) {
	g_csgo.m_move_helper->set_host( g_ctx.m_local );

	m_prevent_sounds = true;

	g_csgo.m_prediction->start_command( g_ctx.m_local, g_ctx.m_cmd );

	m_backup_curtime   = g_csgo.m_globals->m_curtime;
	m_backup_frametime = g_csgo.m_globals->m_frametime;

	g_csgo.m_globals->m_curtime   = sdk::ticks_to_time( g_ctx.m_local.get( ).get_tickbase( ) );
	g_csgo.m_globals->m_frametime = g_csgo.m_prediction->m_engine_paused ? 0.f : g_csgo.m_globals->m_interval_per_tick;

	m_backup_is_first_time_predicted = g_csgo.m_prediction->m_is_first_time_predicted;
	m_backup_in_prediction           = g_csgo.m_prediction->m_in_prediction;

	g_csgo.m_prediction->m_is_first_time_predicted = false;
	g_csgo.m_prediction->m_in_prediction           = true;

	g_ctx.m_cmd->m_buttons |= g_ctx.m_local.get( ).get_button_forced( );
	g_ctx.m_cmd->m_buttons &= ~g_ctx.m_local.get( ).get_button_disabled( );

	g_csgo.m_game_movement->start_track_prediction_errors( g_ctx.m_local );

	// NOTE(para): to investigate crash
	// if ( g_ctx.m_cmd->m_weapon_select ) {
	// 	auto weapon = g_csgo.m_ent_list->get< sdk::weapon_cs_base* >( g_ctx.m_cmd->m_weapon_select );
	// 	if ( weapon ) {
	// 		auto weapon_info = weapon->get_cs_weapon_info( );
	// 		if ( weapon_info )
	// 			g_ctx.m_local.get( ).select_item( weapon_info->m_weapon_name, g_ctx.m_cmd->m_weapon_sub_type );
	// 	}
	// }

	if ( g_ctx.m_cmd->m_impulse )
		g_ctx.m_local.get( ).get_impulse( ) = g_ctx.m_cmd->m_impulse;

	g_ctx.m_local.get( ).update_button_state( g_ctx.m_cmd->m_buttons );

	g_csgo.m_prediction->check_moving_ground( g_ctx.m_local, g_csgo.m_globals->m_frametime );

	// g_ctx.m_local.get( ).set_local_view_angles( g_ctx.m_cmd->m_view_angles );

	memset( &m_move_data, 0, sizeof( decltype( m_move_data ) ) );

	g_csgo.m_prediction->setup_move( g_ctx.m_local, g_ctx.m_cmd, g_csgo.m_move_helper, &m_move_data );
}

void hack::other::prediction::apply( ) {
	g_csgo.m_game_movement->process_movement( g_ctx.m_local, &m_move_data );

	g_csgo.m_prediction->finish_move( g_ctx.m_local, g_ctx.m_cmd, &m_move_data );

	g_csgo.m_move_helper->process_impacts( );
}

void hack::other::prediction::restore( ) {
	m_prevent_sounds = false;

	g_csgo.m_game_movement->finish_track_prediction_errors( g_ctx.m_local );

	g_csgo.m_prediction->m_in_prediction           = m_backup_in_prediction;
	g_csgo.m_prediction->m_is_first_time_predicted = m_backup_is_first_time_predicted;

	g_csgo.m_globals->m_curtime   = m_backup_curtime;
	g_csgo.m_globals->m_frametime = m_backup_frametime;

	g_csgo.m_prediction->finish_command( g_ctx.m_local );

	g_csgo.m_move_helper->set_host( nullptr );

	g_csgo.m_game_movement->reset( );
}

void hack::other::prediction::update( ) {
	if ( g_csgo.m_client_state->m_delta_tick > 0 ) {
		g_csgo.m_prediction->update( g_csgo.m_client_state->m_delta_tick, g_csgo.m_client_state->m_delta_tick > 0,
		                             g_csgo.m_client_state->m_last_command_ack,
		                             g_csgo.m_client_state->m_last_outgoing_command + g_csgo.m_client_state->m_choked_commands );
	}
}