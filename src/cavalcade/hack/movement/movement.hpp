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
			i32 m_old_impulse;
			i32 m_old_buttons;
			math::v3f m_old_base_velocity;
			i32 m_old_eflags;
		};

		void pre( );

		void no_duck_delay( );
		void bunnyhop( );
		void jumpbug( );
		void longjump( );
		void ladderjump( );

		struct pixelsurf {
			void run( const math::v3f& );
			void autoalign( );
			void clear( );

			i32 m_old_buttons{ 0 };
			i32 m_duration{ 0 };
			bool m_autoalign{ false };
			bool m_lock_mouse{ false };
			bool m_in_pixelsurf{ false };
		} m_pixelsurf;

		void pixelsurf_calculator( );

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

		struct jumpstats {
			f32 m_distance{ 0 };
			i32 m_pre{ 0 };
			i32 m_max{ 0 };
			i32 m_strafes{ 0 };
			f32 m_sync{ 0 };
			i32 m_hops{ 0 };
			math::v3f m_jump_origin{ };
			f32 m_height{ 0 };
			math::v3f m_jump_position[ 2 ]{ };
			bool m_last_jumping{ false };
			bool m_strafe_left{ false };
			bool m_strafe_right{ false };
			bool m_calculate_height{ false };
			i32 m_duration_ticks_pending{ 0 };
			i32 m_duration_ticks{ 0 };
			i32 m_on_ground_tick{ 0 };
			i32 m_air_tick{ 0 };
			bool m_jumpbug_update{ false };
			bool m_jumpbug{ false };
			f32 m_jumpbug_z{ 0 };

			void run( );
			void reset( );
		} m_jumpstats;

		math::v3f m_base_origin{ };

		i32 m_base_flags{ };
		bool m_in_jumpbug{ false };
		bool m_jumpbugged{ false };
		bool m_longjumped{ false };
		i32 m_lj_ducked_ticks{ 0 };
		bool m_wait_till_grounded{ false };
		bool m_lj_grounded{ false };
		f32 m_old_velocity_z{ 0 };
		i32 m_ladder_jump_tick{ 0 };
		bool m_in_ladderjump{ false };
		bool m_in_minijump{ false };
		std::optional< math::v3f > m_pixelsurf_calculator_point{ };
	};
} // namespace hack

#endif /* MOVEMENT_HPP */