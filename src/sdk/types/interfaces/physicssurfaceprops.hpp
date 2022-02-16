

#pragma once

namespace sdk {
	struct surface_data {
		PAD( 80 );
		f32 m_max_speed_factor;
		f32 m_jump_factor;
		f32 m_penetration_modifier;
		f32 m_damage_modifier;
		short m_material;
		bool m_climbable;
	};

	namespace interfaces {
		struct physics_surface_props {
			surface_data* get_surface_data( i32 index ) {
				return mem::call_v_func< surface_data*, 5 >( this, index );
			}
		};
	} // namespace interfaces
} // namespace sdk