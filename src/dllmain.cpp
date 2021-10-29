#include "other/other.hpp"

BOOL APIENTRY DllMain( HMODULE module_handle, DWORD call_reason, LPVOID reserved ) {
	if ( call_reason == DLL_PROCESS_ATTACH ) {
		LI_FN( DisableThreadLibraryCalls )( module_handle );

		return utils::create_thread( cavalcade::init, module_handle );
	}

	return TRUE;
}