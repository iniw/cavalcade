#pragma once

namespace utils::cx {
	template< auto V >
	struct data {
		using T = decltype( V );

		constexpr inline static T value = V;
	};
} // namespace utils::cx