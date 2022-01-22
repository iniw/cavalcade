#include "trainer.hpp"

#include "../../ctx/ctx.hpp"

void hack::trainer::go( ) {
	auto cheats = g_ctx.m_cvars.sv_cheats->get_int( ) == 1;
	if ( cheats ) {
		if ( !m_checkpoints.empty( ) ) {
			auto n                  = std::min( m_checkpoints.size( ) - m_entry - 1, m_checkpoints.size( ) - 1 );
			auto [ origin, angles ] = m_checkpoints[ n ];
			// NOTE(para): rebuild this?
			auto fmt = io::format( _( "setpos_exact {} {} {};setang_exact {} {} {}" ), origin[ 0 ], origin[ 1 ], origin[ 2 ], angles.pitch, angles.yaw,
			                       angles.roll );

			// bruh.
			g_csgo.m_engine->execute_client_cmd( fmt.c_str( ) );
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				_( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Failed teleporting to checkpoint (<font "
			       "color=\"#FF0000\">No previous checkpoint</font>)..." ) );
			return;
		}
	} else {
		g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
			0, 0,
			_( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Failed teleporting to "
		       "checkpoint (<font color=\"#FF0000\">sv_cheats</font> was 0)..." ) );
		return;
	}

	g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
		0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Teleported to checkpoint #%d..." ), m_entry + 1 );
}

void hack::trainer::run( ) {
	if ( g_io.key_state< io::key_state::RELEASED >( VK_XBUTTON1 ) ) {
		m_entry = 0;
		m_checkpoints.emplace_back( g_ctx.m_local.get( ).get_origin( ), g_ctx.m_local.get( ).get_abs_angles( ) );
		g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
			0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Pushed checkpoint... updated focus to #1 (%d total)" ),
			m_checkpoints.size( ) );
	} else if ( g_io.key_state< io::key_state::RELEASED >( VK_XBUTTON2 ) ) {
		go( );
	} else if ( g_io.key_state< io::key_state::RELEASED >( '4' ) ) {
		if ( ( m_entry + 1 ) < m_checkpoints.size( ) ) {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Updated focus to #%d" ), ++m_entry + 1 );
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Not enough entries to update focus forward..." ) );
		}
	} else if ( g_io.key_state< io::key_state::RELEASED >( '5' ) ) {
		if ( ( m_entry - 1 ) >= 0 ) {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Updated focus to #%d" ), --m_entry + 1 );
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Entry indice is already #1..." ) );
		}
	} else if ( g_io.key_state< io::key_state::RELEASED >( '6' ) ) {
		if ( !m_checkpoints.empty( ) ) {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] Erased entry that was formerly #%d" ), m_entry + 1 );

			m_checkpoints.erase( m_checkpoints.begin( ) + m_entry );

			if ( m_entry > ( m_checkpoints.size( ) - 1 ) ) {
				m_entry = ( m_checkpoints.size( ) - 1 );
			}
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, _( "<<<NO_TRANSLATE>>> [<font color=\"#FF0000\">TRAINER</font>] No entries to erase..." ) );
		}
	}
}

void hack::trainer::clear( ) {
	m_entry = 0;
	m_checkpoints.clear( );
}