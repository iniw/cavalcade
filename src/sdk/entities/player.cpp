#include "player.hpp"

#include "../csgo/csgo.hpp"

sdk::player::player( sdk::cs_player* p ) : m_player( p ) {
	m_player_info = g_csgo.m_engine->get_player_info( get( ).get_networkable_index( ) );
}

sdk::cs_player& sdk::player::get( ) const {
	return *m_player;
}

const sdk::auxiliary::player_info_t& sdk::player::get_player_info( ) const {
	return m_player_info;
}

std::string_view sdk::player::get_name( ) const {
	return get_player_info( ).m_name;
}
