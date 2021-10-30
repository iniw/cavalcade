#ifndef CSGO_HPP
#define CSGO_HPP

#include "../../other/mem/mem.hpp"
#include "../types/interfaces/interfaces.hpp"

namespace sdk {
	struct csgo {
	private:

		template< typename T = mem::address >
		T find_interface( mem::module_info& the_module, u32 name_hash, bool remove_numbers = true );

	public:

		interfaces::client_dll* m_client_dll;
		interfaces::engine* m_engine;
		interfaces::cvars* m_cvars;
		interfaces::entity_list* m_entity_list;

		globals* m_globals;

	public:

		bool init( );
	};
} // namespace sdk

inline sdk::csgo g_csgo;

#include "csgo.inl"

#endif // CSGO_HPP