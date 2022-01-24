#ifndef ENGINETRACE_HPP
#define ENGINETRACE_HPP

#pragma once

#include "../../other.hpp"

namespace sdk {
	struct base_entity;
} // namespace sdk

namespace sdk {
	struct __declspec( align( 16 ) ) v3f_aligned {
		v3f_aligned( ) = default;

		f32 x;
		f32 y;
		f32 z;
		f32 w;

		explicit v3f_aligned( const math::v3f& v ) {
			x = v[ 0 ];
			y = v[ 1 ];
			z = v[ 2 ];
			w = 0;
		}

		constexpr v3f_aligned& operator=( const math::v3f& v ) {
			x = v[ 0 ];
			y = v[ 1 ];
			z = v[ 2 ];
			w = 0;

			return *this;
		}
	};

	struct ray {
		ray( const math::v3f& src, const math::v3f& dest ) : m_start( src ), m_delta( dest - src ) {
			m_is_swept = m_delta.x || m_delta.y || m_delta.z;
		}

		v3f_aligned m_start;
		v3f_aligned m_delta;
		v3f_aligned m_start_offset;
		v3f_aligned m_vec_extents;
		PAD( 4 );
		bool m_is_ray{ true };
		bool m_is_swept{ };
	};

	struct trace_filter {
		trace_filter( ) = default;

		trace_filter( const sdk::base_entity* e ) : m_skip( e ) { }

		virtual bool should_hit_entity( sdk::base_entity* e, int ) {
			return e != m_skip;
		}

		virtual i32 get_trace_type( ) const {
			return 0;
		}

		const sdk::base_entity* m_skip;
	};

	namespace hit_group {
		enum
		{
			INVALID = -1,
			GENERIC,
			HEAD,
			CHEST,
			STOMACH,
			LEFT_ARM,
			RIGHT_ARM,
			LEFT_LEG,
			RIGHT_LEG,
			GEAR = 10
		};
	} // namespace hit_group

	struct trace {
		math::v3f m_start;
		math::v3f m_end;
		struct plane {
			math::v3f m_normal;
			f32 m_dist;
			uint8_t m_type;
			uint8_t signbits;
			PAD( 2 );
		} m_plane;
		f32 m_fraction;
		i32 m_contents;
		unsigned short m_disp_flags;
		bool m_all_solid;
		bool m_start_solid;
		PAD( 4 );
		struct surface {
			const char* m_name;
			short m_surface_props;
			unsigned short m_flags;
		} m_surface;
		i32 m_hitgroup;
		PAD( 4 );
		sdk::base_entity* m_entity;
		i32 m_hitbox;
	};

	namespace interfaces {
		struct engine_trace {
			i32 get_point_concents( const math::v3f& abs_pos, i32 contents ) {
				return mem::call_v_func< i32, 0 >( this, std::cref( abs_pos ), contents, nullptr );
			}

			void clip_ray_to_entity( const sdk::ray& ray, unsigned int fmask, sdk::base_entity* ent, sdk::trace& trace ) {
				return mem::call_v_func< void, 3 >( this, std::cref( ray ), fmask, ent, std::ref( trace ) );
			}

			void trace_ray( const sdk::ray& ray, unsigned int mask, const trace_filter& filter, sdk::trace& trace ) {
				return mem::call_v_func< void, 5 >( this, std::cref( ray ), mask, std::cref( filter ), std::ref( trace ) );
			}
		};
	} // namespace interfaces
} // namespace sdk

#endif /* ENGINETRACE_HPP */
