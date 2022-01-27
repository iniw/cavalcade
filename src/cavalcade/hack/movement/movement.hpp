#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#pragma once

#include "../../../other/other.hpp"
#include "../../../sdk/types/move_data.hpp"
#include "../../../sdk/types/handle.hpp"
#include "../../../sdk/types/handle.inl"
#include "../../../sdk/enums/move_type.hpp"

namespace hack {
	struct movement {
		struct backup {
			void store( );
			void set( );

			i32 m_old_flags;
			f32 m_old_velocity_modifier;
			sdk::move_type m_old_move_type;
			math::v3f m_old_origin;
			math::v3f m_old_abs_origin;
			math::v3f m_old_view_offset;
			math::v3f m_old_velocity;
			f32 m_old_duck_amount;
			f32 m_old_duck_speed;
			f32 m_old_fall_velocity;
			f32 m_old_sim_time;
			sdk::handle m_old_ground_entity;
		};

		void pre( );

		void no_duck_delay( );
		void bunnyhop( );
		void jumpbug( );
		void longjump( );

		// Aug 3, 2021
		/*
		     herman hacks — Today at 15:27
		     can you put "I don't give a damn how you feel about me, I sip lean pure codeine and I don't give a damn what you say about me" in edgebug
		   struct
		 */
		struct edgebug {
			void prepare( );
			void set( );
			void change_move_data( );
			void predict( i32 base_flags, f32 base_velocity );
			void run( i32 base_flags, f32 base_velocity );

			sdk::move_data m_move_data;
			bool m_in_edgebug{ false };
			bool m_predicted{ false };
			bool m_fail_predict{ false };
			bool m_should_duck{ false };
			i32 m_simulation_tick{ 0 };
			i32 m_simulation_timestamp{ 0 };

			backup m_backup;
		} m_edgebug;

		void edgebug( );

		void edgebug_scale_mouse( f32& x );

		void post( );

		void clear( );

		i32 m_base_flags{ };
		bool m_in_jumpbug{ false };
		bool m_jumpbugged{ false };
		bool m_longjumped{ false };
		i32 m_lj_ducked_ticks{ 0 };
		bool m_wait_till_grounded{ false };
		bool m_lj_grounded{ false };
		f32 m_old_velocity_z{ 0 };
	};
} // namespace hack

#endif /* MOVEMENT_HPP */
