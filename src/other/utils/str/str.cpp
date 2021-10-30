#include "str.hpp"

std::string utils::str::wide_to_multi_byte( std::wstring_view str ) {
	auto length = LI_FN( WideCharToMultiByte )( CP_UTF8, 0, str.data( ), str.size( ), nullptr, 0, nullptr, nullptr );

	auto result = std::string( length, 0 );

	LI_FN( WideCharToMultiByte )
	( CP_UTF8, 0, str.data( ), str.size( ), &result[ 0 ], length, nullptr, nullptr );

	return result;
}

std::wstring utils::str::multi_byte_to_wide( std::string_view str ) {
	auto length = LI_FN( MultiByteToWideChar )( CP_UTF8, 0, str.data( ), str.size( ), nullptr, 0 );

	auto result = std::wstring( length, 0 );

	LI_FN( MultiByteToWideChar )( CP_UTF8, 0, str.data( ), str.size( ), &result[ 0 ], length );

	return result;
}