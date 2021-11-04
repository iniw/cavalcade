#pragma once

namespace utils::cx {
	struct random_generator {
	private:

		constexpr inline static u64 MAGIC = 6364136223846793005ull;

		u64 m_state;
		u64 m_seed;

		constexpr u64 gen_seed( );

	public:

		constexpr random_generator( ) : m_state{ 0 }, m_seed{ gen_seed( ) } { }

		constexpr u32 next( );
	};
} // namespace utils::cx

#include "random_generator.inl"