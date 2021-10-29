#ifndef XXHASH_INL
#define XXHASH_INL

constexpr u8 hash::xxhash::detail::read_u8( cstr input, i32 pos ) {
	return static_cast< u8 >( input[ pos ] );
}

constexpr u32 hash::xxhash::detail::read_u32le( cstr input, i32 pos ) {
	const u32 b0 = read_u8( input, pos + 0 );
	const u32 b1 = read_u8( input, pos + 1 );
	const u32 b2 = read_u8( input, pos + 2 );
	const u32 b3 = read_u8( input, pos + 3 );

	return b0 | ( b1 << 8 ) | ( b2 << 16 ) | ( b3 << 24 );
}

constexpr u32 hash::xxhash::avalanche( u32 h32 ) {
	h32 ^= h32 >> 15;
	h32 *= prime2;
	h32 ^= h32 >> 13;
	h32 *= prime3;
	h32 ^= h32 >> 16;

	return h32;
}

constexpr u32 hash::xxhash::finalize( std::string_view input, i32 pos, u32 h32 ) {
	// XXH_PROCESS4
	while ( ( input.length( ) - pos ) >= 4 ) {
		h32 += detail::read_u32le( input.data( ), pos ) * prime3;
		h32 = std::rotl( h32, 17 ) * prime4;
		pos += 4;
	}

	// XXH_PROCESS1
	while ( ( input.length( ) - pos ) > 0 ) {
		h32 += detail::read_u8( input.data( ), pos ) * prime5;
		h32 = std::rotl( h32, 11 ) * prime1;
		pos += 1;
	}

	return h32;
}

constexpr u32 hash::xxhash::digest( std::string_view input, i32 pos, u32 v1, u32 v2, u32 v3, u32 v4 ) {
	u32 h32 = 0;

	if ( input.length( ) >= 16 )
		h32 = std::rotl( v1, 1 ) + std::rotl( v2, 7 ) + std::rotl( v3, 12 ) + std::rotl( v4, 18 );
	else
		h32 = v3 + prime5;

	h32 += input.length( );
	h32 = finalize( input, pos, h32 );

	return avalanche( h32 );
}

constexpr u32 hash::xxhash::round( u32 acc, std::string_view input, i32 pos ) {
	const u32 d = detail::read_u32le( input.data( ), pos );
	acc += d * prime2;
	acc = std::rotl( acc, 13 ) * prime1;
	return acc;
}

constexpr u32 hash::xxhash::gen( std::string_view input, u32 seed ) {
	u32 v1  = seed + prime1 + prime2;
	u32 v2  = seed + prime2;
	u32 v3  = seed;
	u32 v4  = seed - prime1;
	i32 pos = 0;

	while ( pos + 16 <= input.length( ) ) {
		v1 = round( v1, input, pos + 0 * 4 );
		v2 = round( v2, input, pos + 1 * 4 );
		v3 = round( v3, input, pos + 2 * 4 );
		v4 = round( v4, input, pos + 3 * 4 );
		pos += 16;
	}

	return digest( input, pos, v1, v2, v3, v4 );
}

#endif // XXHASH_INL
