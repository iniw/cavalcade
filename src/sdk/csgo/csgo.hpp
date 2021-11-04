#pragma once

#include "../../other/mem/mem.hpp"
#include "../types/interfaces/interfaces.hpp"

namespace sdk {
	struct csgo {
	private:

		template< typename T = mem::address >
		T find_interface( mem::module_info& the_module, u32 name_hash, bool remove_numbers = true );

	public:

		interfaces::client* m_client;
		interfaces::engine* m_engine;
		interfaces::cvars* m_cvars;
		interfaces::ent_list* m_ent_list;
		interfaces::prediction* m_prediction;
		interfaces::mdl_cache* m_mdl_cache;
		interfaces::game_movement* m_game_movement;

		globals* m_globals;
		move_helper* m_move_helper;
		client_state* m_client_state;
		client_mode_shared* m_client_mode_shared;
		IDirect3DDevice9* m_d3d9_device;

		bool init( );
	};
} // namespace sdk

inline sdk::csgo g_csgo;

#include "csgo.inl"