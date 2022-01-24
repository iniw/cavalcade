#include "../hooks.hpp"

void cavalcade::hooks::steam::on_friend_message( GameConnectedFriendChatMsg_t* callback ) {
	if ( !callback )
		return;

	if ( g_csgo.m_engine->is_in_game( ) && g_csgo.m_client_mode_shared->m_chat_element ) {
		auto friend_name = g_ctx.m_steam.m_steam_friends->GetFriendPersonaName( callback->m_steamIDUser );
		void* msg        = malloc( 4096 );
		auto chat_type   = EChatEntryType::k_EChatEntryTypeInvalid;

		g_ctx.m_steam.m_steam_friends->GetFriendMessage( callback->m_steamIDUser, callback->m_iMessageID, msg, 4096, &chat_type );

		if ( chat_type == EChatEntryType::k_EChatEntryTypeChatMsg ) {
			auto message_interp = std::string_view{ ( const char* )msg };

			std::optional< std::string > image = std::nullopt;
			// https://steamuserimages-a.akamaihd.net/ugc/1
			auto find  = message_interp.find( XOR( "https://steamuserimages-a.akamaihd.net/ugc/" ) );
			size_t end = 0;
			if ( find != std::string::npos ) {
				auto end = message_interp.find_first_of( ' ', find );
				if ( end > message_interp.size( ) ) {
					end = message_interp.size( );
				}

				auto make = std::string( message_interp.begin( ) + find, message_interp.begin( ) + end );

				if ( make.ends_with( XOR( "/" ) ) ) {
					image = make;
				}
			}

			g_ctx.m_last_friend_to_message = callback->m_steamIDUser;
			auto text =
				io::format( XOR( "<font color=\"#00FF00\">friend</font> <font color=\"#B9B9B9\"> | </font>*<font color=\"#00FF00\">{}</font>*: {}" ),
			                friend_name, ( const char* )msg );

			// hack, we want prefix to translation
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0, XOR( "<<<NO_TRANSLATE>>> %s" ), text.c_str( ) );

			if ( image.has_value( ) ) {
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
					0, 0,
					XOR( "<<<NO_TRANSLATE>>> <font color=\"#00FF00\">friend</font> <font color=\"#B9B9B9\"> | image from </font>*<font "
				         "color=\"#00FF00\">%s</font>*..." ),
					friend_name );
				auto fmt = io::format( XOR( "<<<NO_TRANSLATE>>> <img src=\"{}\"></img>" ), image.value( ) );
				g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0, fmt.c_str( ) );
			}

			auto ttext = io::format( XOR( "*{}*: {}" ), friend_name, ( const char* )msg );

			g_hack.m_translator.translate( hack::translator::e_languages::ENGLISH, hack::translator::e_languages::RUSSIAN, ttext,
			                               XOR( "<font color=\"#00FF00\">friend</font> <font color=\"#B9B9B9\"> |</font>" ) );
		}

		free( msg );
	}
}

// void cavalcade::hooks::steam::on_join_server( GSClientApprove_t* callback ) {
// 	if ( !callback )
// 		return;

// 	// verify if join ID == owner ID
// 	// "this will be different from m_SteamID if the game is being borrowed via Steam Family Sharing."
// 	// NOTE(para): this can also be done with engine checks
// 	g_ctx.m_trainer.m_on_dedicated_server = ( callback->m_SteamID == callback->m_OwnerSteamID );

// 	g_io.log( "here {}", g_ctx.m_trainer.m_on_dedicated_server );
// }