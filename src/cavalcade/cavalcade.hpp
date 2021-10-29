#ifndef CAVALCADE_HPP
#define CAVALCADE_HPP

namespace cavalcade {
	inline HMODULE handle;
	inline HWND window;

	DWORD WINAPI init( unk handle );

	DWORD WINAPI unload( BOOL exit_type );
} // namespace cavalcade

#endif // CAVALCADE_HPP
