#include "ctx.hpp"

#include "../../other/lib/include/json/json.hpp"

cavalcade::ctx::ctx( ) : m_translator( "http://localhost:5000" ) {
	m_translator_initialized = m_translator.is_valid( );
}

bool cavalcade::ctx::init( ) {
	MOCKING_TRY;

	MOCK m_cvars.cl_updaterate                   = g_csgo.m_cvars->find_var( XOR( "cl_updaterate" ) );
	MOCK m_cvars.sv_minupdaterate                = g_csgo.m_cvars->find_var( XOR( "sv_minupdaterate" ) );
	MOCK m_cvars.sv_maxupdaterate                = g_csgo.m_cvars->find_var( XOR( "sv_maxupdaterate" ) );
	MOCK m_cvars.viewmodel_offset_z              = g_csgo.m_cvars->find_var( XOR( "viewmodel_offset_z" ) );
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
	MOCK m_cvars.m_yaw                           = g_csgo.m_cvars->find_var( XOR( "m_yaw" ) );

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

	g_io.log( XOR( "initialized ctx" ) );

	return true;
}

void cavalcade::ctx::translate( translator::e_languages source, translator::e_languages target, const std::string& text, const std::string& suffix ) {
	std::thread translation_thread(
		[ this ]( translator::e_languages source, translator::e_languages target, std::string text, std::string suffix ) {
			nlohmann::json json;
			json[ "q" ]      = text;
			json[ "source" ] = translator::get_name_code( source );
			json[ "target" ] = translator::get_name_code( target );
			json[ "format" ] = "text";

			auto response = m_translator.Post( "/translate", json.dump( ), "application/json" );

			if ( response && response.error( ) == httplib::Error::Success && response.value( ).status != -1 ) {
				try {
					auto json = nlohmann::json::parse( response.value( ).body );

					if ( json.contains( "translatedText" ) ) {
						auto res = json[ "translatedText" ].get< std::string >( );

						// TODO: get color dynamically for all cases, currently its static
						auto formatted = io::format( "<<<NO_TRANSLATE>>> {}[<font color=\"{}\">{}</font>-><font color=\"{}\">{}</font>] {}",
					                                 suffix.empty( ) ? "" : " " + suffix + " ", "#0000FF", translator::get_human_name_code( source ),
					                                 "#00FF00", translator::get_human_name_code( target ), res );
						std::unique_lock< std::shared_mutex > lock( m_translations_mutex );
						m_pending_translations.push_back( formatted );
					}
				} catch ( const nlohmann::json::exception& e ) { g_io.log( "{}", e.what( ) ); }
			}
		},
		source, target, text, suffix );

	// run asynchronously
	translation_thread.detach( );
}