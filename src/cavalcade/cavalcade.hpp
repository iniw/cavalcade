#pragma once

namespace cavalcade {
	inline HMODULE handle;

	inline HWND window;

	DWORD WINAPI init( unk module_handle );

	DWORD WINAPI unload( BOOL exit_type );
} // namespace cavalcade