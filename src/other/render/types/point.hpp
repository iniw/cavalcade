#pragma once

#include "size.hpp"

namespace render {
	struct rect;

	struct point {
	public:

		i32 x, y;

	public:

		constexpr point( ) = default;

		constexpr point( i32 x, i32 y ) : x{ x }, y{ y } { }

		explicit constexpr point( std::floating_point auto x, std::floating_point auto y )
			: x{ static_cast< i32 >( x ) }, y{ static_cast< i32 >( y ) } { }

	public:

		constexpr point offset( i32 offset ) {
			return { x + offset, y + offset };
		}

		constexpr point offset( i32 x_offset, i32 y_offset ) {
			return { x + x_offset, y + y_offset };
		}

		constexpr bool is_zero( ) const {
			return !x && !y;
		}

		bool in_rect( const rect& r ) const;

	public:

		constexpr i32& operator[]( i32 idx ) {
			return ( ( i32* )this )[ idx ];
		}

		constexpr const i32& operator[]( i32 idx ) const {
			return ( ( i32* )this )[ idx ];
		}

		constexpr point& operator+=( const point& v ) {
			x += v.x;
			y += v.y;
			return *this;
		}

		constexpr point operator+( const point& v ) const {
			auto tmp = *this;
			tmp += v;
			return tmp;
		}

		constexpr point& operator-=( const point& v ) {
			x -= v.x;
			y -= v.y;
			return *this;
		}

		constexpr point operator-( const point& v ) const {
			auto tmp = *this;
			tmp -= v;
			return tmp;
		}

		constexpr point operator-( int v ) const {
			auto tmp = *this;
			tmp.x -= v;
			tmp.y -= v;
			return tmp;
		}

		constexpr point& operator+=( const size& v ) {
			x += v.w;
			y += v.h;
			return *this;
		}

		constexpr point operator+( const size& v ) const {
			auto tmp = *this;
			tmp += v;
			return tmp;
		}

		constexpr point operator+( int v ) const {
			auto tmp = *this;
			tmp.x += v;
			tmp.y += v;
			return tmp;
		}

		constexpr point& operator-=( const size& v ) {
			x -= v.w;
			y -= v.h;
			return *this;
		}

		constexpr point operator-( const size& v ) const {
			auto tmp = *this;
			tmp -= v;
			return tmp;
		}

		constexpr point& operator*=( f32 amt ) {
			x = x * amt;
			y = y * amt;
			return *this;
		}

		constexpr point operator*( f32 amt ) const {
			auto tmp = *this;
			tmp *= amt;
			return tmp;
		}

		constexpr point& operator/=( f32 amt ) {
			x = x / amt;
			y = y / amt;
			return *this;
		}

		constexpr point operator/( f32 amt ) const {
			auto tmp = *this;
			tmp /= amt;
			return tmp;
		}

		constexpr bool operator==( const point& v ) {
			return x == v.x && y == v.y;
		}
	};
} // namespace render