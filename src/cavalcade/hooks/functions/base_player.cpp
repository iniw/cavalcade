#include "../hooks.hpp"

#define MULTIPLAYER_BACKUP 150
bool cavalcade::hooks::base_player::create_move( sdk::cs_player* ecx, unk, f32 input_sample_time, sdk::user_cmd* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< create_move_fn >( HASH_CT( "C_BasePlayer::CreateMove" ) );
	for ( auto& [ state, _ ] : g_lua.m_callbacks ) {
		state.set( XOR( "g_Cmd" ), sol::lua_nil );
	}
	static bool shoot = false;
	static auto& zb   = gui::cfg::get< bool >( HASH_CT( "main:group1:zeus bug" ) );

	// NOTE(para): don't do local checks in features ty
	// sidenote: when I see pointer checks I want to cry myself to Rust now, honestly
	if ( !g_ctx.m_local )
		return og( ecx, input_sample_time, cmd );

	if ( ( g_ctx.m_local && !g_ctx.m_local.get( ).is_alive( ) ) ) {
		shoot = false;
		g_hack.m_indscreen.clear( );
		return og( ecx, input_sample_time, cmd );
	}

	if ( !cmd || !cmd->m_command_number || !input_sample_time )
		return og( ecx, input_sample_time, cmd );

	if ( og( ecx, input_sample_time, cmd ) )
		ecx->set_local_view_angles( cmd->m_view_angles );

	// NOTE(para): update our command after modification, just in case
	auto slot         = cmd->m_command_number % MULTIPLAYER_BACKUP;
	auto verified_cmd = g_csgo.m_input->get_verified_cmd( slot );

	if ( !verified_cmd )
		return og( ecx, input_sample_time, cmd );

	g_ctx.m_cmd = cmd;

	for ( auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		state.set( XOR( "g_Cmd" ), g_ctx.m_cmd );
		for ( const auto& callback : callbacks[ XOR( "CreateMove" ) ] ) {
			callback( );
		}
	}

	g_hack.m_movement.pre( );

	// masturbation mode
	// g_ctx.m_cvars.viewmodel_offset_z->set_value( sin( g_csgo.m_globals->m_curtime * 3 ) * 2.f );

	g_hack.m_velgraph.gather( );
	g_hack.m_indscreen.gather( );

	g_hack.m_trainer.run( );

	// NOTE(para): RED BLOCK
	{
		g_hack.m_prediction.update( );
		g_hack.m_prediction.start( );
		g_hack.m_prediction.apply( );
		g_hack.m_prediction.restore( );

		g_hack.m_movement.post( );

		if ( zb ) {
			if ( shoot ) {
				g_csgo.m_engine->execute_client_cmd( XOR( "use weapon_taser" ) );
				shoot = false;
			}

			if ( cmd->m_buttons & 1 && g_ctx.m_local.get( ).can_fire_shot( ) ) {
				shoot = true;
			}
		}

		cmd->m_view_angles.sanitize( );
		cmd->m_view_angles.clamp_angle( );
	}

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->get_checksum( );

	return false;
}