#pragma once

namespace sdk::interfaces {
	struct steam_api_context_t {
		ISteamClient* m_steam_client;
		ISteamUser* m_steam_user;
		ISteamFriends* m_steam_friends;
		ISteamUtils* m_steam_utils;
		ISteamMatchmaking* m_steam_matchmaking;
		ISteamGameSearch* m_steam_game_search;
		ISteamMatchmakingServers* m_steam_matchmaking_servers;
		ISteamUserStats* m_steam_user_stats;
		ISteamApps* m_steam_apps;
		ISteamNetworking* m_steam_networking;
		ISteamRemoteStorage* m_steam_remote_storage;
		ISteamScreenshots* m_steam_screenshots;
		ISteamHTTP* m_steam_http;
		ISteamController* m_steam_controller;
		ISteamUGC* m_steam_ugc;
		ISteamAppList* m_steam_app_list;
		ISteamMusic* m_steam_music;
		ISteamMusicRemote* m_steam_music_remote;
		ISteamHTMLSurface* m_steam_html_surface;
		ISteamInventory* m_steam_inventory;
		ISteamVideo* m_steam_video;
		ISteamParentalSettings* m_steam_parental_settings;
		ISteamInput* m_steam_input;
	};

	struct engine {
	private:

		void get_screen_size( i32& w, i32& h ) {
			return mem::call_v_func< void, 5, i32&, i32& >( this, w, h );
		}

	public:

		bool is_in_game( ) {
			return mem::call_v_func< bool, 26 >( this );
		}

		math::v2i get_screen_size( ) {
			i32 w, h;
			get_screen_size( w, h );
			return math::v2i( w, h );
		}

		steam_api_context_t* get_steam_api_context( ) {
			return mem::call_v_func< steam_api_context_t*, 185 >( this );
		}
	};
} // namespace sdk::interfaces