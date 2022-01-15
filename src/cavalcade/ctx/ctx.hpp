#pragma once

#include "../../other/lib/include/http/http.hpp"
#include "../../other/translator/translator.hpp"

#include "../../other/other.hpp"
#include "../../sdk/sdk.hpp"

#include <shared_mutex>

namespace cavalcade {
	enum flags
	{
		LOCAL    = ( 1 << 0 ),
		DEAD     = ( 1 << 1 ),
		DORMANT  = ( 1 << 2 ),
		TEAMMATE = ( 1 << 3 ),
		IMMUNE   = ( 1 << 4 )
	};

	struct ctx {
		ctx( );

		httplib::Client m_translator;
		bool m_translator_initialized;
		std::vector< std::string > m_pending_translations;
		std::shared_mutex m_translations_mutex;
		sdk::cs_player* m_local;
		sdk::user_cmd* m_cmd;

		struct {
			sdk::cvar* cl_updaterate;
			sdk::cvar* sv_minupdaterate;
			sdk::cvar* sv_maxupdaterate;
			sdk::cvar* viewmodel_offset_z;

			sdk::cvar* cl_interp;
			sdk::cvar* cl_interp_ratio;
			sdk::cvar* sv_client_min_interp_ratio;
			sdk::cvar* sv_client_max_interp_ratio;

			sdk::cvar* sv_maxunlag;

			sdk::cvar* sv_showlagcompensation_duration;

			sdk::cvar* sv_autobunnyhopping;

			sdk::cvar* mp_teammates_are_enemies;

			sdk::cvar* sensitivity;

			sdk::cvar* m_pitch;
			sdk::cvar* m_yaw;

		} m_cvars;

		struct {
			ISteamClient* m_steam_client;
			ISteamUser* m_steam_user;
			ISteamUserStats* m_steam_user_stats;
			ISteamFriends* m_steam_friends;
			ISteamGameCoordinator* m_steam_game_coordinator;
			ISteamMatchmaking* m_steam_matchmaking;
			ISteamUtils* m_steam_utils;
		} m_steam;

		bool init( );

		void iter_players( const std::function< void( sdk::cs_player* ) >& fn, i32 flags = 0 );

		void translate( translator::e_languages source, translator::e_languages target, const std::string& text, const std::string& suffix = "" );
	};
} // namespace cavalcade

inline cavalcade::ctx g_ctx;