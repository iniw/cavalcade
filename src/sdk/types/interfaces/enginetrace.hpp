#ifndef ENGINETRACE_HPP
#define ENGINETRACE_HPP

#pragma once

#include "../../other.hpp"

namespace sdk {
	struct base_entity;
} // namespace sdk

namespace sdk {
	__declspec( align( 16 ) ) struct vec3 {
		vec3( ) = default;
		vec3( f32 x, f32 y, f32 z ) : x( x ), y( y ), z( z ) { }
		vec3( const math::v3f& v ) : x( v[ 0 ] ), y( v[ 1 ] ), z( v[ 2 ] ) { }
		vec3( const vec3& v ) : x( v.x ), y( v.y ), z( v.z ) { }

		f32 x;
		f32 y;
		f32 z;

		f32 w{ 0 };
	};

	struct ray {
		ray( ) = default;

		ray( const math::v3f& src, const math::v3f& dest ) : m_start( src ), m_delta( dest - src ) {
			m_is_swept = m_delta.x || m_delta.y || m_delta.z;

			m_extents.x = m_extents.y = m_extents.z = 0;
			m_start_offset.x = m_start_offset.y = m_start_offset.z = 0;
		}

		ray( const math::v3f& start, const math::v3f& end, const math::v3f& mins, const math::v3f& maxs ) {
			m_delta = end - start;

			m_is_swept = ( ( *( math::v3f* )&m_delta ).length_sqr( ) != 0.f );

			m_extents = ( maxs - mins ) * 0.5f;
			m_is_ray  = ( ( *( math::v3f* )&m_extents ).length_sqr( ) < 1e-6f );

			m_start_offset = ( ( mins + maxs ) * 0.5F ) * -1.F;
			m_start        = start + ( ( ( mins + maxs ) * 0.5F ) );
		}

		__declspec( align( 16 ) ) vec3 m_start;
		__declspec( align( 16 ) ) vec3 m_delta;
		__declspec( align( 16 ) ) vec3 m_start_offset;
		__declspec( align( 16 ) ) vec3 m_extents;
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
			u8 m_type;
			u8 m_signbits;
			u8 pad[ 2 ];
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
