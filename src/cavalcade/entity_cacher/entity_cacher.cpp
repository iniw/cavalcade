#include "entity_cacher.hpp"

#include "../ctx/ctx.hpp"

entity_cacher::entity_cacher( ) {
	m_players.reserve( 64 );
}

void entity_cacher::add( sdk::cs_player* p ) {
	if ( auto idx = g_csgo.m_engine->get_local_player_index( ); idx == p->get_networkable_index( ) )
		g_ctx.m_local = sdk::player( g_csgo.m_ent_list->get< sdk::cs_player* >( idx ) );

	if ( p && p->get_networkable_index( ) <= 64 && p->get_networkable_index( ) > 0 )
		m_players.push_back( p );
}

void entity_cacher::remove( sdk::cs_player* p ) {
	auto find = std::find( m_players.begin( ), m_players.end( ), p );
	if ( find != m_players.cend( ) && p->get_networkable_index( ) <= 64 && p->get_networkable_index( ) > 0 ) {
		m_players.erase( find );
	}
}

void entity_cacher::for_each( std::function< void( sdk::player& ) >&& fn ) {
	for ( auto& e : m_players ) {
		fn( e );
	}
}

void entity_cacher::clear( ) {
	m_players.clear( );
}