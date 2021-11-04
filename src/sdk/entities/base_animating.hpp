#pragma once

#include "base_player.hpp"

namespace sdk {
	struct base_animating : base_player {
		enum idx
		{ UPDATE_CLIENT_SIDE_ANIMATION = 224 };

		VFUNC( void, update_client_side_animation, idx::UPDATE_CLIENT_SIDE_ANIMATION, ( ) )

		NETVAR( f32, get_pose_parameter, "DT_BaseAnimating->m_flPoseParameter" );

		NETVAR( i32, get_sequence, "DT_BaseAnimating->m_nSequence" );

		NETVAR( bool, get_client_side_animation, "DT_BaseAnimating->m_bClientSideAnimation" );

		NETVAR_OFFSET( bool, are_jiggle_bones_enabled, "DT_BaseAnimating->m_hLightingOrigin", -0x18 );
	};
} // namespace sdk