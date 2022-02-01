#include "ctx.hpp"

namespace lua {
	struct dbg { };
	struct mem { };
	struct render { };
} // namespace lua

static auto pattern_to_bytes( std::string&& pattern ) {
	auto bytes = std::vector< int >{ };
	auto start = const_cast< char* >( pattern.data( ) );
	auto end   = const_cast< char* >( pattern.data( ) ) + pattern.size( );

	for ( auto current = start; current < end; ++current ) {
		if ( *current == '?' ) {
			++current;
			if ( *current == '?' )
				++current;
			bytes.push_back( -1 );
		} else {
			bytes.push_back( strtoul( current, &current, 16 ) );
		}
	}
	return bytes;
};

void cavalcade::ctx::lua::push( std::string_view code ) {
	auto dummy_state = sol::state{ };
	auto dummy_map   = std::unordered_map< std::string, std::vector< std::function< void( ) > > >{ };
	// Initialize callbacks dictionary
	dummy_map[ XOR( "FrameStageNotify" ) ] = { };
	dummy_map[ XOR( "CreateMove" ) ]       = { };
	// dummy_map[ "EndScene" ]         = { };

	auto& [ state, map ] = m_callbacks.emplace_back( std::move( dummy_state ), std::move( dummy_map ) );

	state.open_libraries( sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::os, sol::lib::jit, sol::lib::ffi );

	// Initialize Lua locals
#ifdef _DEBUG
	// Debugging
	{
		state.new_usertype< ::lua::dbg >( XOR( "_Debug" ) );
		state[ XOR( "_Debug" ) ][ XOR( "Print" ) ] = [ & ]( std::string&& text ) { g_io.log( XOR( "{}" ), text ); };
		state.script( XOR( "g_Debug = _Debug.new()" ) );
	}
#endif

	// Memory
	{
		state.new_usertype< ::lua::mem >( XOR( "_Memory" ) );
		state[ XOR( "_Memory" ) ][ XOR( "PatternScan" ) ] = [ & ]( std::string&& mod, std::string&& pattern, std::string&& section ) {
			auto bytes = pattern_to_bytes( std::move( pattern ) );
			auto& rmod = g_mem[ HASH_RT( mod.c_str( ) ) ];
			return ( uint32_t )rmod.search_byte_array( bytes.data( ), bytes.size( ), rmod.m_sections[ HASH_RT( section.c_str( ) ) ] );
		};

		state.script( XOR( "g_Memory = _Memory.new()" ) );

		// state.set_function( XOR( "Caval_PatternScan" ), [ & ]( std::string&& mod, std::string&& pattern, std::string&& section ) {
		// 	auto bytes = pattern_to_bytes( std::move( pattern ) );
		// 	auto& rmod = g_mem[ HASH_RT( mod.c_str( ) ) ];
		// 	return ( uint32_t )rmod.search_byte_array( bytes.data( ), bytes.size( ), rmod.m_sections[ HASH_RT( section.c_str( ) ) ] );
		// } );
	}

	// Rendering
	{
		// NOTE(para): Maybe having callbacks every frame isn't a great idea... Lets keep this to safe rendering only
		// state.set_function( "Caval_Rect", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
		// 	g_render.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ), t );
		// } );

		// state.set_function( "Caval_RectHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
		// 	g_render.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		// } );

		// state.set_function( "Caval_Line", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
		// 	g_render.draw_shape< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ), t );
		// } );

		// state.set_function( "Caval_LineHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
		// 	g_render.draw_shape< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		// } );

		// state.set_function( "Caval_RectFilled", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u8 r, u8 g, u8 b, u8 a ) {
		// 	g_render.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ) );
		// } );

		// state.set_function( "Caval_RectFilledHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u32 rgba ) {
		// 	g_render.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ) );
		// } );

		state.new_usertype< ::lua::render >( XOR( "_Render" ) );
		state[ XOR( "_Render" ) ][ XOR( "Rect" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ), t );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectHex" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
			g_render.m_safe.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectForward" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ),
			                                                            t );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectForwardHex" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		};
		state[ XOR( "_Render" ) ][ XOR( "Line" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ), t );
		};
		state[ XOR( "_Render" ) ][ XOR( "LineHex" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
			g_render.m_safe.draw_shape< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		};
		state[ XOR( "_Render" ) ][ XOR( "LineForward" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape_front< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ),
			                                                            t );
		};
		state[ XOR( "_Render" ) ][ XOR( "LineForwardHex" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
			g_render.m_safe.draw_shape_front< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectFilled" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ),
			                                                             render::color( r, g, b, a ) );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectFilledHex" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, u32 rgba ) {
			g_render.m_safe.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ) );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectFilledForward" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ),
			                                                                   render::color( r, g, b, a ) );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectFilledForwardHex" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, u32 rgba ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ),
			                                                                   render::color( rgba ) );
		};
		state.script( XOR( "g_Render = _Render.new()" ) );
	}

	// Context
	{
		state.new_usertype< sdk::user_cmd >( XOR( "UserCmd" ) );
		state[ XOR( "UserCmd" ) ][ XOR( "GetCommandNumber" ) ]    = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_command_number; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetTickCount" ) ]        = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_tick_count; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetForwardMove" ) ]      = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_forward_move; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetSideMove" ) ]         = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_side_move; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetUpMove" ) ]           = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_up_move; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetButtons" ) ]          = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_buttons; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetImpulse" ) ]          = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_impulse; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetWeaponSelect" ) ]     = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_weapon_select; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetWeaponSubType" ) ]    = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_weapon_sub_type; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetRandomSeed" ) ]       = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_random_seed; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetMousedX" ) ]          = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_moused_x; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetMousedY" ) ]          = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_moused_y; };
		state[ XOR( "UserCmd" ) ][ XOR( "GetHasBeenPredicted" ) ] = [ & ]( sdk::user_cmd& cmd ) { return cmd.m_has_been_predicted; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetCommandNumber" ) ]    = [ & ]( sdk::user_cmd& cmd, i32 command_number ) {
            cmd.m_command_number = command_number;
		};
		state[ XOR( "UserCmd" ) ][ XOR( "SetTickCount" ) ]    = [ & ]( sdk::user_cmd& cmd, i32 tick_count ) { cmd.m_tick_count = tick_count; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetForwardMove" ) ]  = [ & ]( sdk::user_cmd& cmd, f32 forward_move ) { cmd.m_forward_move = forward_move; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetSideMove" ) ]     = [ & ]( sdk::user_cmd& cmd, f32 side_move ) { cmd.m_side_move = side_move; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetUpMove" ) ]       = [ & ]( sdk::user_cmd& cmd, f32 up_move ) { cmd.m_up_move = up_move; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetButtons" ) ]      = [ & ]( sdk::user_cmd& cmd, i32 buttons ) { cmd.m_buttons = buttons; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetImpulse" ) ]      = [ & ]( sdk::user_cmd& cmd, char impulse ) { cmd.m_impulse = impulse; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetWeaponSelect" ) ] = [ & ]( sdk::user_cmd& cmd, i32 weapon_select ) {
			cmd.m_weapon_select = weapon_select;
		};
		state[ XOR( "UserCmd" ) ][ XOR( "SetWeaponSubType" ) ] = [ & ]( sdk::user_cmd& cmd, i32 weapon_sub_type ) {
			cmd.m_weapon_sub_type = weapon_sub_type;
		};
		state[ XOR( "UserCmd" ) ][ XOR( "SetRandomSeed" ) ]       = [ & ]( sdk::user_cmd& cmd, i32 random_seed ) { cmd.m_random_seed = random_seed; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetMousedX" ) ]          = [ & ]( sdk::user_cmd& cmd, i16 moused_x ) { cmd.m_moused_x = moused_x; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetMousedY" ) ]          = [ & ]( sdk::user_cmd& cmd, i16 moused_y ) { cmd.m_moused_y = moused_y; };
		state[ XOR( "UserCmd" ) ][ XOR( "SetHasBeenPredicted" ) ] = [ & ]( sdk::user_cmd& cmd, bool has_been_predicted ) {
			cmd.m_has_been_predicted = has_been_predicted;
		};
		state.script( XOR( "g_Cmd = UserCmd.new()" ) );
		state.script( XOR( "g_FrameStage = 0" ) );

		state.set_function( XOR( "Caval_PushCallback" ), [ & ]( std::string&& at, std::function< void( ) >&& what ) {
			if ( map.contains( at ) ) {
				map[ at ].push_back( what );
			} // else...
		} );
	}

	auto load = state.load( code );

	if ( !load.valid( ) ) {
		g_io.log( XOR( "{}" ), ( i32 )load.status( ) );
		// alert...
		return;
	}

	load( );
}

