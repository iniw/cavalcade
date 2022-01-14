#include "../hooks.hpp"
#include "../../../other/translator/translator.hpp"

void cavalcade::hooks::steam_friend_message_t::on_friend_message( GameConnectedFriendChatMsg_t* callback ) {
	if ( !callback )
		return;

	if ( g_csgo.m_engine->is_in_game( ) && g_csgo.m_client_mode_shared->m_chat_element ) {
		auto friend_name = g_ctx.m_steam.m_steam_friends->GetFriendPersonaName( callback->m_steamIDUser );
		void* msg        = malloc( 4096 );
		auto chat_type   = EChatEntryType::k_EChatEntryTypeInvalid;

		g_ctx.m_steam.m_steam_friends->GetFriendMessage( callback->m_steamIDUser, callback->m_iMessageID, msg, 4096, &chat_type );
		if ( chat_type == EChatEntryType::k_EChatEntryTypeChatMsg ) {
			auto text = io::format( "*{}*: {}", friend_name, ( const char* )msg );

			// hack, we want prefix to translation
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0, "<<<NO_TRANSLATE>>>%s", text.c_str( ) );

			g_ctx.translate( translator::e_languages::ENGLISH, translator::e_languages::RUSSIAN, text, "[<font color=\"#00FF00\">FRIEND</font>]" );
		}

		free( msg );
	}
}