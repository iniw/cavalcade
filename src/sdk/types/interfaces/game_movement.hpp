#ifndef GAME_MOVEMENT_HPP
#define GAME_MOVEMENT_HPP

namespace sdk::interfaces {
	struct game_movement {
		VFUNC( void, process_movement, 1, ( cs_player * player, move_data* move_data ), player, move_data );

		VFUNC( void, reset, 2, ( ) );

		VFUNC( void, start_track_prediction_errors, 3, ( cs_player * player ), player );

		VFUNC( void, finish_track_prediction_errors, 4, ( cs_player * player ), player );
	};

} // namespace sdk::interfaces

#endif // GAME_MOVEMENT_HPP
