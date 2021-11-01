#include "../hooks.hpp"

HRESULT D3DAPI cavalcade::hooks::d3d9_device::end_scene( IDirect3DDevice9* device ) {
	static auto og = g_mem[ SHADERAPIDX9_DLL ].get_og< end_scene_fn >( HASH_CT( "IDirect3DDevice9::EndScene" ) );

	g_render.begin( );

	g_render.rectangle( { 500, 500 }, { 100, 100 }, render::color::white( ), 1.f ).outline( render::color::black(  ) );

	auto size = g_render.text< render::font::MENU >( { 200, 200 }, "test", render::color::white( ) );

	g_render.rectangle( { 200, 200 }, size, render::color::red( ) ).outline( );

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