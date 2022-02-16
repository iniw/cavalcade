

#pragma once

#include "../../sdk/entities/player.hpp"

struct entity_cacher {
	entity_cacher( );

	void add( sdk::cs_player* p );
	void remove( sdk::cs_player* p );

	/// LOCAL IS ALSO IN THE LIST
	void for_each( std::function< void( sdk::player& ) >&& fn );

	void clear( );

	// private:

	std::vector< sdk::player > m_players;
};

inline entity_cacher g_entity_cacher;