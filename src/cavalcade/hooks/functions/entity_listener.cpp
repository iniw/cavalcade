#include "../hooks.hpp"

void cavalcade::hooks::entity_listener::on_add_entity( unk ecx, unk edx, unk arg1, sdk::handle handle ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< fn >( HASH_CT( "CEntityListener::OnAddEntity" ) );
	og( ecx, edx, arg1, handle );
	auto p = g_csgo.m_ent_list->get_handle< sdk::cs_player* >( handle );
	g_entity_cacher.add( p );
}

void cavalcade::hooks::entity_listener::on_remove_entity( unk ecx, unk edx, unk arg1, sdk::handle handle ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< fn >( HASH_CT( "CEntityListener::OnRemoveEntity" ) );
	og( ecx, edx, arg1, handle );
	auto p = g_csgo.m_ent_list->get_handle< sdk::cs_player* >( handle );
	if ( auto f = g_hack.m_backtrack.m_records.find( p->get_networkable_index( ) ); f != g_hack.m_backtrack.m_records.cend( ) )
		g_hack.m_backtrack.m_records.erase( f );
	g_entity_cacher.remove( p );
}