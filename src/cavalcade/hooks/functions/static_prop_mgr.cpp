#include "../hooks.hpp"

void cavalcade::hooks::static_prop_mgr::precache_lighting( unk ecx, unk edx ) {
	static auto og = g_mem[ ENGINE_DLL ].get_og< precache_lighting_fn >( HASH_CT( "StaticPropMgrPrecacheLighting" ) );

	auto v1  = ( uintptr_t* )( ecx );
	int size = v1[ 11 ];

	if ( size > 0 ) {
		int v2 = 0;
		do {
			auto v4   = v2 + v1[ 8 ];
			auto prop = ( sdk::static_prop* )( v4 );
			g_hack.m_nightmode.add( prop );

			v2 += 256;
			--size;
		} while ( size );
	}

	og( ecx, edx );
}