#ifndef STR_INL
#define STR_INL

template< typename T >
inline T utils::str::to_lower( T&& str ) {
	std::transform( str.begin( ), str.end( ), str.begin( ), tolower );
	return str;
}

template< typename T >
inline T utils::str::to_upper( T&& str ) {
	std::transform( str.begin( ), str.end( ), str.begin( ), toupper );
	return str;
}

#endif // STR_INL