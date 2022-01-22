#include "utils.hpp"

BOOL utils::create_thread( DWORD WINAPI function( unk ), unk parameter ) {
#if _DEBUG
	HANDLE handle = LI_FN( CreateThread )( nullptr, 0, function, parameter, 0, nullptr );
	if ( !handle )
		return FALSE;

	LI_FN( CloseHandle )( handle );
#else
	function( parameter );
#endif
	return TRUE;
}

void utils::sleep( u32 time ) {
	std::this_thread::sleep_for( chr::milliseconds( time ) );
}