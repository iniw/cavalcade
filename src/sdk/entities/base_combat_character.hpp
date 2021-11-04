#pragma once

#include "base_entity.hpp"

namespace sdk {
	struct base_combat_character : base_entity {
		NETVAR( handle, get_active_weapon, "DT_BaseCombatCharacter->m_hActiveWeapon" );

		NETVAR( f32, get_next_attack, "DT_BaseCombatCharacter->m_flNextAttack" );
	};
} // namespace sdk