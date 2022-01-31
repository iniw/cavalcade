#pragma once

#include "../../other/other.hpp"
#include "../../sdk/sdk.hpp"

#include <shared_mutex>

namespace cavalcade {
	struct ctx {
		struct lua {
			void push( std::string_view code );
			std::vector< std::pair< sol::state, std::unordered_map< std::string, std::vector< std::function< void( ) > > > > > m_callbacks;
		} m_lua;
		sdk::player m_local;
		sdk::user_cmd* m_cmd;
		std::optional< CSteamID > m_last_friend_to_message;
		bool m_in_deathcam{ false };

		struct {
			sdk::cvar* sv_cheats;
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
			sdk::cvar* sv_skyname;

			sdk::cvar* sv_gravity;

			sdk::cvar* mp_teammates_are_enemies;

			sdk::cvar* sensitivity;
			sdk::cvar* zoom_sensitivity_ratio_mouse;

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
	};
} // namespace cavalcade

inline cavalcade::ctx g_ctx;