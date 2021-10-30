#ifndef CS_PLAYER_HPP
#define CS_PLAYER_HPP

#include "base_animating.hpp"

namespace sdk {
	struct cs_player : base_animating {
		NETVAR( bool, is_immune, "DT_CSPlayer->m_bGunGameImmunity" );

		NETVAR( bool, is_player_ghost, "DT_CSPlayer->m_bIsPlayerGhost" );

		NETVAR( bool, is_scoped, "DT_CSPlayer->m_bIsScoped" );

		NETVAR_OFFSET( bool, is_using_new_anim_state, "DT_CSPlayer->m_flLastExoJumpTime", 0x8 );

		bool is_enemy( base_player* player );
	};
} // namespace sdk

#endif // CS_PLAYER_HPP