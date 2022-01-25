#include "trainer.hpp"

#include "../../ctx/ctx.hpp"
#include "../../gui/cfg/cfg.hpp"

void hack::trainer::go( ) {
	auto cheats = g_ctx.m_cvars.sv_cheats->get_int( ) >= 1;
	if ( cheats ) {
		if ( !m_checkpoints.empty( ) ) {
			auto n                  = std::min( m_checkpoints.size( ) - m_entry - 1, m_checkpoints.size( ) - 1 );
			auto [ origin, angles ] = m_checkpoints[ n ];
			// NOTE(para): rebuild this?
			auto fmt = io::format( XOR( "setpos_exact {} {} {};setang_exact {} {} {}" ), origin[ 0 ], origin[ 1 ], origin[ 2 ], angles.pitch,
			                       angles.yaw, angles.roll );

			// bruh.
			g_csgo.m_engine->execute_client_cmd( fmt.c_str( ) );
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font color=\"#B9B9B9\"> | failed teleporting to checkpoint (no "
			         "previous checkpoint)...</font>" ) );
			return;
		}
	} else {
		g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
			0, 0,
			XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font color=\"#B9B9B9\"> | failed teleporting to "
		         "checkpoint (</font><font color=\"#FF0000\">sv_cheats was 0</font><font color=\"#B9B9B9\">)...</font>" ) );
		return;
	}

	g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
		0, 0, XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font color=\"#B9B9B9\"> | teleported to checkpoint #%d...</font>" ),
		m_entry + 1 );
}

void hack::trainer::run( ) {
	static auto& tr = gui::cfg::get< bool >( HASH_CT( "main:group1:trainer" ) );
	if ( !tr )
		return;

	auto cheats = g_ctx.m_cvars.sv_cheats->get_int( ) >= 1;

	if ( g_io.key_state< io::key_state::RELEASED >( '2' ) )
		go( );

	if ( cheats ) {
		if ( g_io.key_state< io::key_state::RELEASED >( '1' ) ) {
			m_entry = 0;
			m_checkpoints.emplace_back( g_ctx.m_local.get( ).get_origin( ), g_ctx.m_local.get( ).get_abs_angles( ) );
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font "
			         "color=\"#B9B9B9\"> | pushed checkpoint... updated focus to #1 (%d total)</font>" ),
				m_checkpoints.size( ) );
		} else if ( g_io.key_state< io::key_state::RELEASED >( '4' ) ) {
			if ( ( m_entry + 1 ) < m_checkpoints.size( ) ) {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
					0, 0, XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font color=\"#B9B9B9\"> | updated focus to #%d</font>" ),
					++m_entry + 1 );
			} else {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
					0, 0,
					XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font "
				         "color=\"#B9B9B9\"> | not enough entries to update focus forward...</font>" ) );
			}
		} else if ( g_io.key_state< io::key_state::RELEASED >( '3' ) ) {
			if ( ( m_entry - 1 ) >= 0 ) {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
					0, 0, XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font color=\"#B9B9B9\"> | updated focus to #%d</font>" ),
					--m_entry + 1 );
			} else {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0,
				                                                          XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font "
				                                                               "color=\"#B9B9B9\"> | entry indice is already #1...</font>" ) );
			}
		} else if ( g_io.key_state< io::key_state::RELEASED >( '5' ) ) {
			if ( !m_checkpoints.empty( ) ) {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0,
				                                                          XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font "
				                                                               "color=\"#B9B9B9\"> | erased entry that was formerly #%d</font>" ),
				                                                          m_entry + 1 );

				m_checkpoints.erase( m_checkpoints.begin( ) + m_entry );

				if ( m_entry > ( m_checkpoints.size( ) - 1 ) ) {
					m_entry = ( m_checkpoints.size( ) - 1 );
				}
			} else {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
					0, 0,
					XOR( "<<<NO_TRANSLATE>>> <font color=\"#FF0000\">trainer</font> <font color=\"#B9B9B9\"> | no entries to erase...</font>" ) );
			}
		}
	}
}

void hack::trainer::clear( ) {
	m_entry = 0;
	m_checkpoints.clear( );
}