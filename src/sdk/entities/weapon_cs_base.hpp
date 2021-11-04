#pragma once

#include "base_attributable_item.hpp"

namespace sdk {
	struct weapon_cs_base : base_attributable_item {
		enum idx
		{
			GET_MAX_SPEED      = 442,
			GET_CS_WEAPON_INFO = 461
		};

		VFUNC( f32, get_max_speed, idx::GET_MAX_SPEED, ( ) );

		VFUNC( cs_weapon_info*, get_cs_weapon_info, idx::GET_CS_WEAPON_INFO, ( ) );

		NETVAR( bool, is_burst_mode, "DT_WeaponCSBase->m_bBurstMode" );

		NETVAR( f32, get_fire_ready_time, "DT_WeaponCSBase->m_flPostponeFireReadyTime" );

		NETVAR( i32, get_burst_shots_remaining, "DT_WeaponCSBaseGun->m_iBurstShotsRemaining" );

		NETVAR( i32, get_ammo, "DT_BaseCombatWeapon->m_iClip1" );

		NETVAR( i32, get_view_model_index, "DT_BaseCombatWeapon->m_nViewModelIndex" );

		NETVAR( f32, get_next_primary_attack, "DT_BaseCombatWeapon->m_flNextPrimaryAttack" );
	};
} // namespace sdk