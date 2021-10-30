#ifndef STR_HPP
#define STR_HPP

namespace utils::str {
	std::string wide_to_multi_byte( std::wstring_view str );

	std::wstring multi_byte_to_wide( std::string_view str );

	template< typename T >
	inline T to_lower( T&& str );

	template< typename T >
	inline T to_upper( T&& str );
} // namespace utils::str

#include "str.inl"

#endif // STR_HPP