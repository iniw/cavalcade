#pragma once

namespace math::easing {
	template< typename T = f32 >
	constexpr inline T in_cubic( T x ) {
		return x * x * x;
	}

	template< typename T = f32 >
	constexpr inline T in_quint( T x ) {
		T t2 = x * x;
		return x * t2 * t2;
	}

	template< typename T = f32 >
	constexpr inline T out_quint( T x ) {
		const T t2 = ( --x ) * x;
		return 1.f + x * t2 * t2;
	}
} // namespace math::easing