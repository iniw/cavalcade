#include "../hooks.hpp"

HRESULT D3DAPI cavalcade::hooks::d3d9_device::end_scene( IDirect3DDevice9* device ) {
	static auto og = g_mem[ SHADERAPIDX9_DLL ].get_og< end_scene_fn >( HASH_CT( "IDirect3DDevice9::EndScene" ) );

	g_render.begin( );

	g_gui.render( );

	g_render.text< render::font::MENU >( { 0, 0 }, "test", render::color::white( ) );

	g_render.end( );

	return og( device );
}

HRESULT D3DAPI cavalcade::hooks::d3d9_device::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_params ) {
	static auto og = g_mem[ SHADERAPIDX9_DLL ].get_og< reset_fn >( HASH_CT( "IDirect3DDevice9::Reset" ) );

	g_render.pre_reset( );

	auto result = og( device, presentation_params );

	g_render.post_reset( );

	return result;
}