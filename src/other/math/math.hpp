#ifndef MATH_HPP
#define MATH_HPP

#include "easing/easing.hpp"

#include "types/angle.hpp"
#include "types/matrix_3x4.hpp"
#include "types/vector.hpp"

namespace math {
	constexpr inline f32 PI = std::numbers::pi_v< f32 >;
} // namespace math

#include "math.inl"

#endif // MATH_HPP
