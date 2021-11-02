#include "console.hpp"

bool io::console::init( ) {
#ifdef IO_LOG_CONSOLE
	MOCKING_TRY;

	MOCK AllocConsole( );

	freopen_s( reinterpret_cast< _iobuf** >( stdout ), "CONOUT$", "w", stdout );

	SetConsoleTitleA( "cavalcade" );

	HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );
	MOCK handle;

	DWORD mode = 0;
	GetConsoleMode( handle, &mode );

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	return SetConsoleMode( handle, mode );

	MOCKING_CATCH( return false );
#endif

	return true;
}
void io::console::unload( ) {
#ifdef IO_LOG_CONSOLE

	fclose( static_cast< _iobuf* >( stdout ) );

	FreeConsole( );

#endif
}