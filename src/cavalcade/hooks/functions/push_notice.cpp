#include "../hooks.hpp"

void cavalcade::hooks::push_notice_::push_notice( unk ecx, unk edx, cstr text, int len, cstr normalizer ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< push_notice_fn >( HASH_CT( "PushNotice" ) );

	// TODO: properly verify if the text is being translated
	auto stext = std::string_view( text );

	if ( !stext.starts_with( XOR( "<<<NO_TRANSLATE>>>" ) ) ) {
		if ( g_hack.m_translator.m_valid ) {
			g_hack.m_translator.translate( hack::translator::e_languages::AUTO, hack::translator::e_languages::RUSSIAN, std::string( text ) );
		}
	} else {
		// ensured
		auto en = std::string_view( stext.begin( ) + sizeof( "<<<NO_TRANSLATE>>>" ), stext.end( ) );
		return og( ecx, edx, en.data( ), en.size( ), en.data( ) );
	}

	og( ecx, edx, text, len, normalizer );
}