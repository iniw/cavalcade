#ifndef I_PREDICTION_HPP
#define I_PREDICTION_HPP

#include "../../entities/entities.hpp"

namespace sdk::interfaces {
	struct prediction {
		PAD( 0x8 );
		bool m_in_prediction;
		PAD( 0x1 );
		bool m_engine_paused;
		PAD( 0xD );
		bool m_is_first_time_predicted;

		VFUNC( void, update, 3, ( i32 start_frame, bool valid_frame, i32 incoming_acknowledged, i32 outgoing_command ),
		       start_frame, valid_frame, incoming_acknowledged, outgoing_command );

		VFUNC( void, check_moving_ground, 18, ( base_entity * entity, f64 frametime ), entity, frametime );

		VFUNC( void, setup_move, 20, ( base_entity * entity, user_cmd* cmd, move_helper* helper, move_data* move_data ), entity,
		       cmd, helper, move_data );

		VFUNC( void, finish_move, 21, ( base_entity * entity, user_cmd* cmd, move_data* move_data ), entity, cmd, move_data );

		auto start_command( cs_player* player, user_cmd* cmd ) {
			*player->get_current_cmd( ) = cmd;
			// player->get_last_command( ) = *cmd;

			sdk::base_entity::set_prediction_random_seed( MD5( cmd->m_command_number ) & 0x7FFFFFFF );
			sdk::base_entity::set_prediction_player( player );
		}

		auto run_pre_think( cs_player* player ) {
			if ( !player->physics_run_think( ) )
				return;

			player->pre_think( );
		}

		auto run_think( cs_player* player, i32 tickbase ) {
			auto& thinktick = player->get_next_think_tick( );

			if ( thinktick <= 0 || thinktick > tickbase )
				return;

			thinktick = -1;

			player->check_has_think_function( );

			player->think( );
		}

		// auto run_post_think( cs_player* player ) { player->post_think( ); }

		auto finish_command( cs_player* player ) {
			*player->get_current_cmd( ) = nullptr;
			sdk::base_entity::set_prediction_random_seed( NULL );
			sdk::base_entity::set_prediction_player( nullptr );
		}
	};
} // namespace sdk::interfaces

#endif // I_PREDICTION_HPP