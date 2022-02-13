#pragma once

static uintptr_t gadget;

template< typename T >
inline T mem::get_v_func( address thisptr, u32 idx ) {
	return ( T )( thisptr.to< T* >( )[ idx ] );
}

template< typename T, u32 I, typename... VA >
inline T mem::call_v_func( address thisptr, VA... args ) {
	using FN = T( __thiscall* )( unk, decltype( args )... );
	return thisptr.to< FN* >( )[ I ]( thisptr, args... );
	// return x86RetSpoof::invokeThiscall< T >( thisptr.as< uintptr_t >( ), get_v_func< uintptr_t >( thisptr, I ), gadget, args... );
}

template< typename T, u32 I, typename... VA >
inline T mem::call_v_func_cdecl( address thisptr, VA... args ) {
	using FN = T( __cdecl* )( unk, decltype( args )... );
	return thisptr.to< FN* >( )[ I ]( thisptr, args... );
	// return x86RetSpoof::invokeThiscall< T >( thisptr.as< uintptr_t >( ), get_v_func< uintptr_t >( thisptr, I ), gadget,
	//  /* thisptr.as< uintptr_t >( ), */ args... );
}