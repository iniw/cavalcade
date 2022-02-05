#include "translator.hpp"

#include "../../../other/lib/include/json/json.hpp"

hack::translator::translator( ) : m_translator( "http://localhost:5000" ) {
	m_valid = m_translator.is_valid( );
}

void hack::translator::translate( e_languages source, e_languages target, const std::string& text, const std::string& suffix ) {
	std::thread translation_thread(
		[ this ]( translator::e_languages source, translator::e_languages target, std::string text, std::string suffix ) {
			nlohmann::json json;
			json[ XOR( "q" ) ]      = text;
			json[ XOR( "source" ) ] = get_name_code( source );
			json[ XOR( "target" ) ] = get_name_code( target );
			json[ XOR( "format" ) ] = XOR( "text" );

			auto response = m_translator.Post( XOR( "/translate" ), json.dump( ), XOR( "application/json" ) );

			if ( response && response.error( ) == httplib::Error::Success && response.value( ).status != -1 ) {
				try {
					auto json = nlohmann::json::parse( response.value( ).body );

					if ( json.contains( XOR( "translatedText" ) ) ) {
						auto res = json[ XOR( "translatedText" ) ].get< std::string >( );

						// TODO: get color dynamically for all cases, currently its static
						auto tsource = std::string{ get_human_name_code( source ) };
						std::transform( tsource.begin( ), tsource.end( ), tsource.begin( ), ::tolower );
						auto ttarget = std::string{ get_human_name_code( target ) };
						std::transform( ttarget.begin( ), ttarget.end( ), ttarget.begin( ), ::tolower );
						auto formatted = io::format( XOR( "<<<NO_TRANSLATE>>> {}<font color=\"{}\">{}</font> <font color=\"#B9B9B9\">-></font> <font "
					                                      "color=\"{}\">{}</font> <font color=\"#B9B9B9\"> |</font> {}" ),
					                                 suffix.empty( ) ? XOR( "" ) : XOR( " " ) + suffix + XOR( " " ), "#00FF00", std::move( tsource ),
					                                 XOR( "#FFFF00" ), std::move( ttarget ), res );
						std::unique_lock< std::shared_mutex > lock( m_translations_mutex );
						m_pending_translations.push_back( formatted );
					}
				} catch ( ... ) { }
			}
		},
		source, target, text, suffix );

	// run asynchronously
	translation_thread.detach( );
}