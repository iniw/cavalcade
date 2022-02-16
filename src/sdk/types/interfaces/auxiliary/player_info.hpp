

#pragma once

namespace sdk::auxiliary {
	struct player_info_t {
		uint64_t m_version;
		uint64_t m_steam_id;
		char m_name[ 128 ];
		int m_user_id;
		char m_guid[ 33 ];
		uint32_t m_friends_id;
		char m_friends_name[ 128 ];
		bool m_fake_player;
		bool m_is_hltv;
		int m_custom_files[ 4 ];
		uint8_t m_files_downloaded;
	};
} // namespace sdk::auxiliary