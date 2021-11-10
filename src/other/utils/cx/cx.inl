#pragma once

constexpr inline u32 utils::cx::rng( u32 count ) {
	random_generator gen;

	while ( count ) {
		gen.next( );
		--count;
	}

	return gen.next( );
}

constexpr inline u32 utils::cx::strlen( cstr str ) {
	u32 i = 0;
	while ( str[ i ] != 0 ) {
		++i;
	}
	return i;
}

template< typename T >
constexpr inline auto utils::cx::type_name( ) {
	constexpr std::string_view fsig = __FUNCSIG__;

	constexpr u32 end   = fsig.rfind( '>' ) - 1;
	constexpr u32 start = fsig.rfind( '<' ) + 1;

	constexpr u32 len = end - start + 1;

	string< len + 1 > ret{ };
	for ( u32 i = 0; i < len; i++ )
		ret.m_data[ i ] = fsig[ start + i ];

	ret.m_data[ len ] = '\0';

	return ret;
}