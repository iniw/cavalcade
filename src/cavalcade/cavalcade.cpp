#include "cavalcade.hpp"

#include "ctx/ctx.hpp"
#include "hooks/hooks.hpp"

DWORD WINAPI cavalcade::init( unk module_handle ) {
	handle = static_cast< HMODULE >( module_handle );
	window = LI_FN( FindWindowA )( XOR( "Valve001" ), nullptr );

	HENFORCE( g_io.init( ), "failed to initialize io" );

	HENFORCE( g_mem.init( ), "failed to initialize mem" );

	HENFORCE( g_csgo.init( ), "failed to initialize csgo" );

	HENFORCE( g_mem.setup( ), "failed to initialize addresses" );

	HENFORCE( g_netvars.init( ), "failed to initialize netvars" );

	HENFORCE( g_ctx.init( ), "failed to initialize ctx" );

	HENFORCE( g_render.init( ), "failed to initialize renderer" );

	HENFORCE( g_hooks.init( ), "failed to initialize hooks" );

	g_gui.init( );

#ifdef DEV

	while ( !g_io.key_state( VK_END ) )
		utils::sleep( 50 );

	return unload( EXIT_SUCCESS );

#endif

	return EXIT_SUCCESS;
}

DWORD WINAPI cavalcade::unload( BOOL exit_type ) {
	g_mem.unload( );

	g_render.unload( );

	g_io.unload( );

	LI_FN( FreeLibraryAndExitThread )
	( handle, exit_type );

	return exit_type;
}