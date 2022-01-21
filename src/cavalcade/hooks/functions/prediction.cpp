
#include "../hooks.hpp"
#include <corecrt_math_defines.h>

inline constexpr f32 DEG_2_RAD( const f32 x ) {
	return x * ( M_PI / 180.f );
}

void cavalcade::hooks::prediction::setup_move( unk ecx, unk edx, sdk::cs_player* pl, sdk::user_cmd* cmd, unk arg, sdk::move_data* move ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< setup_move_fn >( HASH_CT( "CPrediction::SetupMove" ) );
	og( ecx, edx, pl, cmd, arg, move );

	if ( g_hack.m_aimbot.m_movement_fix && g_hack.m_aimbot.m_aiming ) {
		f32 old_yaw      = g_hack.m_aimbot.m_old_angles.yaw + ( g_hack.m_aimbot.m_old_angles.yaw < 0.F ? 360.F : 0.F );
		auto view_angles = g_csgo.m_engine->get_view_angles( );
		f32 new_yaw      = view_angles.yaw + ( view_angles.yaw < 0.F ? 360.F : 0.F );
		f32 yaw_delta    = new_yaw < old_yaw ? fabsf( new_yaw - old_yaw ) : 360.F - fabsf( new_yaw - old_yaw );
		yaw_delta        = 360.F - yaw_delta;

		// run this over a cmd processed by the game... this is where it updates this so it should be fine, lets not be libertines about this though
		auto fm = cmd->m_forward_move;
		auto sm = cmd->m_side_move;

		cmd->m_forward_move  = cos( DEG_2_RAD( yaw_delta ) ) * fm + cos( DEG_2_RAD( yaw_delta + 90.F ) ) * sm;
		cmd->m_side_move     = sin( DEG_2_RAD( yaw_delta ) ) * fm + sin( DEG_2_RAD( yaw_delta + 90.F ) ) * sm;
		move->m_forward_move = cmd->m_forward_move;
		move->m_side_move    = cmd->m_side_move;
	}
}