#include "../hooks.hpp"

void cavalcade::hooks::push_notice_::push_notice( unk ecx, unk edx, const char* text, int len, const char* normalizer ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< push_notice_fn >( HASH_CT( "PushNotice" ) );

	og( ecx, edx, text, len, normalizer );

	// lazy troubled first implementation
	// TODO: skip name (i want this to be done with my original smart caching idea which'll store all this playerinfo detail)
	// TODO: actually make fucking sense of stuff (what to translate ect)
	// TODO: wait for my friend to locally host this API so we can have non-ratelimited usage
	if ( !std::string_view( text ).starts_with( "<<<TRANSLATION>>>" ) ) {
		// text should be normalized now.

		std::thread translation_thread( [ & ]( ) {
			// this operation is not threaded so it's safe to use value if it's not nullptr thereafter
			std::stringstream shit;
			shit << "{\"q\":\"{";
			shit << text;
			shit << "}\",\"source\":\"";
			shit << "en";
			shit << "\",\"target\":\"";
			// irish
			shit << "ga";
			shit << "\",\"format\":\"text\"}";

			auto response = g_ctx.m_translator.Post( "/translate", shit.str( ), "application/json" );

			// explicit bool operator()
			if ( response && response.error( ) == httplib::Error::Success && response.value( ).status != -1 ) {
				// we might get cucked by api but i dont care for now
				auto json = response.value( ).body;
				// hack, doesn't expect error
				constexpr auto len = sizeof( "{\"translatedText\":" );
				auto first_split   = std::string_view( json.begin( ) + len, json.end( ) );
				auto it            = first_split.find_last_of( '\"' );
				auto res           = std::string( first_split.begin( ) + 1, first_split.begin( ) + it - 1 );
				g_io.log( "{}", res );
				std::unique_lock< std::shared_mutex > lock( g_ctx.m_translations_mutex );
				g_ctx.m_pending_translations.emplace_back( "<<<TRANSLATION>>>" + res );
			}
		} );

		// run independent
		translation_thread.detach( );
	}
}
