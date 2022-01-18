#include "trainer.hpp"

#include "../../ctx/ctx.hpp"

void hack::trainer::go( ) {
	auto cheats = g_ctx.m_cvars.sv_cheats->get_int( ) == 1;
	if ( cheats ) {
		if ( m_checkpoint.has_value( ) ) {
			auto [ origin, angles ] = m_checkpoint.value( );
			// NOTE(para): rebuild this?
			auto fmt = io::format( "setpos_exact {} {} {};setang_exact {} {} {}", origin[ 0 ], origin[ 1 ], origin[ 2 ], angles.pitch, angles.yaw,
			                       angles.roll );

			// bruh.
			g_csgo.m_engine->execute_client_cmd( fmt.c_str( ) );
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				"<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Failed teleporting to checkpoint (<font "
				"color=\"#FF0000\">No previous checkpoint</font>)..." );
			return;
		}
	} else {
		g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0,
		                                                          "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Failed teleporting to "
		                                                          "checkpoint (<font color=\"#FF0000\">sv_cheats</font> was 0)..." );
		return;
	}

	g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
		0, 0, "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Teleported to checkpoint..." );
}

void hack::trainer::run( ) {
	if ( g_io.key_state< io::key_state::RELEASED >( VK_XBUTTON1 ) ) {
		m_checkpoint = { g_ctx.m_local.get( ).get_origin( ), g_ctx.m_local.get( ).get_abs_angles( ) };
		g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
			0, 0, "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Updated checkpoint..." );
	}
	if ( g_io.key_state< io::key_state::RELEASED >( VK_XBUTTON2 ) ) {
		go( );
	}
}