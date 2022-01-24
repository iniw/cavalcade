#include "../hooks.hpp"

void cavalcade::hooks::material_system::get_color_modulation( sdk::material* ecx, unk edx, f32& r, f32& g, f32& b ) {
	static auto og = g_mem[ MATERIALSYSTEM_DLL ].get_og< get_color_modulation_fn >( HASH_CT( "GetColorModulation" ) );
	og( ecx, edx, std::ref( r ), std::ref( g ), std::ref( b ) );

	g_hack.m_nightmode.run( ecx, r, g, b );
}