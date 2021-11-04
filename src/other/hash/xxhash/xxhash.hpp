#pragma once

namespace hash {
	struct xxhash {
		constexpr static u32 gen( std::string_view input, u32 seed );

	private:

		constexpr static u32 round( u32 acc, std::string_view input, i32 pos );

		constexpr static u32 digest( std::string_view input, i32 pos, u32 v1, u32 v2, u32 v3, u32 v4 );

		constexpr static u32 finalize( std::string_view input, i32 pos, u32 h32 );

		constexpr static u32 avalanche( u32 h32 );

		constexpr static inline u32 prime1 = 0x9E3779B1u;
		constexpr static inline u32 prime2 = 0x85EBCA77u;
		constexpr static inline u32 prime3 = 0xC2B2AE3Du;
		constexpr static inline u32 prime4 = 0x27D4EB2Fu;
		constexpr static inline u32 prime5 = 0x165667B1u;
		struct detail {
			constexpr static u8 read_u8( cstr input, i32 pos );

			constexpr static u32 read_u32le( cstr input, i32 pos );
		};
	};
} // namespace hash

#include "xxhash.inl"