#include "../hooks.hpp"
#include "../../cavalcade.hpp"

// NOTE(para): verifies if return address is within module
// NOTE(para): we'd probably be fine if we just returned true too
bool cavalcade::hooks::valve::retaddr_bypass( unk ecx, unk edx, cstr addy ) {
	if ( ( uintptr_t )addy >= ( uintptr_t )cavalcade::handle ) {
		auto dos_header = ( PIMAGE_DOS_HEADER )cavalcade::handle;
		auto nt_headers = ( PIMAGE_NT_HEADERS )( ( uintptr_t )cavalcade::handle + dos_header->e_lfanew );
		if ( ( uintptr_t )nt_headers->OptionalHeader.ImageBase < ( uintptr_t )addy &&
		     ( uintptr_t )addy < ( ( uintptr_t )nt_headers->OptionalHeader.ImageBase + nt_headers->OptionalHeader.SizeOfImage ) ) {
			// g_io.log( "passed over this shit" );
			return true;
		}
	}

	// g_io.log( "executing normally" );

	int v3;    // eax
	LPCSTR v4; // edi
	int v5;    // ecx
	int v6;    // ecx
	int v7;    // eax

	v3 = 0;
	v4 = addy;
	v5 = *( int* )( ( uintptr_t )ecx + 160 );
	if ( v5 <= 0 ) {
	LABEL_5:
		if ( !GetModuleHandleExA( 6u, addy, ( HMODULE* )&addy ) )
			return 0;
		// eat shit
		// sub_102A8870( );
		v6 = *( int* )( ( uintptr_t )ecx + 160 );
		v7 = 0;
		if ( v6 <= 0 )
			return 0;
		while ( ( unsigned int )v4 < *( unsigned int* )( ( uintptr_t )ecx + v7 ) ||
		        ( unsigned int )v4 >= *( unsigned int* )( ( uintptr_t )ecx + v7 + 1 ) ) {
			v7 += 2;
			if ( v7 >= v6 )
				return 0;
		}
	} else {
		while ( ( unsigned int )addy < *( unsigned int* )( ( uintptr_t )ecx + v3 ) ||
		        ( unsigned int )addy >= *( unsigned int* )( ( uintptr_t )ecx + v3 + 1 ) ) {
			v3 += 2;
			if ( v3 >= v5 )
				goto LABEL_5;
		}
	}
	return 1;
}