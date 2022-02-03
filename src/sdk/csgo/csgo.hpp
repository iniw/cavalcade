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
		interfaces::model_info* m_model_info;
		interfaces::input* m_input;
		interfaces::debug_overlay* m_debug_overlay;
		interfaces::engine_trace* m_engine_trace;
		interfaces::physics_surface_props* m_physics_surface_props;
		interfaces::engine_sound* m_engine_sound;

		globals* m_globals;
		move_helper* m_move_helper;
		client_state* m_client_state;
		client_mode_shared* m_client_mode_shared;
		IDirect3DDevice9* m_d3d9_device;
		math::v3f* m_main_view_origin;

		std::optional< std::string > m_sky_name;

		bool init( );
	};
} // namespace sdk

inline sdk::csgo g_csgo;

#include "csgo.inl"