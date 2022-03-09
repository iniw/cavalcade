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
		NETVAR( math::ang, get_ang_eye_pos, "DT_CSPlayer->m_angEyeAngles[0]" );

		NETVAR_OFFSET( bool, is_using_new_anim_state, "DT_CSPlayer->m_flLastExoJumpTime", 0x8 );

		OFFSET( i32, occlusion_flags, 0xA28 );
		OFFSET( i32, occlusion_framecount, 0xA30 );
		OFFSET( i32, get_bone_count, 0x291C );

		bool is_enemy( base_player* player );

		bool is_other_enemy( base_player* other );

		void post_think( );

		math::v3f get_hitbox_position( i32 hitbox_id, const math::matrix_3x4* m );
		math::v3f get_hitbox_position( i32 hitbox_id );

		// NOTE(para): this is what we modify
		sdk::utl_vector< math::matrix_3x4 >& get_cached_bones( );

		sdk::varmapping* varmapping( );

		math::v3f get_eye_position( );

		bool can_fire_shot( );
	};
} // namespace sdk