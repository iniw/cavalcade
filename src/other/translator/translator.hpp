#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#pragma once

namespace translator {
	enum e_languages
	{
		ENGLISH,
		ARABIC,
		AZERBAIJANI,
		CHINESE,
		CZECH,
		DUTCH,
		FINNISH,
		FRENCH,
		GERMAN,
		GREEK,
		HINDI,
		HUNGARIAN,
		INDONESIAN,
		IRISH,
		ITALIAN,
		JAPANESE,
		KOREAN,
		POLISH,
		PORTUGUESE,
		RUSSIAN,
		SPANISH,
		SWEDISH,
		TURKISH,
		UKRANIAN,
		VIETNAMESE,
		FAIL,
	};

	constexpr static auto get_name_code( e_languages lang ) {
		switch ( lang ) {
		case e_languages::ENGLISH:
			return "en";
		case e_languages::ARABIC:
			return "ar";
		case e_languages::AZERBAIJANI:
			return "az";
		case e_languages::CHINESE:
			return "zh";
		case e_languages::CZECH:
			return "cs";
		case e_languages::DUTCH:
			return "nl";
		case e_languages::FINNISH:
			return "fi";
		case e_languages::FRENCH:
			return "fr";
		case e_languages::GERMAN:
			return "de";
		case e_languages::GREEK:
			return "el";
		case e_languages::HINDI:
			return "hi";
		case e_languages::HUNGARIAN:
			return "hu";
		case e_languages::INDONESIAN:
			return "id";
		case e_languages::IRISH:
			return "ga";
		case e_languages::ITALIAN:
			return "it";
		case e_languages::JAPANESE:
			return "ja";
		case e_languages::KOREAN:
			return "ko";
		case e_languages::POLISH:
			return "pl";
		case e_languages::PORTUGUESE:
			return "pt";
		case e_languages::RUSSIAN:
			return "ru";
		case e_languages::SPANISH:
			return "es";
		case e_languages::SWEDISH:
			return "sv";
		case e_languages::TURKISH:
			return "tr";
		case e_languages::UKRANIAN:
			return "uk";
		case e_languages::VIETNAMESE:
			return "vi";
		}
	}

	constexpr static auto get_human_name_code( e_languages lang ) {
		switch ( lang ) {
		case e_languages::ENGLISH:
			return "English";
		case e_languages::ARABIC:
			return "Arabic";
		case e_languages::AZERBAIJANI:
			return "Azerbaijani";
		case e_languages::CHINESE:
			return "Chinese";
		case e_languages::CZECH:
			return "Czech";
		case e_languages::DUTCH:
			return "Dutch";
		case e_languages::FINNISH:
			return "Finnish";
		case e_languages::FRENCH:
			return "French";
		case e_languages::GERMAN:
			return "German";
		case e_languages::GREEK:
			return "Greek";
		case e_languages::HINDI:
			return "Hindi";
		case e_languages::HUNGARIAN:
			return "Hungarian";
		case e_languages::INDONESIAN:
			return "Indonesian";
		case e_languages::IRISH:
			return "Irish";
		case e_languages::ITALIAN:
			return "Italian";
		case e_languages::JAPANESE:
			return "Japanese";
		case e_languages::KOREAN:
			return "Korean";
		case e_languages::POLISH:
			return "Polish";
		case e_languages::PORTUGUESE:
			return "Portuguese";
		case e_languages::RUSSIAN:
			return "Russian";
		case e_languages::SPANISH:
			return "Spanish";
		case e_languages::SWEDISH:
			return "Swedish";
		case e_languages::TURKISH:
			return "Turkish";
		case e_languages::UKRANIAN:
			return "Ukranian";
		case e_languages::VIETNAMESE:
			return "Vietnamese";
		}
	}

	inline static auto get_code_name( const std::string_view lang ) {
		switch ( HASH_RT( lang ) ) {
		case HASH_CT( "en" ):
			return e_languages::ENGLISH;
		case HASH_CT( "ar" ):
			return e_languages::ARABIC;
		case HASH_CT( "az" ):
			return e_languages::AZERBAIJANI;
		case HASH_CT( "zh" ):
			return e_languages::CHINESE;
		case HASH_CT( "cs" ):
			return e_languages::CZECH;
		case HASH_CT( "nl" ):
			return e_languages::DUTCH;
		case HASH_CT( "fi" ):
			return e_languages::FINNISH;
		case HASH_CT( "fr" ):
			return e_languages::FRENCH;
		case HASH_CT( "de" ):
			return e_languages::GERMAN;
		case HASH_CT( "el" ):
			return e_languages::GREEK;
		case HASH_CT( "hi" ):
			return e_languages::HINDI;
		case HASH_CT( "hu" ):
			return e_languages::HUNGARIAN;
		case HASH_CT( "id" ):
			return e_languages::INDONESIAN;
		case HASH_CT( "ga" ):
			return e_languages::IRISH;
		case HASH_CT( "it" ):
			return e_languages::ITALIAN;
		case HASH_CT( "ja" ):
			return e_languages::JAPANESE;
		case HASH_CT( "ko" ):
			return e_languages::KOREAN;
		case HASH_CT( "pl" ):
			return e_languages::POLISH;
		case HASH_CT( "pt" ):
			return e_languages::PORTUGUESE;
		case HASH_CT( "ru" ):
			return e_languages::RUSSIAN;
		case HASH_CT( "es" ):
			return e_languages::SPANISH;
		case HASH_CT( "sv" ):
			return e_languages::SWEDISH;
		case HASH_CT( "tr" ):
			return e_languages::TURKISH;
		case HASH_CT( "uk" ):
			return e_languages::UKRANIAN;
		case HASH_CT( "vi" ):
			return e_languages::VIETNAMESE;
		}

		return e_languages::FAIL;
	}
} // namespace translator

#endif /* TRANSLATOR_HPP */
