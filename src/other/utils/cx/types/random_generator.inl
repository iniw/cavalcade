#pragma once

constexpr u64 utils::cx::random_generator::gen_seed( ) {
	u64 shifted = 0;

	for ( const auto c : __TIME__ ) {
		shifted <<= 8;
		shifted |= c;
	}

	return shifted;
}

constexpr u32 utils::cx::random_generator::next( ) {
	u64 oldstate = m_state;

	m_state = oldstate * MAGIC + ( m_seed | 1 );

	u32 xorshifted = ( ( oldstate >> 18u ) ^ oldstate ) >> 27u;
	u32 rot        = oldstate >> 59u;

	return ( xorshifted >> rot ) | ( xorshifted << ( ( -rot ) & 31 ) );
}