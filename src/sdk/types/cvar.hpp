#pragma once

#include "utl_vector.hpp"

namespace sdk {
	struct cvar {
		enum idx
		{
			GET_FLOAT     = 12,
			GET_INT       = 13,
			SET_VALUE_STR = 14,
			SET_VALUE_F32 = 15,
			SET_VALUE_I32 = 16
		};

		using change_callback_t = void( __cdecl* )( unk, cstr, f32 );

		VFUNC( f32, get_float, idx::GET_FLOAT, ( ) );

		VFUNC( i32, get_int, idx::GET_INT, ( ) );

		VFUNC( void, set_value, idx::SET_VALUE_STR, ( std::string_view value ), value.data( ) );

		VFUNC( void, set_value, idx::SET_VALUE_F32, ( f32 value ), value );

		VFUNC( void, set_value, idx::SET_VALUE_I32, ( i32 value ), value );

		const std::optional< std::string > get_string( ) {
			if ( m_string == nullptr )
				return std::nullopt;

			return std::string( m_string );
		}

		PAD( 9 );
		cstr m_name;

		PAD( 0xC );
		cvar* m_parent;
		PAD( 0x4 );
		cstr m_string;
		PAD( 0x4 );
		f32 m_f32;
		i32 m_i32;
		PAD( 0xA );
		utl_vector< change_callback_t > m_change_callbacks;
	};
} // namespace sdk