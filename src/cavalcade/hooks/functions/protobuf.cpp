#include "../hooks.hpp"

bool cavalcade::hooks::protobuf::report_hit( uintptr_t* hit ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< report_hit_fn >( HASH_CT( "ReportHit" ) );

	struct hit_ {
		PAD( 8 );
		f32 x;
		f32 y;
		PAD( 4 );
		f32 z;
	};

	auto h = ( hit_* )hit;
	g_hack.m_hitmarker.push( hack::hitmark{ math::v3f( h->x, h->y, h->z ), g_csgo.m_globals->m_curtime } );
	return og( hit );
}