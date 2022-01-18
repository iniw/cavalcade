#ifndef AIMBOT_HPP
#define AIMBOT_HPP

#include "../../../other/other.hpp"

namespace hack {
	struct aimbot {
		static math::ang pixels_to_angle( f32 x, f32 y );
		static math::ang angle_to_pixels( const math::ang& );

		void run( float& x, float& y );

		i32 m_smoothing{ 85 };
		i32 m_fov{ 50 };
		bool m_rcs{ true };
	};
} // namespace hack

#endif /* AIMBOT_HPP */
