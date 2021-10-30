#ifndef ANGLE_HPP
#define ANGLE_HPP

namespace math {
	struct ang {
		f32 pitch, yaw, roll;

		constexpr ang( f32 pitch = 0.f, f32 yaw = 0.f, f32 roll = 0.f ) : pitch{ pitch }, yaw{ yaw }, roll{ roll } { }

		constexpr ang( const f32* arr ) : pitch( arr[ 0 ] ), yaw( arr[ 1 ] ), roll( arr[ 2 ] ) { }

		constexpr ang( const ang& ) = default;

		constexpr auto operator<=>( const ang& ) const = default;

		constexpr ang& operator=( const ang& a ) = default;

		constexpr ang operator+( const ang& a ) const { return ang( pitch + a.pitch, yaw + a.yaw, roll + a.roll ); }

		constexpr ang operator+( const f32 fl ) const { return ang( pitch + fl, yaw + fl, roll + fl ); }

		constexpr ang& operator+=( const ang& a ) {
			pitch += a.pitch;
			yaw += a.yaw;
			roll += a.roll;

			return *this;
		}

		constexpr ang& operator+=( const f32 fl ) {
			pitch += fl;
			yaw += fl;
			roll += fl;

			return *this;
		}

		constexpr ang operator-( const ang& a ) const { return ang( pitch - a.pitch, yaw - a.yaw, roll - a.roll ); }

		constexpr ang operator-( const f32 fl ) const { return ang( pitch - fl, yaw - fl, roll - fl ); }

		constexpr ang& operator-=( const ang& a ) {
			pitch -= a.pitch;
			yaw -= a.yaw;
			roll -= a.roll;

			return *this;
		}

		constexpr ang& operator-=( const f32 fl ) {
			pitch -= fl;
			yaw -= fl;
			roll -= fl;

			return *this;
		}

		constexpr ang operator*( const ang& a ) const { return ang( pitch * a.pitch, yaw * a.yaw, roll * a.roll ); }

		constexpr ang operator*( const f32 fl ) const { return ang( pitch * fl, yaw * fl, roll * fl ); }

		constexpr ang& operator*=( const ang& a ) {
			pitch *= a.pitch;
			yaw *= a.yaw;
			roll *= a.roll;

			return *this;
		}

		constexpr ang& operator*=( const f32 fl ) {
			pitch *= fl;
			yaw *= fl;
			roll *= fl;

			return *this;
		}

		constexpr ang operator/( const ang& a ) const { return ang( pitch / a.pitch, yaw / a.yaw, roll / a.roll ); }

		constexpr ang operator/( const f32 fl ) const { return ang( pitch * fl, yaw * fl, roll * fl ); }

		constexpr ang& operator/=( const ang& a ) {
			pitch /= a.pitch;
			yaw /= a.yaw;
			roll /= a.roll;

			return *this;
		}

		constexpr ang& operator/=( const f32 fl ) {
			pitch /= fl;
			yaw /= fl;
			roll /= fl;

			return *this;
		}

		constexpr ang& normalize( ) {
			pitch = std::isfinite( pitch ) ? std::remainderf( pitch, 360.f ) : 0.f;
			yaw   = std::isfinite( yaw ) ? std::remainderf( yaw, 360.f ) : 0.f;
			roll  = std::clamp( roll, -50.f, 50.f );

			return *this;
		}

		constexpr ang normalized( ) {
			ang ret = *this;
			ret.normalize( );

			return ret;
		}

		constexpr ang clamp( ) {
			pitch = std::clamp( pitch, -89.f, 89.f );
			yaw   = std::clamp( yaw, -180.f, 180.f );
			roll  = std::clamp( roll, -50.f, 50.f );

			return *this;
		}

		constexpr ang& sanitize( ) {
			normalize( );
			clamp( );

			return *this;
		}
	};
} // namespace math

#endif // ANGLE_HPP