#ifndef DATA_HPP
#define DATA_HPP

namespace utils::cx {
	template< auto V >
	struct data {
		using T = decltype( V );

		constexpr inline static T value = V;
	};
} // namespace utils::cx

#endif // DATA_HPP
