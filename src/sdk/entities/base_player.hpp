#pragma once

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

		NETVAR( f32, get_duck_amount, "DT_BasePlayer->m_flDuckAmount" );

		NETVAR( f32, get_duck_speed, "DT_BasePlayer->m_flDuckSpeed" );

		NETVAR( i32, get_health, "DT_BasePlayer->m_iHealth" );

		NETVAR( i32, get_flags, "DT_BasePlayer->m_fFlags" );

		NETVAR( i32, get_tickbase, "DT_BasePlayer->m_nTickBase" );

		NETVAR( i32, get_next_think_tick, "DT_BasePlayer->m_nNextThinkTick" );

		NETVAR( f32, get_fall_velocity, "DT_BasePlayer->m_flFallVelocity" );

		NETVAR( math::v3f, get_velocity, "DT_BasePlayer->m_vecVelocity[0]" );

		NETVAR( math::v3f, get_view_offset, "DT_BasePlayer->m_vecViewOffset[0]" );

		NETVAR( math::ang, get_aim_punch_angle, "DT_BasePlayer->m_aimPunchAngle" );

		NETVAR_OFFSET( i32, get_button_disabled, "DT_BasePlayer->m_hViewEntity", -0xC );

		NETVAR_OFFSET( i32, get_button_forced, "DT_BasePlayer->m_hViewEntity", -0x8 );

		PNETVAR_OFFSET( user_cmd*, get_current_cmd, "DT_BasePlayer->m_hViewEntity", -0x4 );

		DATAFIELD_PRED( i32, get_buttons, "m_nButtons" );

		DATAFIELD_PRED( i32, get_button_last, "m_afButtonLast" );

		DATAFIELD_PRED( i32, get_button_pressed, "m_afButtonPressed" );

		DATAFIELD_PRED( i32, get_button_released, "m_afButtonReleased" );

		DATAFIELD_PRED( move_type, get_move_type, "m_MoveType" );
		DATAFIELD_PRED( handle, get_ground_entity, "m_hGroundEntity" );

		DATAFIELD_PRED( i32, get_impulse, "m_nImpulse" );
		DATAFIELD_PRED( math::v3f, get_base_velocity, "m_vecBaseVelocity" );
		DATAFIELD_PRED( i32, get_eflags, "m_iEFlags" );

		// vfunc for isalive is a horrible idea. vfcalls are one of the slowest operation possible by default
		// VFUNC( bool, is_alive, idx::IS_ALIVE, ( ) );
		NETVAR( i8, get_lifestate, "DT_BasePlayer->m_lifeState" );

		bool is_alive( ) {
			return get_health( ) > 0 || get_lifestate( ) == 0;
		}

		auto set_local_view_angles( const math::ang& view_angles ) {
			return mem::call_v_func< void, idx::SET_LOCAL_VIEW_ANGLES, const math::ang& >( this, view_angles );
		}

		auto post_think_v_physics( ) {
			static auto function =
				g_mem[ CLIENT_DLL ].get_address< bool( __thiscall* )( base_player* ) >( HASH_CT( "C_BasePlayer::PostThinkVPhysics" ) );

			return function( this );
		}

		auto simulate_player_simulated_entities( ) {
			static auto function =
				g_mem[ CLIENT_DLL ].get_address< void( __thiscall* )( base_player* ) >( HASH_CT( "C_BasePlayer::SimulatePlayerSimulatedEntities" ) );

			return function( this );
		}

		auto update_button_state( i32 cmd_mask ) {
			auto& buttons = get_buttons( );

			get_button_last( ) = buttons;

			buttons             = cmd_mask;
			i32 buttons_changed = get_button_last( ) ^ buttons;

			get_button_pressed( )  = buttons_changed & buttons;
			get_button_released( ) = buttons_changed & ( ~buttons );
		}

		void set_abs_origin( const math::v3f& origin ) {
			static auto function =
				g_mem[ CLIENT_DLL ].get_address< void( __thiscall* )( base_player*, const math::v3f& ) >( HASH_CT( "C_BaseEntity::SetAbsOrigin" ) );

			function( this, origin );
		}
	};
} // namespace sdk