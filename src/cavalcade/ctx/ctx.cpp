#include "ctx.hpp"

bool cavalcade::ctx::init( ) {
	MOCKING_REGION( MOCK m_cvars.cl_updaterate                   = g_csgo.m_cvars->find_var( XOR( "cl_updaterate" ) );
	                MOCK m_cvars.sv_minupdaterate                = g_csgo.m_cvars->find_var( XOR( "sv_minupdaterate" ) );
	                MOCK m_cvars.sv_maxupdaterate                = g_csgo.m_cvars->find_var( XOR( "sv_maxupdaterate" ) );
	                MOCK m_cvars.cl_interp                       = g_csgo.m_cvars->find_var( XOR( "cl_interp" ) );
	                MOCK m_cvars.cl_interp_ratio                 = g_csgo.m_cvars->find_var( XOR( "cl_interp_ratio" ) );
	                MOCK m_cvars.sv_client_min_interp_ratio      = g_csgo.m_cvars->find_var( XOR( "sv_client_min_interp_ratio" ) );
	                MOCK m_cvars.sv_client_max_interp_ratio      = g_csgo.m_cvars->find_var( XOR( "sv_client_max_interp_ratio" ) );
	                MOCK m_cvars.sv_maxunlag                     = g_csgo.m_cvars->find_var( XOR( "sv_maxunlag" ) );
	                MOCK m_cvars.sv_showlagcompensation_duration = g_csgo.m_cvars->find_var( XOR( "sv_showlagcompensation_duration" ) );
	                MOCK m_cvars.sv_autobunnyhopping             = g_csgo.m_cvars->find_var( XOR( "sv_autobunnyhopping" ) );
	                MOCK m_cvars.mp_teammates_are_enemies        = g_csgo.m_cvars->find_var( XOR( "mp_teammates_are_enemies" ) );
	                MOCK m_cvars.sensitivity                     = g_csgo.m_cvars->find_var( XOR( "sensitivity" ) );
	                MOCK m_cvars.m_pitch                         = g_csgo.m_cvars->find_var( XOR( "m_pitch" ) );
	                MOCK m_cvars.m_yaw                           = g_csgo.m_cvars->find_var( XOR( "m_yaw" ) ); );

	g_io.log( XOR( "initialized ctx" ) );

	return true;
}

void cavalcade::ctx::iter_players( const std::function< void( sdk::cs_player* ) >& fn, i32 flags ) {
	if ( !m_local )
		return;

	for ( i32 i = 1; i <= g_csgo.m_globals->m_max_clients; i++ ) {
		auto player = g_csgo.m_ent_list->get< sdk::cs_player* >( i );
		if ( !player )
			continue;

		if ( !( flags & LOCAL ) )
			if ( player == m_local )
				continue;

		if ( !( flags & DEAD ) )
			if ( !player->is_alive( ) )
				continue;

		if ( !( flags & DORMANT ) )
			if ( player->is_dormant( ) )
				continue;

		if ( !( flags & TEAMMATE ) )
			if ( !player->is_enemy( m_local ) )
				continue;

		if ( !( flags & IMMUNE ) )
			if ( player->is_immune( ) )
				continue;

		fn( player );
	}
}