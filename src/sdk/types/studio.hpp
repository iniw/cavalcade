
#pragma once

#include "material.hh"
#include "../../other/render/types/color.hpp"

namespace sdk {
	struct draw_model_info {
		PAD( 0x18 );
		size_t m_client_entity;
	};

	struct studio_render_context {
		PAD( 0x240 );
		f32 m_color_mod[ 3 ];
		f32 m_alpha_mod;
		material* m_forced_material[ 4 ];
		i32 m_forced_material_type;
		i32 m_forced_material_index[ 4 ];
		i32 m_forced_material_index_count;

		inline void set_color( const render::color& color ) {
			m_color_mod[ 0 ] = ( f32 )( color.m_r ) / 255.f;
			m_color_mod[ 1 ] = ( f32 )( color.m_g ) / 255.f;
			m_color_mod[ 2 ] = ( f32 )( color.m_b ) / 255.f;
		}

		inline void set_alpha( float alpha ) {
			m_alpha_mod = alpha;
		}

		inline void set_material( material* material, bool ignore_z = true ) {
			if ( material )
				material->set_material_var_flag( 1 << 15, ignore_z );

			m_forced_material_type        = 0;
			m_forced_material[ 0 ]        = material;
			m_forced_material_index[ 0 ]  = -1;
			m_forced_material_index_count = 0;
		}

		bool is_forced_material_override( ) {
			if ( !*m_forced_material )
				return m_forced_material_type == 2 || m_forced_material_type == 4;

			return std::string_view{ ( *m_forced_material )->get_name( ) }.starts_with( XOR( "dev/glow" ) );
		}

		bool is_weapon( ) {
			return m_forced_material_type == 3;
		}
	};
} // namespace sdk