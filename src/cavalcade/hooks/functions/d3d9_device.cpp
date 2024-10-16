#include "../hooks.hpp"

HRESULT D3DAPI cavalcade::hooks::d3d9_device::end_scene( IDirect3DDevice9* device ) {
	static auto og = g_mem[ SHADERAPIDX9_DLL ].get_og< end_scene_fn >( HASH_CT( "IDirect3DDevice9::EndScene" ) );

	if ( g_hack.m_translator.m_valid && g_csgo.m_engine->is_in_game( ) && g_csgo.m_client_mode_shared->m_chat_element ) {
		std::unique_lock< std::shared_mutex > lock( g_hack.m_translator.m_translations_mutex );
		if ( !g_hack.m_translator.m_pending_translations.empty( ) ) {
			for ( const auto& entry : g_hack.m_translator.m_pending_translations ) {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0, XOR( "%s" ), entry.c_str( ) );
			}

			g_hack.m_translator.m_pending_translations.clear( );
		}
	}

	g_io.update_input( );

	g_render.begin( );

	g_gui.think( );

	g_gui.animate( );

	g_gui.render( );

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