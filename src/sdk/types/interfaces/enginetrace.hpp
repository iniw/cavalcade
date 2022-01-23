#ifndef ENGINETRACE_HPP
#define ENGINETRACE_HPP

#pragma once

#include "../../other.hpp"

namespace sdk {
	struct base_entity;

	struct ray {
		ray( const math::v3f& src, const math::v3f& dest ) : m_start( src ), m_delta( dest - src ) {
			m_is_swept = m_delta[ 0 ] || m_delta[ 1 ] || m_delta[ 2 ];
		}

		math::v3f m_start;
		PAD( 4 );
		math::v3f m_delta;
		PAD( 40 );
		bool m_is_ray{ true };
		bool m_is_swept{ };
	};

	struct trace_filter {
		trace_filter( const base_entity* e ) : m_skip( e ) { }

		virtual bool should_hit_entity( base_entity* e, int ) {
			return e != m_skip;
		}

		virtual i32 get_trace_type( ) const {
			return 0;
		}

		const base_entity* m_skip;
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

		constexpr f32 get_damage_multiplier( i32 group ) noexcept {
			switch ( group ) {
			case HEAD:
				return 4.0f;
			case STOMACH:
				return 1.25f;
			case LEFT_LEG:
			case RIGHT_LEG:
				return 0.75f;
			default:
				return 1.0f;
			}
		}

		constexpr bool is_armored( i32 group, bool helmet ) noexcept {
			switch ( group ) {
			case HEAD:
				return helmet;

			case CHEST:
			case STOMACH:
			case LEFT_ARM:
			case RIGHT_ARM:
				return true;
			default:
				return false;
			}
		}
	} // namespace hit_group

	struct trace {
		math::v3f m_start;
		math::v3f m_end;
		PAD( 20 );
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
		base_entity* m_entity;
		i32 m_hitbox;
	};

	namespace interfaces {
		struct engine_trace {
			i32 get_point_concents( const math::v3f& abs_pos, i32 contents ) {
				return mem::call_v_func< i32, 0 >( this, std::cref( abs_pos ), contents, nullptr );
			}

			void trace_ray( const sdk::ray& ray, unsigned int mask, const trace_filter& filter, sdk::trace& trace ) {
				return mem::call_v_func< void, 5 >( this, std::cref( ray ), mask, std::cref( filter ), std::ref( trace ) );
			}
		};
	} // namespace interfaces
} // namespace sdk

#endif /* ENGINETRACE_HPP */
