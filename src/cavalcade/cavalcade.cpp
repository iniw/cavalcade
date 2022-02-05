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
                --g_Render.Text(10, 10, 'Hello World ' .. stage .. ' ' .. g_Globals.m_CurTime, Fonts.MENU, Color.new(0xffffffff))
            end

            local function hello_again(stage)
                if (g_Local:IsValid() and g_Local:GetRef():IsAlive()) then
                   --g_PlayerCache:ForEach(function (player) 
                   --    if (player:GetRef():IsAlive() and player:GetRef():IsDormant() ~= true and player:GetRef():IsEnemy(g_Local:GetRef())) then
                   --        --if (player:GetRef():GetActiveWeapon():GetWeaponRef() ~= nil) then
                   --            --g_ConVars:ConsolePrint('max clip: ' .. player:GetRef():GetActiveWeapon():GetWeaponRef():GetCSWeaponInfo().m_MaxClip1)
                   --        --end
                   --        
                   --        --local_eye_pos = g_Local:GetRef():GetEyePosition()
                   --        --enemy_head_pos = player:GetRef():GetHitboxPosition(Hitboxes.HEAD)

                   --        --g_ConVars:ConsolePrint('Head pos x ' .. enemy_head_pos.m_X .. ' y ' .. enemy_head_pos.m_Y .. ' z ' .. --enemy_head_pos.m_Z)

                   --        --trace = g_EngineTrace:TraceRay(Ray.new(local_eye_pos, enemy_head_pos), 0x46004009, TraceFilter.new--(g_Local:GetRef()))

                   --        ---- if head visible
                   --        --if (trace.m_Entity == player:GetRef() or trace.m_Fraction > 0.97) then
                   --        --    g_ConVars:ConsolePrint('Player ' .. player:GetPlayerInfo().m_Name .. ' is visible')
                   --        --end
                   --    end
                   --end)

                   for i = 1, g_Globals.m_MaxClients, 1
                   do
                        player = g_EntList:GetPlayerRef(i)
                        if (player ~= nil and player:IsAlive() and player:IsPlayer()) then
                            info = g_EngineClient:GetPlayerInfo(i)
                            g_ConVars:ConsolePrint(info.m_Name)
                        end
                   end

                    --g_ClientModeShared.m_ChatElement:Print('Hello World ' .. g_Globals.m_CurTime)
                end
            end

            local function create_move(cmd)
               --if (cmd ~= nil) then
               --     vec = Vector3.new(g_Cmd.m_ViewAngles)
               --     g_Debug.Print('x: ' .. vec.m_X .. ' y ' .. vec.m_Y .. ' z ' .. vec.m_Z)
               --     cmd.m_ViewAngles.m_X = 57.0
               -- else
               --     g_Debug.Print('is nil')
               -- end
            end

            g_Ctx.PushCallback('FrameStageNotify', hello)
            g_Ctx.PushCallback('FrameStageNotify', hello_again)
            g_Ctx.PushCallback('CreateMove', create_move)
            --g_Ctx.PushCallback('CreateMove', function() g_ConVars:ConsolePrint('hi') end)
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