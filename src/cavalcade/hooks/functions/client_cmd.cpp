#include "../hooks.hpp"

void cavalcade::hooks::client_cmd_::client_cmd( unk ecx, unk edx, const char* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< client_cmd_fn >( HASH_CT( "CEngineClient::ClientCmd" ) );

	std::string_view text{ cmd };
	if ( g_hack.m_translator.m_valid ) {
		// verify command
		if ( text.starts_with( XOR( "say \"/translate" ) ) && text.ends_with( XOR( "\"" ) ) && text.size( ) > ( sizeof( "say \"/translate" ) + 2 ) ) {
			auto new_range = std::string_view( text.begin( ) + sizeof( "say \"/translate" ), text.end( ) - 1 );

			auto first_language = new_range.find_first_not_of( ' ' );
			if ( first_language == std::string_view::npos || ( first_language + 5 ) >= ( sizeof( "say \"/translate" ) + 5 ) )
				return og( ecx, edx, cmd );
			auto continue_second_language = new_range.find_first_of( ' ', first_language );
			if ( continue_second_language == std::string_view::npos )
				return og( ecx, edx, cmd );

			auto second_language = new_range.find_first_not_of( ' ', continue_second_language );

			if ( second_language == std::string_view::npos )
				return og( ecx, edx, cmd );

			auto text_to_translate_pos_start = new_range.find_first_of( XOR( "\"" ) );
			auto text_to_translate_pos_end   = new_range.find_last_of( XOR( "\"" ) );
			// npos is -1
			// -1 - (-1) = 0
			if ( ( text_to_translate_pos_end - text_to_translate_pos_start ) <= 0 )
				return og( ecx, edx, cmd );

			// MY FLESH... .ITS BURNING.....
			std::string sz_first_language = std::string( new_range.begin( ) + first_language, new_range.begin( ) + second_language - 1 );

			std::string sz_second_language =
				std::string( new_range.begin( ) + second_language, new_range.begin( ) + text_to_translate_pos_start - 1 );

			std::ignore = std::remove( sz_first_language.begin( ), sz_first_language.end( ), ' ' );

			g_io.log( XOR( "{}" ), sz_first_language );

			auto source = hack::translator::get_code_name( sz_first_language );
			if ( source == hack::translator::e_languages::FAIL )
				return og( ecx, edx, cmd );

			std::ignore = std::remove( sz_second_language.begin( ), sz_second_language.end( ), ' ' );

			g_io.log( XOR( "{}" ), sz_second_language );

			auto target = hack::translator::get_code_name( sz_second_language );
			if ( target == hack::translator::e_languages::FAIL )
				return og( ecx, edx, cmd );

			auto translate = std::string( new_range.begin( ) + text_to_translate_pos_start + 1, new_range.begin( ) + text_to_translate_pos_end );

			g_io.log( XOR( "{}" ), translate );

			g_hack.m_translator.translate( source, target, translate, XOR( "<font color=\"#B9B9B9\">cmd |</font>" ) );

			return;
		}
	}

	if ( text.starts_with( XOR( "say \"/reply" ) ) && text.ends_with( XOR( "\"" ) ) && text.size( ) > sizeof( "say \"/reply" ) + 1 ) {
		// We run this logic regardless because we don't want to impair unrelated messages
		const std::string_view split = std::string_view( text.begin( ) + sizeof( "say \"/reply" ), text.end( ) );

		if ( g_ctx.m_last_friend_to_message.has_value( ) ) {
			std::string message = std::string( split.begin( ), split.end( ) - 1 );
			g_ctx.m_steam.m_steam_friends->ReplyToFriendMessage( g_ctx.m_last_friend_to_message.value( ), message.c_str( ) );
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				XOR( "<<<NO_TRANSLATE>>> <font color=\"#00FF00\">friend</font> <font color=\"#B9B9B9\"> | replied: \"%s\" to </font>*<font "
			         "color=\"#00FF00\">%s</font>*" ),
				message.c_str( ), g_ctx.m_steam.m_steam_friends->GetFriendPersonaName( g_ctx.m_last_friend_to_message.value( ) ) );
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf( 0, 0,
			                                                          XOR( "<<<NO_TRANSLATE>>> <font color=\"#00FF00\">friend</font> <font "
			                                                               "color=\"#B9B9B9\"> | no messages received in this session yet</font>" ) );
		}

		return;
	}

	og( ecx, edx, cmd );
}