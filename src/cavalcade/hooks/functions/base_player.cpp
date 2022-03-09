#include "../hooks.hpp"

static bool prev = false;

void cavalcade::hooks::engine_client::set_view_angles( unk ecx, unk edx, unk ang ) {
	if ( prev )
		return;

	static auto og = g_mem[ CLIENT_DLL ].get_og< set_view_angles_fn >( HASH_CT( "SetViewAngles" ) );

	return og( ecx, edx, ang );
}

#define MULTIPLAYER_BACKUP 150
bool cavalcade::hooks::base_player::create_move( f32 input_sample_time, sdk::user_cmd* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< create_move_fn >( HASH_CT( "CreateMove" ) );
	// std::unique_lock lock( g_lua.m_mutex );
	for ( auto& [ state, _ ] : g_lua.m_callbacks ) {
		state.set( XOR( "g_Cmd" ), sol::lua_nil );
	}
	static bool shoot = false;
	static auto& zb   = gui::cfg::get< bool >( HASH_CT( "main:group1:zeus bug" ) );

	// NOTE(para): don't do local checks in features ty
	// sidenote: when I see pointer checks I want to cry myself to Rust now, honestly
	if ( !g_ctx.m_local )
		return og( input_sample_time, cmd );

	if ( ( g_ctx.m_local && !g_ctx.m_local.get( ).is_alive( ) ) ) {
		shoot = false;
		g_hack.m_indscreen.clear( );
		g_hack.m_backtrack.clear( );
		return og( input_sample_time, cmd );
	}

	// NOTE(para): update our command after modification, just in case
	auto slot         = cmd->m_command_number % MULTIPLAYER_BACKUP;
	auto verified_cmd = g_csgo.m_input->get_verified_cmd( slot );

	if ( !verified_cmd )
		return og( input_sample_time, cmd );

	if ( !cmd || !cmd->m_command_number )
		return og( input_sample_time, cmd );

	g_ctx.m_cmd = cmd;

	if ( !input_sample_time )
		return og( input_sample_time, cmd );

	const auto cur = cmd->m_view_angles;

	og( input_sample_time, cmd );

	for ( auto& [ state, callbacks ] : g_lua.m_callbacks ) {
		state.set( XOR( "g_Cmd" ), g_ctx.m_cmd );
		for ( const auto& callback : callbacks[ XOR( "CreateMove" ) ] ) {
			if ( callback.valid( ) ) {
				sol::protected_function_result result = callback( cmd );

				if ( !result.valid( ) ) {
					sol::error err = result;
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "cavalcade.cc" ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 0, 0, 255 ), XOR( "ERROR: " ) );
					g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), io::format( XOR( "{}\n" ), err.what( ) ).c_str( ) );
				}
			}
		}
	}

	g_hack.m_movement.pre( );

	// masturbation mode
	// g_ctx.m_cvars.viewmodel_offset_z->set_value( sin( g_csgo.m_globals->m_curtime * 3 ) * 2.f );

	g_hack.m_velgraph.gather( );
	g_hack.m_indscreen.gather( );

	g_hack.m_trainer.run( );

	auto old_angles = cmd->m_view_angles;
	// NOTE(para): RED BLOCK
	{
		g_hack.m_prediction.update( );
		g_hack.m_prediction.start( );
		g_hack.m_prediction.apply( );
		static auto& silent = gui::cfg::get< bool >( HASH_CT( "main:group1:silent" ) );

		g_hack.m_backtrack.store_records( );
		if ( silent )
			g_hack.m_aimbot.run( cmd->m_view_angles.pitch, cmd->m_view_angles.yaw, true );
		g_hack.m_backtrack.run( );

		g_hack.m_prediction.restore( );

		g_hack.m_movement.post( );

		if ( zb ) {
			if ( shoot ) {
				g_csgo.m_engine->execute_client_cmd( XOR( "use weapon_taser" ) );
				shoot                              = false;
				g_hack.m_scaleform.m_pending_ticks = 5;
			}

			if ( cmd->m_buttons & 1 && g_ctx.m_local.get( ).can_fire_shot( ) ) {
				shoot = true;
			}
		}

		cmd->m_view_angles.sanitize( );

		constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };

		if ( silent ) {
			auto yaw       = cur.yaw;
			auto old_yaw   = yaw + ( yaw < 0.0f ? 360.0f : 0.0f );
			auto new_yaw   = cmd->m_view_angles.yaw + ( cmd->m_view_angles.yaw < 0.0f ? 360.0f : 0.0f );
			auto yaw_delta = new_yaw < old_yaw ? fabsf( new_yaw - old_yaw ) : 360.0f - fabsf( new_yaw - old_yaw );
			yaw_delta      = 360.0f - yaw_delta;

			auto forwardmove    = cmd->m_forward_move;
			auto sidemove       = cmd->m_side_move;
			cmd->m_forward_move = cos( DEG2RAD( yaw_delta ) ) * forwardmove + cos( DEG2RAD( yaw_delta + 90.0f ) ) * sidemove;
			cmd->m_side_move    = sin( DEG2RAD( yaw_delta ) ) * forwardmove + sin( DEG2RAD( yaw_delta + 90.0f ) ) * sidemove;
			cmd->m_forward_move = std::clamp( cmd->m_forward_move, -450.0f, 450.0f );
			cmd->m_side_move    = std::clamp( cmd->m_side_move, -450.0f, 450.0f );
		}

		cmd->m_view_angles.clamp_angle( );
	}

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = verified_cmd->m_cmd.get_checksum( );

	g_hack.m_scaleform.update( );

	return false;
}

int cavalcade::hooks::base_player::some_fn( unk ecx, unk edx, unk a1, unk a2, unk a3, unk a4 ) {
	static auto og      = g_mem[ CLIENT_DLL ].get_og< some_fn_ >( HASH_CT( "SomeFn" ) );
	static bool& interp = **g_mem[ CLIENT_DLL ].get_address< mem::address >( HASH_CT( "g_bInterpolationEnabled" ) ).add< bool** >( 2 );
	static auto& bt     = gui::cfg::get< bool >( HASH_CT( "main:group1:backtrack" ) );
	static auto& btt    = gui::cfg::get< f32 >( HASH_CT( "main:group1:backtrack time" ) );

	auto backup = interp;
	if ( ( bt && btt > 0.F ) && g_ctx.m_local && ( ( sdk::cs_player* )( ecx ) )->is_enemy( g_ctx.m_local ) )
		interp = false;
	auto ret = og( ecx, edx, a1, a2, a3, a4 );
	interp   = backup;
	return ret;
}