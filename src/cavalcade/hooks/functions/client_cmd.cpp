#include "../hooks.hpp"
#include "../../../other/translator/translator.hpp"

void cavalcade::hooks::client_cmd_::client_cmd( unk ecx, unk edx, const char* cmd ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< client_cmd_fn >( HASH_CT( "CEngineClient::ClientCmd" ) );

	if ( g_ctx.m_translator_initialized ) {
		std::string_view text{ cmd };

		constexpr const char expected_suffix[] = "say \"/translate";

		// verify command
		if ( text.starts_with( expected_suffix ) && text.ends_with( "\"" ) && text.size( ) > ( sizeof( expected_suffix ) + 2 ) ) {
			auto new_range = std::string_view( text.begin( ) + sizeof( expected_suffix ), text.end( ) - 1 );

			auto first_language = new_range.find_first_not_of( ' ' );
			if ( first_language == std::string_view::npos || ( first_language + 5 ) >= ( sizeof( expected_suffix ) + 5 ) )
				return og( ecx, edx, cmd );
			auto continue_second_language = new_range.find_first_of( ' ', first_language );
			if ( continue_second_language == std::string_view::npos )
				return og( ecx, edx, cmd );

			auto second_language = new_range.find_first_not_of( ' ', continue_second_language );

			if ( second_language == std::string_view::npos )
				return og( ecx, edx, cmd );

			auto text_to_translate_pos_start = new_range.find_first_of( "\"" );
			auto text_to_translate_pos_end   = new_range.find_last_of( "\"" );
			// npos is -1
			// -1 - (-1) = 0
			if ( ( text_to_translate_pos_end - text_to_translate_pos_start ) <= 0 )
				return og( ecx, edx, cmd );

			// MY FLESH... .ITS BURNING.....
			std::string sz_first_language = std::string( new_range.begin( ) + first_language, new_range.begin( ) + second_language - 1 );

			std::string sz_second_language =
				std::string( new_range.begin( ) + second_language, new_range.begin( ) + text_to_translate_pos_start - 1 );

			std::ignore = std::remove( sz_first_language.begin( ), sz_first_language.end( ), ' ' );

			g_io.log( "{}", sz_first_language );

			auto source = translator::get_code_name( sz_first_language );
			if ( source == translator::e_languages::FAIL )
				return og( ecx, edx, cmd );

			std::ignore = std::remove( sz_second_language.begin( ), sz_second_language.end( ), ' ' );

			g_io.log( "{}", sz_second_language );

			auto target = translator::get_code_name( sz_second_language );
			if ( target == translator::e_languages::FAIL )
				return og( ecx, edx, cmd );

			auto translate = std::string( new_range.begin( ) + text_to_translate_pos_start + 1, new_range.begin( ) + text_to_translate_pos_end );

			g_io.log( "{}", translate );

			g_ctx.translate( source, target, translate, "[CMD]" );

			return;
		}
	}

	og( ecx, edx, cmd );
}