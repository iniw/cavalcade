#pragma once

namespace render {
	struct size {
		i32 w, h;

		constexpr size( ) = default;

		constexpr size( i32 w, i32 h ) : w{ w }, h{ h } { }

		constexpr size expand( i32 amt ) const {
			size tmp = *this;
			tmp.w += amt;
			tmp.h += amt;
			return tmp;
		}

		constexpr size shrink( i32 amt ) const {
			return expand( -amt );
		}

		constexpr bool is_zero( ) const {
			return !w && !h;
		}

		constexpr i32 operator[]( i32 idx ) {
			return ( ( i32* )this )[ idx ];
		}

		constexpr const i32 operator[]( i32 idx ) const {
			return ( ( i32* )this )[ idx ];
		}

		constexpr size& operator+=( const size& v ) {
			w += v.w;
			h += v.h;
			return *this;
		}

		constexpr size operator+( const size& v ) const {
			auto tmp = *this;
			tmp += v;
			return tmp;
		}

		constexpr size& operator-=( const size& v ) {
			w -= v.w;
			h -= v.h;
			return *this;
		}

		constexpr size operator-( const size& v ) const {
			auto tmp = *this;
			tmp -= v;
			return tmp;
		}

		constexpr size& operator*=( const f32& amt ) {
			w = w * amt;
			h = h * amt;
			return *this;
		}

		constexpr size operator*( const f32& amt ) const {
			auto tmp = *this;
			tmp *= amt;
			return tmp;
		}

		constexpr size& operator/=( const f32& amt ) {
			w = w / amt;
			h = h / amt;
			return *this;
		}

		constexpr size operator/( const f32& amt ) const {
			auto tmp = *this;
			tmp /= amt;
			return tmp;
		}

		constexpr bool operator==( const size& v ) {
			return w == v.w && h == v.h;
		}
	};
} // namespace render