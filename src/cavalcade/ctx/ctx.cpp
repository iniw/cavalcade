#include "ctx.hpp"

bool cavalcade::ctx::init( ) {
	MOCKING_TRY;

	MOCK m_cvars.sv_cheats                       = g_csgo.m_cvars->find_var( _( "sv_cheats" ) );
	MOCK m_cvars.cl_updaterate                   = g_csgo.m_cvars->find_var( _( "cl_updaterate" ) );
	MOCK m_cvars.sv_minupdaterate                = g_csgo.m_cvars->find_var( _( "sv_minupdaterate" ) );
	MOCK m_cvars.sv_maxupdaterate                = g_csgo.m_cvars->find_var( _( "sv_maxupdaterate" ) );
	MOCK m_cvars.viewmodel_offset_z              = g_csgo.m_cvars->find_var( _( "viewmodel_offset_z" ) );
	MOCK m_cvars.cl_interp                       = g_csgo.m_cvars->find_var( _( "cl_interp" ) );
	MOCK m_cvars.cl_interp_ratio                 = g_csgo.m_cvars->find_var( _( "cl_interp_ratio" ) );
	MOCK m_cvars.sv_client_min_interp_ratio      = g_csgo.m_cvars->find_var( _( "sv_client_min_interp_ratio" ) );
	MOCK m_cvars.sv_client_max_interp_ratio      = g_csgo.m_cvars->find_var( _( "sv_client_max_interp_ratio" ) );
	MOCK m_cvars.sv_maxunlag                     = g_csgo.m_cvars->find_var( _( "sv_maxunlag" ) );
	MOCK m_cvars.sv_showlagcompensation_duration = g_csgo.m_cvars->find_var( _( "sv_showlagcompensation_duration" ) );
	MOCK m_cvars.sv_autobunnyhopping             = g_csgo.m_cvars->find_var( _( "sv_autobunnyhopping" ) );
	MOCK m_cvars.mp_teammates_are_enemies        = g_csgo.m_cvars->find_var( _( "mp_teammates_are_enemies" ) );
	MOCK m_cvars.sensitivity                     = g_csgo.m_cvars->find_var( _( "sensitivity" ) );
	MOCK m_cvars.zoom_sensitivity_ratio_mouse    = g_csgo.m_cvars->find_var( _( "zoom_sensitivity_ratio_mouse" ) );
	MOCK m_cvars.m_pitch                         = g_csgo.m_cvars->find_var( _( "m_pitch" ) );
	MOCK m_cvars.m_yaw                           = g_csgo.m_cvars->find_var( _( "m_yaw" ) );

	MOCK m_steam.m_steam_client = g_csgo.m_engine->get_steam_api_context( )->m_steam_client;

	static HSteamUser steam_user = SteamAPI_GetHSteamUser( );
	static HSteamPipe steam_pipe = SteamAPI_GetHSteamUser( );

	MOCK m_steam.m_steam_user =
		( ISteamUser* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMUSER_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_user_stats =
		( ISteamUserStats* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMUSERSTATS_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_friends =
		( ISteamFriends* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMFRIENDS_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_game_coordinator = ( ISteamGameCoordinator* )( m_steam.m_steam_client->GetISteamGenericInterface(
		steam_pipe, steam_user, STEAMGAMECOORDINATOR_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_matchmaking =
		( ISteamMatchmaking* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMMATCHMAKING_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_utils =
		( ISteamUtils* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMUTILS_INTERFACE_VERSION ) );

	MOCKING_CATCH( return false );

	m_steam.m_steam_friends->SetListenForFriendsMessages( true );

	g_io.log( _( "initialized ctx" ) );

	return true;
}