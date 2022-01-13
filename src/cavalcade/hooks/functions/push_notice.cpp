#include "../hooks.hpp"
#include "../../../other/json/json.hpp"

void cavalcade::hooks::push_notice_::push_notice( unk ecx, unk edx, const char* text, int len, const char* normalizer ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< push_notice_fn >( HASH_CT( "PushNotice" ) );

	og( ecx, edx, text, len, normalizer );

	auto stext = std::string_view( text );

	// TODO: skip name (i want this to be done with my original smart caching idea which'll store all this playerinfo detail)
	// TODO: properly verify if the text is being translated
	if ( g_ctx.m_translator_initialized && !stext.starts_with( "<<<TRANSLATION>>>" ) ) {
		// text should be normalized now.

		std::thread translation_thread(
			[]( std::string text ) {
				nlohmann::json json;
				json[ "q" ]      = text;
				json[ "source" ] = "en";
				json[ "target" ] = "ga";
				json[ "format" ] = "text";

				auto response = g_ctx.m_translator.Post( "/translate", json.dump( ), "application/json" );

				if ( response && response.error( ) == httplib::Error::Success && response.value( ).status != -1 ) {
					try {
						auto json = nlohmann::json::parse( response.value( ).body );

						if ( json.contains( "translatedText" ) ) {
							auto res = json[ "translatedText" ].get< std::string >( );

							std::unique_lock< std::shared_mutex > lock( g_ctx.m_translations_mutex );
							g_ctx.m_pending_translations.emplace_back( "<<<TRANSLATION>>>" + res );
						}
					} catch ( const nlohmann::json::exception& e ) { g_io.log( "{}", e.what( ) ); }
				}
			},
			std::string( text ) );

		// run independent
		translation_thread.detach( );
	}
}
