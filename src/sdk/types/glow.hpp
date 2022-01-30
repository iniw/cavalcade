#ifndef GLOW_HPP
#define GLOW_HPP

#pragma once

#include "../../other.hpp"

namespace sdk {
	struct glow_object_definition {
		void set( f32 r, f32 g, f32 b, f32 a ) {
			m_color                  = math::v3f( r, g, b );
			m_alpha                  = a;
			m_render_when_occluded   = true;
			m_render_when_unoccluded = false;
			m_bloom_amount           = 1.0f;
		}

		bool unused( ) {
			return m_next_free_slot != -2;
		}

		i32 m_next_free_slot;
		unk m_entity;
		math::v3f m_color;
		f32 m_alpha;
		PAD( 8 );
		f32 m_bloom_amount;
		PAD( 4 );
		bool m_render_when_occluded;
		bool m_render_when_unoccluded;
		bool m_full_bloom_render;
		i32 m_full_bloom_stencil_test_value;
		i32 m_glow_style;
		i32 m_split_screen_slot;
	};

	struct glow_manager {
		glow_object_definition* m_objects;
		PAD( 8 );
		i32 m_size;
	};
} // namespace sdk

#endif /* GLOW_HPP */
