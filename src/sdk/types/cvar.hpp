#ifndef CVAR_HPP
#define CVAR_HPP

#include "utl_vector.hpp"

namespace sdk {
	struct cvar {
		enum idx
		{
			GET_NAME      = 5,
			GET_FLOAT     = 12,
			GET_INT       = 13,
			SET_VALUE_STR = 14,
			SET_VALUE_F32 = 15,
			SET_VALUE_I32 = 16
		};

		using change_callback_t = void( __cdecl* )( unk, cstr, f32 );

		VFUNC( cstr, get_name, idx::GET_NAME, ( ) );

		VFUNC( f32, get_float, idx::GET_FLOAT, ( ) );

		VFUNC( i32, get_int, idx::GET_INT, ( ) );

		VFUNC( void, set_value, idx::SET_VALUE_STR, ( std::string_view value ), value.data( ) );

		VFUNC( void, set_value, idx::SET_VALUE_F32, ( f32 value ), value );

		VFUNC( void, set_value, idx::SET_VALUE_I32, ( i32 value ), value );

		PAD( 0x19 );

		cvar* m_parent;

		PAD( 0x4 );

		str m_string;

		PAD( 0x4 );

		f32 m_f32;
		i32 m_i32;

		PAD( 0xA );

		utl_vector< change_callback_t > m_change_callbacks;
	};
} // namespace sdk

#endif // CVAR_HPP
