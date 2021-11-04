#pragma once

#include "../types/types.hpp"

namespace sdk {
	struct base_entity : interfaces::client_entity {
		enum idx
		{
			THINK              = 138,
			GET_CLASS_NAME     = 142,
			PHYSICS_SOLID_MASK = 152,
			IS_ALIVE           = 156,
			IS_PLAYER          = 158,
			IS_WEAPON          = 166
		};

		VFUNC( void, think, idx::THINK, ( ) );

		VFUNC( cstr, get_class_name, idx::GET_CLASS_NAME, ( ) );

		VFUNC( u32, physics_solid_mask, idx::PHYSICS_SOLID_MASK, ( ) );

		VFUNC( bool, is_alive, idx::IS_ALIVE, ( ) );

		VFUNC( bool, is_player, idx::IS_PLAYER, ( ) );

		VFUNC( bool, is_weapon, idx::IS_WEAPON, ( ) );

		NETVAR( i32, get_effects, "DT_BaseEntity->m_fEffects" );

		NETVAR( math::v3f, get_origin, "DT_BaseEntity->m_vecOrigin" );

		NETVAR( i32, get_collision_group, "DT_BaseEntity->m_CollisionGroup" );

		NETVAR( f32, get_sim_time, "DT_BaseEntity->m_flSimulationTime" );

		NETVAR( math::v3f, get_mins, "DT_BaseEntity->m_vecMins" );

		NETVAR( math::v3f, get_maxs, "DT_BaseEntity->m_vecMaxs" );

		NETVAR( i32, get_team, "DT_BaseEntity->m_iTeamNum" );

		NETVAR( handle, get_move_parent, "DT_BaseEntity->moveparent" );

		NETVAR_OFFSET( f32, get_old_sim_time, "DT_BaseEntity->m_flSimulationTime", 0x4 );

		DATAFIELD( math::matrix_3x4, get_coordinate_frame, "m_rgflCoordinateFrame" );

		auto physics_run_think( i32 think_method = 0 ) {
			static auto function = g_mem[ CLIENT_DLL ].get_address< bool( __thiscall* )( void*, i32 ) >( HASH_CT( "C_BaseEntity::PhysicsRunThink" ) );

			return function( this, think_method );
		}

		auto check_has_think_function( bool is_thinking = false ) {
			static auto function =
				g_mem[ CLIENT_DLL ].get_address< void( __thiscall* )( void*, bool ) >( HASH_CT( "C_BaseEntity::CheckHasThinkFunction" ) );

			return function( this, is_thinking );
		}

		static void set_prediction_random_seed( u32 value ) {
			static auto pred_random_seed =
				g_mem[ CLIENT_DLL ].get_address( HASH_CT( "C_BaseEntity->m_nPredictionRandomSeed" ) ).add( 0x4 ).to< i32* >( );

			if ( !value ) {
				*pred_random_seed = -1;
				return;
			}

			*pred_random_seed = value;
		}

		static void set_prediction_player( base_entity* player ) {
			static auto pred_player =
				g_mem[ CLIENT_DLL ].get_address( HASH_CT( "C_BaseEntity->m_pPredictionPlayer" ) ).add( 0x5 ).to< base_entity** >( );

			*pred_player = player;
		}
	};
} // namespace sdk