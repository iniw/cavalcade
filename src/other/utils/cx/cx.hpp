#ifndef CX_HPP
#define CX_HPP

#include "types/data.hpp"
#include "types/random_generator.hpp"
#include "types/string.hpp"

namespace utils::cx {
	constexpr inline u32 rng( u32 count = 11 );

	constexpr inline u32 strlen( cstr str );

	template< typename T >
	constexpr inline auto type_name( );
} // namespace utils::cx

#include "cx.inl"

#endif // CX_HPP