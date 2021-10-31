#ifndef BASE_PLAYER_HPP
#define BASE_PLAYER_HPP

#include "base_combat_character.hpp"

namespace sdk {
	struct base_player : base_combat_character {
		enum idx
		{
			PRE_THINK             = 318,
			SELECT_ITEM           = 330,
			SET_LOCAL_VIEW_ANGLES = 373
		};

		VFUNC( void, pre_think, idx::PRE_THINK, ( ) );

		VFUNC( void, select_item, idx::SELECT_ITEM, ( cstr item, i32 sub_type ), item, sub_type );

		NETVAR( i32, get_health, "DT_BasePlayer->m_iHealth" );

		NETVAR( i32, get_flags, "DT_BasePlayer->m_fFlags" );

		NETVAR( i32, get_tickbase, "DT_BasePlayer->m_nTickBase" );

		NETVAR( i32, get_next_think_tick, "DT_BasePlayer->m_nNextThinkTick" );

		NETVAR( f32, get_fall_velocity, "DT_BasePlayer->m_flFallVelocity" );

		NETVAR_OFFSET( i32, get_button_disabled, "DT_BasePlayer->m_hViewEntity", -0xC );

		NETVAR_OFFSET( i32, get_button_forced, "DT_BasePlayer->m_hViewEntity", -0x8 );

		PNETVAR_OFFSET( user_cmd*, get_current_cmd, "DT_BasePlayer->m_hViewEntity", -0x4 );

		DATAFIELD_PRED( move_type, get_move_type, "m_MoveType" );

		DATAFIELD_PRED( i32, get_buttons, "m_nButtons" );

		DATAFIELD_PRED( i32, get_button_last, "m_afButtonLast" );

		DATAFIELD_PRED( i32, get_button_pressed, "m_afButtonPressed" );

		DATAFIELD_PRED( i32, get_button_released, "m_afButtonReleased" );

		/*

	N_ADD_VARIABLE_OFFSET(int, GetButtonDisabled, "CBasePlayer->m_hViewEntity", -0xC);
	N_ADD_VARIABLE_OFFSET(int, GetButtonForced, "CBasePlayer->m_hViewEntity", -0x8);
	N_ADD_PVARIABLE_OFFSET(CUserCmd*, GetCurrentCommand, "CBasePlayer->m_hViewEntity", -0x4); // @ida: client.dll @ [89 BE ? ? ? ?
	E8 ? ? ? ? 85 FF + 0x2]
		*/
		auto set_local_view_angles( const math::ang& view_angles ) {
			return mem::call_v_func< void, idx::SET_LOCAL_VIEW_ANGLES, const math::ang& >( this, view_angles );
		}
	};
} // namespace sdk

#endif // BASE_PLAYER_HPP