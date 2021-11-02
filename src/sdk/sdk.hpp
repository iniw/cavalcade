#ifndef SDK_HPP
#define SDK_HPP

#include "enums/enums.hpp"
#include "types/types.hpp"
#include "types/handle.inl" // oof
#include "netvars/netvars.hpp"
#include "entities/entities.hpp"
#include "csgo/csgo.hpp"

namespace sdk {
	inline i32 time_to_ticks( f32 time ) {
		return static_cast< i32 >( 0.5f + time / g_csgo.m_globals->m_interval_per_tick );
	}

	inline f32 ticks_to_time( i32 ticks ) {
		return static_cast< f32 >( ticks ) * g_csgo.m_globals->m_interval_per_tick;
	}
} // namespace sdk

#endif // SDK_HPP