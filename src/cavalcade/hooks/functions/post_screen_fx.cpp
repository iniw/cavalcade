#include "../hooks.hpp"

void __fastcall cavalcade::hooks::fx::post_screen_fx( unk ecx, unk edx, unk view ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< post_screen_fn >( HASH_CT( "PostScreenFx" ) );

	if ( !g_csgo.m_engine->is_in_game( ) )
		return og( ecx, edx, view );

	if ( !g_ctx.m_local )
		return og( ecx, edx, view );

	static auto glow_mgr = ( sdk::glow_manager* )( *( uintptr_t* )( g_mem[ CLIENT_DLL ].get_address< uintptr_t >( HASH_CT( "GlowManager" ) ) + 3 ) );

	for ( auto i = 0; i < glow_mgr->m_size; ++i ) {
		sdk::glow_object_definition& obj = glow_mgr->m_objects[ i ];

		if ( obj.unused( ) )
			continue;

		auto ent = ( sdk::cs_player* )obj.m_entity;

		if ( !ent || ent->is_dormant( ) || ent->get_client_class( )->m_class_id != sdk::class_id::CS_PLAYER || !g_ctx.m_local.get( ).is_enemy( ent ) )
			continue;

		obj.m_full_bloom_render = false;
		obj.m_glow_style        = 0;
		obj.set( 1.F, 1.F, 1.f, 0.3F );
	}

	og( ecx, edx, view );
}