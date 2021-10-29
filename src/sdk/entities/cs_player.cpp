#include "../../cavalcade/ctx/ctx.hpp"

bool sdk::cs_player::is_enemy( sdk::base_player* player ) {
	if ( g_ctx.m_cvars.mp_teammates_are_enemies->get_int( ) )
		return this != player;

	return get_team( ) != player->get_team( );
}