bool cavalcade::ctx::init( ) {
	MOCKING_TRY;

	MOCK m_cvars.sv_cheats                       = g_csgo.m_cvars->find_var( XOR( "sv_cheats" ) );
	MOCK m_cvars.cl_updaterate                   = g_csgo.m_cvars->find_var( XOR( "cl_updaterate" ) );
	MOCK m_cvars.sv_minupdaterate                = g_csgo.m_cvars->find_var( XOR( "sv_minupdaterate" ) );
	MOCK m_cvars.sv_maxupdaterate                = g_csgo.m_cvars->find_var( XOR( "sv_maxupdaterate" ) );
	MOCK m_cvars.viewmodel_offset_z              = g_csgo.m_cvars->find_var( XOR( "viewmodel_offset_z" ) );
	MOCK m_cvars.cl_interp                       = g_csgo.m_cvars->find_var( XOR( "cl_interp" ) );
	MOCK m_cvars.cl_interp_ratio                 = g_csgo.m_cvars->find_var( XOR( "cl_interp_ratio" ) );
	MOCK m_cvars.sv_client_min_interp_ratio      = g_csgo.m_cvars->find_var( XOR( "sv_client_min_interp_ratio" ) );
	MOCK m_cvars.sv_client_max_interp_ratio      = g_csgo.m_cvars->find_var( XOR( "sv_client_max_interp_ratio" ) );
	MOCK m_cvars.sv_maxunlag                     = g_csgo.m_cvars->find_var( XOR( "sv_maxunlag" ) );
	MOCK m_cvars.sv_showlagcompensation_duration = g_csgo.m_cvars->find_var( XOR( "sv_showlagcompensation_duration" ) );
	MOCK m_cvars.sv_autobunnyhopping             = g_csgo.m_cvars->find_var( XOR( "sv_autobunnyhopping" ) );
	MOCK m_cvars.mp_teammates_are_enemies        = g_csgo.m_cvars->find_var( XOR( "mp_teammates_are_enemies" ) );
	MOCK m_cvars.sensitivity                     = g_csgo.m_cvars->find_var( XOR( "sensitivity" ) );
	MOCK m_cvars.zoom_sensitivity_ratio_mouse    = g_csgo.m_cvars->find_var( XOR( "zoom_sensitivity_ratio_mouse" ) );
	MOCK m_cvars.m_pitch                         = g_csgo.m_cvars->find_var( XOR( "m_pitch" ) );
	MOCK m_cvars.m_yaw                           = g_csgo.m_cvars->find_var( XOR( "m_yaw" ) );

	MOCK m_cvars.sv_skyname = g_csgo.m_cvars->find_var( XOR( "sv_skyname" ) );
	MOCK m_cvars.sv_gravity = g_csgo.m_cvars->find_var( XOR( "sv_gravity" ) );

	MOCK m_steam.m_steam_client = g_csgo.m_engine->get_steam_api_context( )->m_steam_client;

	static HSteamUser steam_user = SteamAPI_GetHSteamUser( );
	static HSteamPipe steam_pipe = SteamAPI_GetHSteamUser( );

	MOCK m_steam.m_steam_user =
		( ISteamUser* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMUSER_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_user_stats =
		( ISteamUserStats* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMUSERSTATS_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_friends =
		( ISteamFriends* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMFRIENDS_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_game_coordinator = ( ISteamGameCoordinator* )( m_steam.m_steam_client->GetISteamGenericInterface(
		steam_pipe, steam_user, STEAMGAMECOORDINATOR_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_matchmaking =
		( ISteamMatchmaking* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMMATCHMAKING_INTERFACE_VERSION ) );
	MOCK m_steam.m_steam_utils =
		( ISteamUtils* )( m_steam.m_steam_client->GetISteamGenericInterface( steam_pipe, steam_user, STEAMUTILS_INTERFACE_VERSION ) );

	MOCKING_CATCH( return false );

	m_steam.m_steam_friends->SetListenForFriendsMessages( true );

	m_lua.push( R"(
            local string = require('string')

            state = false
            addy = g_Memory.PatternScan('client.dll', '55 8B EC', '.text')
            local function hello()
                if (state ~= true) then
                    g_Debug.Print(string.format('%x', addy))
                    state = true
                end
                
                g_Render.RectFilled(10, 10, 30, 30, 255, 0, 0, 255)
                g_Render.RectFilledHex(10, 50, 30, 100, 0xff00ffff)
            end

            local function hello_again()
                --g_Debug.Print('Hello from Lua Second Callback in same script')
            end

            local function create_move()
                if (g_Cmd ~= nil) then
                    g_Debug.Print(string.format('%d', g_Cmd:GetCommandNumber()))
                else
                    g_Debug.Print('is nil')
                end
            end

            Caval_PushCallback('FrameStageNotify', hello)
            Caval_PushCallback('FrameStageNotify', hello_again)
            Caval_PushCallback('CreateMove', create_move)
        )" );

	g_io.log( XOR( "initialized ctx" ) );

	return true;
}