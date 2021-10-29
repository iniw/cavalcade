#include "console.hpp"

bool io::console::init( ) {
#ifdef IO_LOG_CONSOLE
	if ( !AllocConsole( ) )
		return false;

	freopen_s( reinterpret_cast< _iobuf** >( stdout ), "CONOUT$", "w", stdout );

	SetConsoleTitleA( "cavalcade" );

	HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );
	if ( !handle )
		return false;

	DWORD mode = 0;
	GetConsoleMode( handle, &mode );

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	return SetConsoleMode( handle, mode );

#endif

	return true;
}

void io::console::unload( ) {
#ifdef IO_LOG_CONSOLE

	fclose( static_cast< _iobuf* >( stdout ) );

	FreeConsole( );

#endif
}