#pragma once

namespace utils::cx {
	constexpr inline u32 rng( u32 count = 11 );

	constexpr inline u32 strlen( cstr str );

	template< typename T >
	constexpr inline auto type_name( );
} // namespace utils::cx

#include "types/data.hpp"
#include "types/random_generator.hpp"
#include "types/string.hpp"

#include "cx.inl"