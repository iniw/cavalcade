#include "cavalcade.hpp"

#include "ctx/ctx.hpp"
#include "hooks/hooks.hpp"

DWORD WINAPI cavalcade::init( unk module_handle ) {
	handle = static_cast< HMODULE >( module_handle );
	window = LI_FN( FindWindowA )( XOR( "Valve001" ), nullptr );

	H_ENFORCE( g_io.init( ), "failed to initialize io" );

	H_ENFORCE( g_mem.init( ), "failed to initialize mem" );

	H_ENFORCE( g_csgo.init( ), "failed to initialize csgo" );

	H_ENFORCE( g_mem.setup( ), "failed to initialize addresses" );

	H_ENFORCE( g_netvars.init( ), "failed to initialize netvars" );

	H_ENFORCE( g_ctx.init( ), "failed to initialize ctx" );

	H_ENFORCE( g_render.init( ), "failed to initialize renderer" );

	H_ENFORCE( g_hooks.init( ), "failed to initialize hooks" );

	g_lua.push( R"(
            local string = require('string')

            state = false
            addy = g_Memory.PatternScan('client.dll', '55 8B EC', '.text')
            local function hello()
                if (state ~= true) then
                    g_Debug.Print(string.format('%x', addy))
                    state = true
                end
                
                g_Render.RectFilled(10, 10, 30, 30, Color.new(255, 0, 255, 255))
                g_Render.RectFilled(10, 50, 30, 100, Color.new(0xffff00ff))
                -- g_Debug.Print(string.format('%x', Color.new(255, 255, 0, 255):ToU32()))
            end

            local function player_stuff(player)
                if (player:GetRef():IsAlive()) then
                    --g_Debug.Print('Alive: ' .. player:GetPlayerInfo().m_Name)
                else
                    --g_Debug.Print('Dead: ' .. player:GetPlayerInfo().m_Name)
                end

                if (player == g_Local) then
                    --g_Debug.Print('dis local!')
                end
            end

            local function hello_again()
                --g_PlayerCache:ForEach(function (player) player_stuff(player) end)
            end

            local function create_move()
               -- if (g_Cmd ~= nil) then
             --       vec = Vector3.new(g_Cmd.m_ViewAngles)
           --         g_Debug.Print('x: ' .. vec.m_X .. ' y ' .. vec.m_Y .. ' z ' .. vec.m_Z)
                    --g_Cmd.m_ViewAngles.m_X = 57.0
                --else
                  --  g_Debug.Print('is nil')
                --end
            end

            g_Ctx.PushCallback('FrameStageNotify', hello)
            g_Ctx.PushCallback('FrameStageNotify', hello_again)
            g_Ctx.PushCallback('CreateMove', create_move)
        )" );

#ifdef _DEBUG

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

	LI_FN( FreeLibraryAndExitThread )( handle, exit_type );

	return exit_type;
}