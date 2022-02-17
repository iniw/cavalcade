#include "../hooks.hpp"

int __cdecl cavalcade::hooks::panorama::radar_init( int a1, int a2 ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< radar_init_fn >( HASH_CT( "CRadar::Initialize" ) );
	auto panel     = ( sdk::interfaces::panel_2d* )og( a1, a2 );

	// NOTE(para): no't work yet...

	// NOTE(para): every game init there's a fresh csgo hud
	g_csgo.m_csgo_radar = panel;

	auto a = panel->m_panel->load_layout_from_string( R"(
	<root>
        <CSGOHudRadar class="hud-radar" hittest="false">
        </CSGOHudRadar>
    </root>
    )",
	                                                  true, true );

	g_csgo.m_cvars->console_color_printf( render::color( 255, 0, 255, 255 ),
	                                      io::format( "huauasuiashuiasuihuiashuiash {} - - \n", ( int )a ).c_str( ) );

	return ( int )panel;
}