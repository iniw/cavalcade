#ifndef AIMBOT_HPP
#define AIMBOT_HPP

#include "../../../other/other.hpp"

namespace sdk {
	struct cs_player;
} // namespace sdk

namespace hack {
	struct aimbot {
		static math::ang pixels_to_angle( f32 x, f32 y );
		static math::ang angle_to_pixels( const math::ang& );

		void run( f32& x, f32& y );

		sdk::cs_player* m_best_player{ nullptr };
		f32 m_best_fov{ std::numeric_limits< f32 >::max( ) };

		// move to cfg
		i32 m_smoothing{ 85 };
		i32 m_fov{ 50 };
		bool m_rcs{ true };
	};
} // namespace hack

#endif /* AIMBOT_HPP */
