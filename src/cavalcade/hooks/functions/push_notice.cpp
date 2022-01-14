#include "../hooks.hpp"
#include "../../../other/translator/translator.hpp"

void cavalcade::hooks::push_notice_::push_notice( unk ecx, unk edx, const char* text, int len, const char* normalizer ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< push_notice_fn >( HASH_CT( "PushNotice" ) );

	// TODO: skip name (i want this to be done with my original smart caching idea which'll store all this playerinfo detail)
	// TODO: properly verify if the text is being translated
	auto stext = std::string_view( text );

	if ( !stext.starts_with( "<<<NO_TRANSLATE>>>" ) ) {
		if ( g_ctx.m_translator_initialized ) {
			g_ctx.translate( translator::e_languages::ENGLISH, translator::e_languages::RUSSIAN, std::string( text ) );
		}
	} else {
		// ensured
		auto en = std::string_view( stext.begin( ) + sizeof( "<<<NO_TRANSLATE>>>" ), stext.end( ) );
		return og( ecx, edx, en.data( ), en.size( ), en.data( ) );
	}

	og( ecx, edx, text, len, normalizer );
}
