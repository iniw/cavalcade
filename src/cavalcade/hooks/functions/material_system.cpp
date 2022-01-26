#include "../hooks.hpp"

void cavalcade::hooks::material_system::get_color_modulation( sdk::material* ecx, unk edx, f32& r, f32& g, f32& b ) {
	static auto og = g_mem[ MATERIALSYSTEM_DLL ].get_og< get_color_modulation_fn >( HASH_CT( "GetColorModulation" ) );
	og( ecx, edx, std::ref( r ), std::ref( g ), std::ref( b ) );

	g_hack.m_nightmode.run( ecx, r, g, b );
}

unk cavalcade::hooks::material_system::find_material( unk ecx, unk edx, const char* name, const char* group, i32 arg, i32 arg2 ) {
	static auto og = g_mem[ MATERIALSYSTEM_DLL ].get_og< find_material_fn >( HASH_CT( "GetColorModulation" ) );

	if ( g_csgo.m_sky_name.has_value( ) && std::string_view{ name }.find( g_csgo.m_sky_name.value( ) ) != std::string_view::npos ) {
		g_io.log( XOR( "{} {}" ), name, group );
		return og( ecx, edx, name, group, arg, arg2 );
	}
	return og( ecx, edx, name, group, arg, arg2 );
}