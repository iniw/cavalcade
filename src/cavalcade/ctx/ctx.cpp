#include "ctx.hpp"

namespace lua {
	struct dbg { };
	struct mem { };
	struct render { };
	struct ctx { };
	struct vec {
		constexpr vec( ) = default;
		constexpr vec( f32 x, f32 y, f32 z ) : x( x ), y( y ), z( z ) { }
		constexpr vec( const vec& v ) : x( v.x ), y( v.y ), z( v.z ) { }
		f32 x{ }, y{ }, z{ };
	};
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
		state.new_usertype< ::lua::vec >( XOR( "Vector3" ),
		                                  sol::constructors< ::lua::vec( ), ::lua::vec( f32, f32, f32 ), ::lua::vec( const ::lua::vec& ) >( ),
		                                  XOR( "x" ), &::lua::vec::x, XOR( "y" ), &::lua::vec::y, XOR( "z" ), &::lua::vec::z );
		state.new_usertype< sdk::user_cmd >(
			XOR( "UserCmd" ), XOR( "m_CommandNumber" ), &sdk::user_cmd::m_command_number, XOR( "m_TickCount" ), &sdk::user_cmd::m_tick_count,
			XOR( "m_ViewAngles" ), ( ::lua::vec( sdk::user_cmd::* ) ) & sdk::user_cmd::m_view_angles, XOR( "m_AimDirection" ),
			( ::lua::vec( sdk::user_cmd::* ) ) & sdk::user_cmd::m_aim_direction, XOR( "m_ForwardMove" ), &sdk::user_cmd::m_forward_move,
			XOR( "m_SideMove" ), &sdk::user_cmd::m_side_move, XOR( "m_UpMove" ), &sdk::user_cmd::m_up_move, XOR( "m_Buttons" ),
			&sdk::user_cmd::m_buttons, XOR( "m_Impulse" ), &sdk::user_cmd::m_impulse, XOR( "m_WeaponSelect" ), &sdk::user_cmd::m_weapon_select,
			XOR( "m_WeaponSubType" ), &sdk::user_cmd::m_weapon_sub_type, XOR( "m_RandomSeed" ), &sdk::user_cmd::m_random_seed, XOR( "m_MousedX" ),
			&sdk::user_cmd::m_moused_x, XOR( "m_Moused_Y" ), &sdk::user_cmd::m_moused_y );

		state.script( XOR( "g_Cmd = UserCmd.new()" ) );
		state.script( XOR( "g_FrameStage = 0" ) );

