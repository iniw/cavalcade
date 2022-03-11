#include "../hooks.hpp"

void* cavalcade::hooks::vphysics::get_collide( unk ecx, unk edx, i32 model ) {
	static auto og = g_mem[ PHYSICS_DLL ].get_og< get_collide_fn >( HASH_CT( "GetVCollide" ) );

	if ( model == -1 )
		return &g_hack.m_weather.m_collide;

	return og( ecx, edx, model );
}