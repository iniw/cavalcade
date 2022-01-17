#include "translator.hpp"

#include "../../../other/lib/include/json/json.hpp"

hack::translator::translator( ) : m_translator( "http://localhost:5000" ) {
	m_valid = m_translator.is_valid( );
}

void hack::translator::translate( e_languages source, e_languages target, const std::string& text, const std::string& suffix ) {
	std::thread translation_thread(
		[ this ]( translator::e_languages source, translator::e_languages target, std::string text, std::string suffix ) {
			nlohmann::json json;
			json[ "q" ]      = text;
			json[ "source" ] = get_name_code( source );
			json[ "target" ] = get_name_code( target );
			json[ "format" ] = "text";

			auto response = m_translator.Post( "/translate", json.dump( ), "application/json" );

			if ( response && response.error( ) == httplib::Error::Success && response.value( ).status != -1 ) {
				try {
					auto json = nlohmann::json::parse( response.value( ).body );

					if ( json.contains( "translatedText" ) ) {
						auto res = json[ "translatedText" ].get< std::string >( );

						// TODO: get color dynamically for all cases, currently its static
						auto formatted = io::format( "<<<NO_TRANSLATE>>> {}[<font color=\"{}\">{}</font>-><font color=\"{}\">{}</font>] {}",
					                                 suffix.empty( ) ? "" : " " + suffix + " ", "#0000FF", get_human_name_code( source ), "#00FF00",
					                                 get_human_name_code( target ), res );
						std::unique_lock< std::shared_mutex > lock( m_translations_mutex );
						m_pending_translations.push_back( formatted );
					}
				} catch ( const nlohmann::json::exception& e ) { g_io.log( "{}", e.what( ) ); }
			}
		},
		source, target, text, suffix );

	// run asynchronously
	translation_thread.detach( );
}