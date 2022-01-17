#ifndef AIMBOT_HPP
#define AIMBOT_HPP

#include "../../../other/other.hpp"

namespace hack {
	struct aimbot {
		static math::ang pixels_to_angle( float x, float y );
		static math::ang angle_to_pixels( const math::ang& );

		void run( float& x, float& y );

		// 0-100
		int m_strength{ 50 };
		// 0-100
		int m_smoothing{ 50 };
	};
} // namespace hack

#endif /* AIMBOT_HPP */
