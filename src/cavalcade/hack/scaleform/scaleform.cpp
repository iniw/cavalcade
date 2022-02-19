#include "../hack.hpp"
#include "scaleform.hpp"

#include "../../ctx/ctx.hpp"
#include "../../gui/cfg/cfg.hpp"
#include <fstream>

auto get_panel( u32 _hash ) {
	auto engine = g_csgo.m_panorama->access_ui_engine( );
	auto panel  = engine->get_last_dispatched_event_target_panel( );

	auto itr                       = panel;
	sdk::interfaces::ui_panel* ret = nullptr;

	while ( itr && engine->is_valid_panel_pointer( itr ) ) {
		auto hash = HASH_RT( itr->get_id( ) );
		if ( hash == _hash ) {
			ret = itr;
			break;
		}
		itr = itr->get_parent( );
	}

	return ret;
}

#define DEBUG_SCALEFORM 1

constexpr const char* scaleformy =
#include "scaleform.txt"
	;

// constexpr const char* frame =
// #include "frame.txt"
// 	;

constexpr const char* radars[] =
#include "radar.txt"
	;

void hack::scaleform::install( ) {
	m_focus                = get_panel( HASH_CT( "CSGOHud" ) );
	static auto& scaleform = gui::cfg::get< bool >( HASH_CT( "main:group1:scaleform" ) );
	if ( !scaleform )
		return;

	g_csgo.m_panorama->access_ui_engine( )->run_script( m_focus, scaleformy, "panorama/layout/hud/hud.xml", 8, 10, false );
	auto color = std::max( 0, std::min( 11, g_ctx.m_cvars.cl_hud_color->get_int( ) ) );
	g_csgo.m_panorama->access_ui_engine( )->run_script( m_focus, radars[ color ], "panorama/layout/hud/hud.xml", 8, 10, false );
	m_old_hud_color = color;
}

void hack::scaleform::update( ) {
	if ( !g_csgo.m_engine->is_in_game( ) || !m_focus )
		return;
	static auto& scaleform = gui::cfg::get< bool >( HASH_CT( "main:group1:scaleform" ) );
	if ( !scaleform )
		return;

	auto hud_color = std::max( 0, std::min( 11, g_ctx.m_cvars.cl_hud_color->get_int( ) ) );

	if ( m_old_hud_color != hud_color ) {
		g_csgo.m_panorama->access_ui_engine( )->run_script( m_focus, radars[ hud_color ], "panorama/layout/hud/hud.xml", 8, 10, false );
		m_old_hud_color = hud_color;
	}

	if ( !g_ctx.m_local )
		return;

		// NOTE(para): really bad idea....
		// g_csgo.m_panorama->access_ui_engine( )->run_script( m_focus, frame, "panorama/layout/hud/hud.xml", 8, 10, false );

#if DEBUG_SCALEFORM
	if ( g_io.key_state< io::key_state::DOWN >( 'L' ) ) {
		std::ifstream s( "E:\\scaleform.txt" );
		std::string __scaleform( ( std::istreambuf_iterator< char >( s ) ), std::istreambuf_iterator< char >( ) );
		s.close( );
		const char* _scaleform = __scaleform.c_str( );
		g_csgo.m_panorama->access_ui_engine( )->run_script( m_focus, _scaleform, "panorama/layout/hud/hud.xml", 8, 10, false );
	}
#endif
}

void hack::scaleform::reset( ) {
	m_focus = nullptr;
}