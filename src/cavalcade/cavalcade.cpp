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
            local math = require('math')
            local bit = require('bit32')

            state = false
            addy = g_Memory.PatternScan('client.dll', '55 8B EC', '.text')
            cvar = g_ConVars:FindVar('sv_cheats')
            local function hello(stage)
                if (state ~= true) then
                    g_ConVars:ConsolePrint(string.format('%x', addy))
                    g_ConVars:ConsolePrint(string.format('%s %d', cvar:GetName(), cvar:GetInt()))
                    cvar:SetInt(3)
                    g_ConVars:ConsolePrint(string.format('%s %d %d', cvar:GetName(), cvar:GetInt(), Flags.ANIMDUCKING))
                    state = true
                end

                --a = Vector3.new(1.2, 2.2, 3.3)
                --b = a + a
                --g_ConVars:ConsolePrint('x ' .. b.m_X)

                --g_Render.RectFilled(10, 10, 30, 30, Color.new(255, 0, 255, 255))
                --g_Render.RectFilled(10, 50, 30, 100, Color.new(0xffff00ff))
                g_Render.Text(10, 10, 'Hello World ' .. stage .. ' ' .. g_Globals.m_CurTime, Fonts.MENU, Color.new(0xffffffff))
            end

            local function hello_again(stage)
                if (g_Local:IsValid() and g_Local:GetRef():IsAlive()) then
                    g_PlayerCache:ForEach(function (player) 
                        if (player:GetRef():IsEnemy(g_Local)) then
                            team_str = ''
                            if (player:GetRef():GetTeam() == Teams.TERRORIST) then
                                team_str = 'Terrorist'
                            else 
                                team_str = 'Counter-Terrorist'
                            end
                            g_ConVars:ConsolePrint(string.format('-- %s %d %d %s', player:GetPlayerInfo().m_Name, player:GetRef():GetHealth(), player:GetRef():GetMoveType(), team_str))
                        end
                    end)

                    g_ClientModeShared.m_ChatElement:Print('Hello World ' .. g_Globals.m_CurTime)
                end
            end

            local function create_move(cmd)
               if (g_Cmd ~= nil) then
                    vec = Vector3.new(g_Cmd.m_ViewAngles)
                    g_Debug.Print('x: ' .. vec.m_X .. ' y ' .. vec.m_Y .. ' z ' .. vec.m_Z)
                    g_Cmd.m_ViewAngles.m_X = 57.0
                else
                    g_Debug.Print('is nil')
                end
            end

            g_Ctx.PushCallback('FrameStageNotify', hello)
            g_Ctx.PushCallback('FrameStageNotify', hello_again)
            g_Ctx.PushCallback('CreateMove', create_move)
            g_Ctx.PushCallback('LevelInitPreEntity', function (map)
                g_ConVars:ConsolePrint(string.format('%s %s %s', g_Ctx.GetMapName(), map, g_Ctx.GetSkyName()))
            end)
            g_Ctx.PushCallback('LevelInitPostEntity', function ()
                g_ConVars:ConsolePrint(string.format('%s %s', g_Ctx.GetMapName(), g_Ctx.GetSkyName()))
            end)
            g_Ctx.PushCallback('LevelShutdown', function ()
                if (g_Ctx.GetMapName() == nil) then
                    g_ConVars:ConsolePrint('nil')
                end
            end)
           -- g_Ctx.PushCallback('CreateMove', create_move)
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