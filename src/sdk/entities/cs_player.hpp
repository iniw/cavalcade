#pragma once

#include "base_animating.hpp"

#include "../../../other/other.hpp"

namespace sdk {
	struct cs_player : base_animating {
		NETVAR( f32, get_velocity_modifier, "DT_CSPlayer->m_flVelocityModifier" );

		NETVAR( bool, is_immune, "DT_CSPlayer->m_bGunGameImmunity" );

		NETVAR( bool, is_player_ghost, "DT_CSPlayer->m_bIsPlayerGhost" );

		NETVAR( bool, is_scoped, "DT_CSPlayer->m_bIsScoped" );

		NETVAR( bool, has_helmet, "DT_CSPlayer->m_bHasHelmet" );

		NETVAR( i32, armor_value, "DT_CSPlayer->m_ArmorValue" );

		NETVAR( i16, current_equip_value, "DT_CSPlayer->m_unCurrentEquipmentValue" );

		NETVAR_OFFSET( bool, is_using_new_anim_state, "DT_CSPlayer->m_flLastExoJumpTime", 0x8 );

		bool is_enemy( base_player* player );

		bool is_other_enemy( base_player* other );

		void post_think( );

		math::v3f get_hitbox_position( i32 hitbox_id );

		math::v3f get_eye_position( );

		bool can_fire_shot( );
	};
} // namespace sdk