		state.script( XOR( R"(
        Keys = {
            VK_LBUTTON = 0x01,
            VK_RBUTTON = 0x02,
            VK_CANCEL = 0x03,
            VK_MBUTTON = 0x04, -- NOT contiguous with L &amp; RBUTTON
            VK_XBUTTON1 = 0x05, -- NOT contiguous with L &amp; RBUTTON
            VK_XBUTTON2 = 0x06, -- NOT contiguous with L &amp; RBUTTON
            VK_BACK = 0x08,
            VK_TAB = 0x09,
            VK_CLEAR = 0x0C,
            VK_RETURN = 0x0D,
            VK_SHIFT = 0x10,
            VK_CONTROL = 0x11,
            VK_MENU = 0x12,
            VK_PAUSE = 0x13,
            VK_CAPITAL = 0x14,
            VK_KANA = 0x15,
            VK_HANGEUL = 0x15, -- old name - should be here for compatibility
            VK_HANGUL = 0x15,
            VK_JUNJA = 0x17,
            VK_FINAL = 0x18,
            VK_HANJA = 0x19,
            VK_KANJI = 0x19,
            VK_ESCAPE = 0x1B,
            VK_CONVERT = 0x1C,
            VK_NONCONVERT = 0x1D,
            VK_ACCEPT = 0x1E,
            VK_MODECHANGE = 0x1F,
            VK_SPACE = 0x20,
            VK_PRIOR = 0x21,
            VK_NEXT = 0x22,
            VK_END = 0x23,
            VK_HOME = 0x24,
            VK_LEFT = 0x25,
            VK_UP = 0x26,
            VK_RIGHT = 0x27,
            VK_DOWN = 0x28,
            VK_SELECT = 0x29,
            VK_PRINT = 0x2A,
            VK_EXECUTE = 0x2B,
            VK_SNAPSHOT = 0x2C,
            VK_INSERT = 0x2D,
            VK_DELETE = 0x2E,
            VK_HELP = 0x2F,
            VK_LWIN = 0x5B,
            VK_RWIN = 0x5C,
            VK_APPS = 0x5D,
            VK_SLEEP = 0x5F,
            VK_NUMPAD0 = 0x60,
            VK_NUMPAD1 = 0x61,
            VK_NUMPAD2 = 0x62,
            VK_NUMPAD3 = 0x63,
            VK_NUMPAD4 = 0x64,
            VK_NUMPAD5 = 0x65,
            VK_NUMPAD6 = 0x66,
            VK_NUMPAD7 = 0x67,
            VK_NUMPAD8 = 0x68,
            VK_NUMPAD9 = 0x69,
            VK_MULTIPLY = 0x6A,
            VK_ADD = 0x6B,
            VK_SEPARATOR = 0x6C,
            VK_SUBTRACT = 0x6D,
            VK_DECIMAL = 0x6E,
            VK_DIVIDE = 0x6F,
            VK_F1 = 0x70,
            VK_F2 = 0x71,
            VK_F3 = 0x72,
            VK_F4 = 0x73,
            VK_F5 = 0x74,
            VK_F6 = 0x75,
            VK_F7 = 0x76,
            VK_F8 = 0x77,
            VK_F9 = 0x78,
            VK_F10 = 0x79,
            VK_F11 = 0x7A,
            VK_F12 = 0x7B,
            VK_F13 = 0x7C,
            VK_F14 = 0x7D,
            VK_F15 = 0x7E,
            VK_F16 = 0x7F,
            VK_F17 = 0x80,
            VK_F18 = 0x81,
            VK_F19 = 0x82,
            VK_F20 = 0x83,
            VK_F21 = 0x84,
            VK_F22 = 0x85,
            VK_F23 = 0x86,
            VK_F24 = 0x87,
            VK_NUMLOCK = 0x90,
            VK_SCROLL = 0x91,
            VK_OEM_NEC_EQUAL = 0x92, -- '=' key on numpad
            VK_OEM_FJ_JISHO = 0x92, -- 'Dictionary' key
            VK_OEM_FJ_MASSHOU = 0x93, -- 'Unregister word' key
            VK_OEM_FJ_TOUROKU = 0x94, -- 'Register word' key
            VK_OEM_FJ_LOYA = 0x95, -- 'Left OYAYUBI' key
            VK_OEM_FJ_ROYA = 0x96, -- 'Right OYAYUBI' key
            VK_LSHIFT = 0xA0,
            VK_RSHIFT = 0xA1,
            VK_LCONTROL = 0xA2,
            VK_RCONTROL = 0xA3,
            VK_LMENU = 0xA4,
            VK_RMENU = 0xA5,
            VK_BROWSER_BACK = 0xA6,
            VK_BROWSER_FORWARD = 0xA7,
            VK_BROWSER_REFRESH = 0xA8,
            VK_BROWSER_STOP = 0xA9,
            VK_BROWSER_SEARCH = 0xAA,
            VK_BROWSER_FAVORITES = 0xAB,
            VK_BROWSER_HOME = 0xAC,
            VK_VOLUME_MUTE = 0xAD,
            VK_VOLUME_DOWN = 0xAE,
            VK_VOLUME_UP = 0xAF,
            VK_MEDIA_NEXT_TRACK = 0xB0,
            VK_MEDIA_PREV_TRACK = 0xB1,
            VK_MEDIA_STOP = 0xB2,
            VK_MEDIA_PLAY_PAUSE = 0xB3,
            VK_LAUNCH_MAIL = 0xB4,
            VK_LAUNCH_MEDIA_SELECT = 0xB5,
            VK_LAUNCH_APP1 = 0xB6,
            VK_LAUNCH_APP2 = 0xB7,
            VK_OEM_1 = 0xBA, -- ';:' for US
            VK_OEM_PLUS = 0xBB, -- '+' any country
            VK_OEM_COMMA = 0xBC, -- ',' any country
            VK_OEM_MINUS = 0xBD, -- '-' any country
            VK_OEM_PERIOD = 0xBE, -- '.' any country
            VK_OEM_2 = 0xBF, -- '/?' for US
            VK_OEM_3 = 0xC0, -- '`~' for US
            VK_OEM_4 = 0xDB, -- '[{' for US
            VK_OEM_5 = 0xDC, -- '\|' for US
            VK_OEM_6 = 0xDD, -- ']}' for US
            VK_OEM_7 = 0xDE, -- ''"' for US
            VK_OEM_8 = 0xDF,
            VK_OEM_AX = 0xE1, -- 'AX' key on Japanese AX kbd
            VK_OEM_102 = 0xE2, -- "<>" or "\|" on RT 102-key kbd.
            VK_ICO_HELP = 0xE3, -- Help key on ICO
            VK_ICO_00 = 0xE4, -- 00 key on ICO
            VK_PROCESSKEY = 0xE5,
            VK_ICO_CLEAR = 0xE6,
            VK_PACKET = 0xE7,
            VK_OEM_RESET = 0xE9,
            VK_OEM_JUMP = 0xEA,
            VK_OEM_PA1 = 0xEB,
            VK_OEM_PA2 = 0xEC,
            VK_OEM_PA3 = 0xED,
            VK_OEM_WSCTRL = 0xEE,
            VK_OEM_CUSEL = 0xEF,
            VK_OEM_ATTN = 0xF0,
            VK_OEM_FINISH = 0xF1,
            VK_OEM_COPY = 0xF2,
            VK_OEM_AUTO = 0xF3,
            VK_OEM_ENLW = 0xF4,
            VK_OEM_BACKTAB = 0xF5,
            VK_ATTN = 0xF6,
            VK_CRSEL = 0xF7,
            VK_EXSEL = 0xF8,
            VK_EREOF = 0xF9,
            VK_PLAY = 0xFA,
            VK_ZOOM = 0xFB,
            VK_NONAME = 0xFC,
            VK_PA1 = 0xFD,
            VK_OEM_CLEAR = 0xFE
        }

        KeyStates = {
            UP = 1,
            DOWN = 2,
            RELEASED = 3
        }
        )" ) );

		state.new_usertype< ::lua::ctx >( XOR( "_Ctx" ) );
		state[ XOR( "_Ctx" ) ][ XOR( "GetKeyState" ) ] = [ & ]( u8 key, i32 key_state ) -> bool {
			switch ( key_state ) {
			case io::key_state::UP:
				return g_io.key_state< io::key_state::UP >( key );
				break;

			case io::key_state::DOWN:
				return g_io.key_state< io::key_state::DOWN >( key );
				break;

			case io::key_state::RELEASED:
				return g_io.key_state< io::key_state::RELEASED >( key );
				break;
			}

			return false;
		};
		state[ XOR( "_Ctx" ) ][ XOR( "PushCallback" ) ] = [ & ]( std::string&& at, std::function< void( ) >&& what ) {
			if ( map.contains( at ) ) {
				map[ at ].push_back( what );
			} // else...
		};
		state.script( XOR( "g_Ctx = _Ctx.new()" ) );
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
                --vec = Vector3.new(1.1, 2.2, 3.3)
                --g_Debug.Print('here')
            end

            local function create_move()
                if (g_Cmd ~= nil) then

                    g_Debug.Print(string.format('%f %f %f', g_Cmd.m_ViewAngles[1],g_Cmd.m_ViewAngles[2],g_Cmd.m_ViewAngles[3]))
                else
                    g_Debug.Print('is nil')
                end
            end

            g_Ctx.PushCallback('FrameStageNotify', hello)
            g_Ctx.PushCallback('FrameStageNotify', hello_again)
            g_Ctx.PushCallback('CreateMove', create_move)
        )" );

	g_io.log( XOR( "initialized ctx" ) );

	return true;
}