#include "../../cavalcade/ctx/ctx.hpp"

bool sdk::cs_player::is_enemy( sdk::base_player* player ) {
	if ( g_ctx.m_cvars.mp_teammates_are_enemies->get_int( ) )
		return this != player;

	return get_team( ) != player->get_team( );
}

void sdk::cs_player::post_think( ) {
	g_csgo.m_mdl_cache->begin_lock( );

	if ( is_alive( ) || is_player_ghost( ) ) { // cmp [edi+3A95h], al

		mem::call_v_func< void, 340 >( this ); // UpdateCollisionBounds

		if ( get_flags( ) & flags::ONGROUND )
			get_fall_velocity( ) = 0.f;

		if ( get_sequence( ) == -1 )
			mem::call_v_func< void, 219 >( this, 0 ); // SetSequence

		mem::call_v_func< void, 220 >( this ); // StudioFrameAdvance

		post_think_v_physics( );
	}

	simulate_player_simulated_entities( );

	g_csgo.m_mdl_cache->end_lock( );
}