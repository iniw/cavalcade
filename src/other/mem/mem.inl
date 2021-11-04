#pragma once

template< typename T >
inline T mem::get_v_func( address thisptr, u32 idx ) {
	return ( T )( thisptr.to< T* >( )[ idx ] );
}

template< typename T, u32 I, typename... VA >
inline T mem::call_v_func( address thisptr, VA... args ) {
	using FN = T( __thiscall* )( unk, decltype( args )... );
	return thisptr.to< FN* >( )[ I ]( thisptr, args... );
}

#pragma once