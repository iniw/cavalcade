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

		// NOTE(para): maybe consider IsBoneAvailableForRead?
		math::matrix_3x4 get_cached_bone_matrix( i32 bone ) {
			math::matrix_3x4 out{ };
			if ( bone < 0 || bone > 256 )
				return out;

			static auto og =
				g_mem[ CLIENT_DLL ].get_address< void( __thiscall* )( base_animating*, i32, math::matrix_3x4& ) >( HASH_CT( "GetCachedBoneMatrix" ) );
			og( this, bone, out );
			return out;
		}
	};
} // namespace sdk