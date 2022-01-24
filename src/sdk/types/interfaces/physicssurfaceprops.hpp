#ifndef PHYSICSSURFACEPROPS_HPP
#define PHYSICSSURFACEPROPS_HPP

#pragma once

namespace sdk {
	struct surface_data {
		struct surfacephysicsparams {
			f32 m_friction;
			f32 m_elasticity;
			f32 m_density;
			f32 m_thickness;
			f32 m_dampening;
		} m_physics;

		struct surfaceaudioparams {
			f32 m_audio_reflectivity;
			f32 m_audio_hardness_factor;
			f32 m_audio_roughness_factor;
			f32 m_scrape_rough_threshold;
			f32 m_impact_hard_threshold;
			f32 m_audio_hard_min_velocity;
			f32 m_high_pitch_occlusion;
			f32 m_mid_pitch_occlusion;
			f32 m_low_pitch_occlusion;
		} m_audio;

		struct surfacesoundnames {
			short m_walk_left;
			short m_walk_right;
			short m_run_left;
			short m_run_right;
			short m_impact_soft;
			short m_impact_hard;
			short m_scrape_smooth;
			short m_scrape_rough;
			short m_bullet_impact;
			short m_rolling;
			short m_break_sound;
			short m_strain;
		} m_sounds;

		struct surfacegameprops {
			f32 m_max_speed_factor;
			f32 m_jump_factor;
			f32 m_penetration_modifier;
			f32 m_damage_modifier;
			u16 m_material;
			u8 m_climbable;
		} m_game;

		PAD( 48 );
	};

	namespace interfaces {
		struct physics_surface_props {
			surface_data* get_surface_data( i32 index ) {
				return mem::call_v_func< surface_data*, 5 >( this, index );
			}
		};
	} // namespace interfaces
} // namespace sdk

#endif /* PHYSICSSURFACEPROPS_HPP */
