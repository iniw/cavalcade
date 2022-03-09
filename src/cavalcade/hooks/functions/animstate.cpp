#include "../hooks.hpp"

// NOTE(para): fixes camera height if u duck after pred on high ping
void cavalcade::hooks::csgo_player_anim_state::modify_eye_position( unk, unk, math::v3f& ) {
	return;
}

bool cavalcade::hooks::base_animating::setup_bones( unk ecx, unk edx, math::matrix_3x4* bonetoworldout, i32 max, i32 mask, f32 time ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< setup_bones_fn >( HASH_CT( "CBaseAnimating::SetupBones" ) );

	if ( g_ctx.m_local && g_csgo.m_engine->is_in_game( ) ) {
		auto thisptr = ( sdk::cs_player* )ecx;
		if ( thisptr && thisptr->is_alive( ) ) {
			auto clazz = thisptr->get_client_class( );
			if ( clazz && ( uintptr_t )clazz > 0xFFFF && clazz->m_class_id == sdk::class_id::CS_PLAYER ) {
				auto old_curtime             = g_csgo.m_globals->m_curtime;
				auto old_realtime            = g_csgo.m_globals->m_realtime;
				auto old_frametime           = g_csgo.m_globals->m_frametime;
				auto old_abs_frametime       = g_csgo.m_globals->m_absoluteframetime;
				auto old_abs_frametime_start = g_csgo.m_globals->m_absoluteframestarttime;
				auto old_interp_amount       = g_csgo.m_globals->m_interpolation_amount;
				auto old_framecount          = g_csgo.m_globals->m_framecount;
				auto old_tickcount           = g_csgo.m_globals->m_tickcount;

				g_csgo.m_globals->m_curtime = g_csgo.m_globals->m_realtime = thisptr->get_sim_time( );
				g_csgo.m_globals->m_frametime                              = g_csgo.m_globals->m_absoluteframetime =
					g_csgo.m_globals->m_interval_per_tick * g_ctx.m_cvars.host_timescale->get_float( );
				g_csgo.m_globals->m_absoluteframestarttime = thisptr->get_sim_time( ) - g_csgo.m_globals->m_absoluteframetime;
				g_csgo.m_globals->m_interpolation_amount   = 0;
				g_csgo.m_globals->m_framecount = g_csgo.m_globals->m_tickcount = sdk::time_to_ticks( thisptr->get_sim_time( ) );

				thisptr->get_effects( ) |= 8;
				auto ret = og( ecx, edx, bonetoworldout, max, mask, time );
				thisptr->get_effects( ) &= ~8;

				g_csgo.m_globals->m_curtime                = old_curtime;
				g_csgo.m_globals->m_realtime               = old_realtime;
				g_csgo.m_globals->m_frametime              = old_frametime;
				g_csgo.m_globals->m_absoluteframetime      = old_abs_frametime;
				g_csgo.m_globals->m_absoluteframestarttime = old_abs_frametime_start;
				g_csgo.m_globals->m_interpolation_amount   = old_interp_amount;
				g_csgo.m_globals->m_framecount             = old_framecount;
				g_csgo.m_globals->m_tickcount              = old_tickcount;
				return ret;
			}
		}
	}

	return og( ecx, edx, bonetoworldout, max, mask, time );
}