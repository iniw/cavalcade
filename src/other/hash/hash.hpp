#pragma once

#include "xxhash/xxhash.hpp"
#include "md5/md5.hpp"

#include "../utils/cx/cx.hpp"

namespace hash {
	constexpr inline u32 SEED = utils::cx::rng( );
} // namespace hash