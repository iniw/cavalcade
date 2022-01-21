#ifndef AIMBOT_HPP
#define AIMBOT_HPP

#include "../../../other/other.hpp"

namespace sdk {
	struct cs_player;
} // namespace sdk

namespace hack {
	struct aimbot {
		enum e_hitboxes
		{
			NEAREST = 0,
			HEAD    = 1 << 1,
			NECK    = 1 << 2,
			BODY    = 1 << 3,
			ARMS    = 1 << 4,
			LEGS    = 1 << 5
		};

		static math::ang pixels_to_angle( f32 x, f32 y );
		static math::ang angle_to_pixels( const math::ang& );

		void run( f32& x, f32& y );

		sdk::cs_player* m_best_player{ nullptr };
		f32 m_best_fov{ std::numeric_limits< f32 >::max( ) };

		// move to cfg
		bool m_rcs{ true };
	};
} // namespace hack

#endif /* AIMBOT_HPP */
