#pragma once

#include "../../other/other.hpp"
#include "../../sdk/sdk.hpp"

#include <shared_mutex>

namespace cavalcade {
	struct lua_impl {
		void push( std::string_view code );
		std::vector< std::pair< mutable sol::state, std::unordered_map< std::string, std::vector< sol::protected_function > > > > m_callbacks;
		// std::shared_mutex m_mutex;
	};

	struct ctx {
		sdk::player m_local;
		sdk::user_cmd* m_cmd;
		std::optional< CSteamID > m_last_friend_to_message;
		bool m_in_deathcam{ false };
		sdk::view m_view{ };

		struct {
			sdk::cvar* host_timescale;
			sdk::cvar* sv_cheats;
			sdk::cvar* cl_cmdrate;
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
			sdk::cvar* cl_hud_color;
			sdk::cvar* r_rainradius;

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
inline cavalcade::lua_impl g_lua;