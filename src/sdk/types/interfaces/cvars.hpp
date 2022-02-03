#pragma once

#include "../../../other/render/types/color.hpp"

namespace sdk::interfaces {
	struct cvars {
		enum idx
		{
			FIND_VAR             = 16,
			CONSOLE_COLOR_PRINTF = 25
		};

		VFUNC( cvar*, find_var, idx::FIND_VAR, ( std::string_view name ), name.data( ) );
		void console_color_printf( const render::color& clr, const char* fmt ) {
			const uint8_t c[ 4 ] = { clr.m_r, clr.m_g, clr.m_b, clr.m_a };
			return mem::call_v_func_cdecl< void, idx::CONSOLE_COLOR_PRINTF >( this, c, fmt );
		}
	};
} // namespace sdk::interfaces