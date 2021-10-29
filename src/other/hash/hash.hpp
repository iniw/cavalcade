#ifndef HASH_HPP
#define HASH_HPP

#include "fnv1a/fnv1a.hpp"
#include "md5/md5.hpp"
#include "xxhash/xxhash.hpp"

#include "../utils/cx/cx.hpp"

namespace hash {
	constexpr inline u32 SEED = utils::cx::rng( );
} // namespace hash

#define HASH_RT( str ) hash::xxhash::gen( str, hash::SEED )
#define HASH_CT( str ) utils::cx::data< hash::xxhash::gen( str, hash::SEED ) >::value

#endif // HASH_HPP
