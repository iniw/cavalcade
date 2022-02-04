#include "ctx.hpp"
#include "../entity_cacher/entity_cacher.hpp"
namespace lua {
	struct dbg { };
	struct mem { };
	struct render { };
	struct ctx { };
	struct vec2 {
		constexpr vec2( ) = default;
		constexpr vec2( f32 x, f32 y ) : x( x ), y( y ) { }
		constexpr vec2( const vec2& v ) : x( v.x ), y( v.y ) { }
		f32 x{ }, y{ };
	};
	struct vec {
		constexpr vec( ) = default;
		constexpr vec( f32 x, f32 y, f32 z ) : x( x ), y( y ), z( z ) { }
		constexpr vec( const vec& v ) : x( v.x ), y( v.y ), z( v.z ) { }

		f32 x{ }, y{ }, z{ };
	};

	struct bounds {
		constexpr bounds( ) = default;
		constexpr bounds( const vec& a, const vec& b ) : a( a ), b( b ) { }
		constexpr bounds( const bounds& a ) : a( a.a ), b( a.b ) { }

		vec a{ }, b{ };
	};
} // namespace lua

int my_exception_handler( lua_State* L, sol::optional< const std::exception& > maybe_exception, sol::string_view description ) {
	// L is the lua state, which you can wrap in a state_view if necessary
	// maybe_exception will contain exception, if it exists
	// description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
	if ( maybe_exception ) {
		const std::exception& ex = *maybe_exception;
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "cavalcade" ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ),
		                                      io::format( XOR( "(straight from the exception): {}\n" ), ex.what( ) ).c_str( ) );
	} else {
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "cavalcade" ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ),
		                                      io::format( XOR( "(from the description parameter): {}\n" ), description.data( ) ).c_str( ) );
	}

	// you must push 1 element onto the stack to be
	// transported through as the error object in Lua
	// note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
	// so we push a single string (in our case, the description of the error)
	return sol::stack::push( L, description );
}

inline void my_panic( sol::optional< std::string > maybe_msg ) {
	if ( maybe_msg ) {
		const std::string& msg = maybe_msg.value( );
		std::cerr << "\terror message: " << msg << std::endl;
	}
	// When this function exits, Lua will exhibit default behavior and abort()
}

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

void cavalcade::lua_impl::push( std::string_view code ) {
	// std::unique_lock lock( m_mutex );

	auto state = sol::state{ };
	auto map   = std::unordered_map< std::string, std::vector< sol::protected_function > >{ };
	// Initialize callbacks dictionary
	map[ XOR( "FrameStageNotify" ) ]    = { };
	map[ XOR( "CreateMove" ) ]          = { };
	map[ XOR( "LevelInitPreEntity" ) ]  = { };
	map[ XOR( "LevelInitPostEntity" ) ] = { };
	map[ XOR( "LevelShutdown" ) ]       = { };
	// dummy_map[ "EndScene" ]         = { };

	state.open_libraries( sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::bit32, sol::lib::os, sol::lib::jit,
	                      sol::lib::ffi );
	state.set_exception_handler( &my_exception_handler );

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
	}
	state.new_usertype< ::lua::vec2 >( XOR( "Vector2" ),
	                                   sol::constructors< ::lua::vec2( ), ::lua::vec2( f32, f32 ), ::lua::vec2( const ::lua::vec2& ) >( ),
	                                   XOR( "m_X" ), &::lua::vec2::x, XOR( "m_Y" ), &::lua::vec2::y );
	state.new_usertype< ::lua::vec >( XOR( "Vector3" ),
	                                  sol::constructors< ::lua::vec( ), ::lua::vec( f32, f32, f32 ), ::lua::vec( const ::lua::vec& ) >( ),
	                                  XOR( "m_X" ), &::lua::vec::x, XOR( "m_Y" ), &::lua::vec::y, XOR( "m_Z" ), &::lua::vec::z );
	state.new_usertype< ::lua::bounds >(
		XOR( "Bounds" ),
		sol::constructors< ::lua::bounds( ), ::lua::bounds( const ::lua::vec&, const ::lua::vec& ), ::lua::bounds( const ::lua::bounds& ) >( ),
		XOR( "m_Mins" ), &::lua::bounds::a, XOR( "m_Maxs" ), &::lua::bounds::b );
	// Rendering
	{
		state.new_usertype< render::color >( XOR( "Color" ),
		                                     sol::constructors< render::color( ), render::color( u8, u8, u8, u8 ), render::color( u32 ) >( ),
		                                     XOR( "ToU32" ), &render::color::to_u32, XOR( "FracAlpha" ), &render::color::frac_alpha );
		state.new_usertype< ::lua::render >( XOR( "_Render" ) );
		state[ XOR( "_Render" ) ][ XOR( "Rect" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, const render::color& clr ) {
			g_render.m_safe.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), clr, t );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectForward" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, const render::color& clr ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), clr, t );
		};
		state[ XOR( "_Render" ) ][ XOR( "Line" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, const render::color& clr ) {
			g_render.m_safe.draw_shape< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), clr, t );
		};
		state[ XOR( "_Render" ) ][ XOR( "LineForward" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, const render::color& clr ) {
			g_render.m_safe.draw_shape_front< render::geometry::line >( render::point( x, y ), render::point( x2, y2 ), clr, t );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectFilled" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, const render::color& clr ) {
			g_render.m_safe.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), clr );
		};
		state[ XOR( "_Render" ) ][ XOR( "RectFilledForward" ) ] = [ & ]( i32 x, i32 y, i32 x2, i32 y2, const render::color& clr ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), clr );
		};
		state[ XOR( "_Render" ) ][ XOR( "Text" ) ] = [ & ]( i32 x, i32 y, std::string&& text, u32 font, const render::color& clr ) {
			g_render.m_safe.draw_shape< render::geometry::text >( &g_render.m_fonts[ font ], render::point( x, y ), std::move( text ), clr );
		};
		state[ XOR( "_Render" ) ][ XOR( "TextForward" ) ] = [ & ]( i32 x, i32 y, std::string&& text, u32 font, const render::color& clr ) {
			g_render.m_safe.draw_shape_front< render::geometry::text >( &g_render.m_fonts[ font ], render::point( x, y ), std::move( text ), clr );
		};
		state[ XOR( "_Render" ) ][ XOR( "WorldToScreen" ) ] = [ & ]( ::lua::vec v ) {
			math::v3f out{ };
			if ( g_csgo.m_debug_overlay->screen_position( *( math::v3f* )&v, out ) )
				return ::lua::vec2( 0, 0 );

			return ::lua::vec2( out[ 0 ], out[ 1 ] );
		};
		state.script( XOR( "g_Render = _Render.new()" ) );
	}

	// Context
	{
		state.new_usertype< sdk::user_cmd >(
			XOR( "UserCmd" ), XOR( "m_CommandNumber" ), &sdk::user_cmd::m_command_number, XOR( "m_TickCount" ), &sdk::user_cmd::m_tick_count,
			XOR( "m_ViewAngles" ), ( ::lua::vec( sdk::user_cmd::* ) ) & sdk::user_cmd::m_view_angles, XOR( "m_AimDirection" ),
			( ::lua::vec( sdk::user_cmd::* ) ) & sdk::user_cmd::m_aim_direction, XOR( "m_ForwardMove" ), &sdk::user_cmd::m_forward_move,
			XOR( "m_SideMove" ), &sdk::user_cmd::m_side_move, XOR( "m_UpMove" ), &sdk::user_cmd::m_up_move, XOR( "m_Buttons" ),
			&sdk::user_cmd::m_buttons, XOR( "m_Impulse" ), &sdk::user_cmd::m_impulse, XOR( "m_WeaponSelect" ), &sdk::user_cmd::m_weapon_select,
			XOR( "m_WeaponSubType" ), &sdk::user_cmd::m_weapon_sub_type, XOR( "m_RandomSeed" ), &sdk::user_cmd::m_random_seed, XOR( "m_MousedX" ),
			&sdk::user_cmd::m_moused_x, XOR( "m_Moused_Y" ), &sdk::user_cmd::m_moused_y );
		state[ XOR( "UserCmd" ) ][ XOR( "GetChecksum" ) ] = []( sdk::user_cmd& cmd ) { return cmd.get_checksum( ); };

		// Entity cacher
		state.new_usertype< sdk::auxiliary::player_info_t >(
			XOR( "PlayerInfo" ),
			sol::constructors< sdk::auxiliary::player_info_t( ), sdk::auxiliary::player_info_t( const sdk::auxiliary::player_info_t& ) >( ),
			XOR( "m_Version" ), &sdk::auxiliary::player_info_t::m_version, XOR( "m_SteamId" ), &sdk::auxiliary::player_info_t::m_steam_id,
			XOR( "m_Name" ), &sdk::auxiliary::player_info_t::m_name, XOR( "m_UserId" ), &sdk::auxiliary::player_info_t::m_user_id, XOR( "m_Guid" ),
			&sdk::auxiliary::player_info_t::m_guid, XOR( "m_FriendsId" ), &sdk::auxiliary::player_info_t::m_friends_id, XOR( "m_FriendsName" ),
			&sdk::auxiliary::player_info_t::m_friends_name, XOR( "m_FakePlayer" ), &sdk::auxiliary::player_info_t::m_fake_player, XOR( "m_IsHltv" ),
			&sdk::auxiliary::player_info_t::m_is_hltv, XOR( "m_CustomFiles" ), &sdk::auxiliary::player_info_t::m_custom_files,
			XOR( "m_FilesDownloaded" ), &sdk::auxiliary::player_info_t::m_files_downloaded );
		state.new_usertype< sdk::cs_player >(
			XOR( "CSPlayer" ), XOR( "IsAlive" ), &sdk::cs_player::is_alive, XOR( "GetEyePosition" ),
			[ & ]( sdk::cs_player& pl ) {
				auto vec = pl.get_eye_position( );
				return *( ::lua::vec* )&vec;
			},
			XOR( "GetMinsMaxs" ),
			[ & ]( sdk::cs_player& pl ) {
				math::v3f mins{ }, maxs{ };
				if ( !pl.get_collideable( )->world_space_surrounding_bounds( &mins, &maxs ) )
					return ::lua::bounds{ };
				return ::lua::bounds( *( ::lua::vec* )&mins, *( ::lua::vec* )&maxs );
			},
			XOR( "GetVelocityModifier" ), &sdk::cs_player::get_velocity_modifier, XOR( "IsImmune" ), &sdk::cs_player::is_immune,
			XOR( "IsPlayerGhost" ), &sdk::cs_player::is_player_ghost, XOR( "IsScoped" ), &sdk::cs_player::is_scoped, XOR( "HasHelmet" ),
			&sdk::cs_player::has_helmet, XOR( "GetArmorValue" ), &sdk::cs_player::armor_value, XOR( "CanFireShot" ), &sdk::cs_player::can_fire_shot,
			XOR( "IsEnemy" ), [ & ]( sdk::cs_player& p, sdk::player& rhs ) { return p.is_enemy( &rhs.get( ) ); }, XOR( "GetDuckAmount" ),
			[ & ]( sdk::cs_player& p ) { return p.get_duck_amount( ); }, XOR( "GetDuckSpeed" ),
			[ & ]( sdk::cs_player& p ) { return p.get_duck_speed( ); }, XOR( "GetHealth" ), [ & ]( sdk::cs_player& p ) { return p.get_health( ); },
			XOR( "GetFlags" ), [ & ]( sdk::cs_player& p ) { return p.get_flags( ); }, XOR( "GetTickbase" ),
			[ & ]( sdk::cs_player& p ) { return p.get_tickbase( ); }, XOR( "GetFallVelocity" ),
			[ & ]( sdk::cs_player& p ) { return p.get_fall_velocity( ); }, XOR( "GetViewOffset" ),
			[ & ]( sdk::cs_player& p ) {
				auto what = p.get_view_offset( );
				return *( ::lua::vec* )&what;
			},
			XOR( "GetVelocity" ),
			[ & ]( sdk::cs_player& p ) {
				auto what = p.get_velocity( );
				return *( ::lua::vec* )&what;
			},
			XOR( "GetMoveType" ), [ & ]( sdk::cs_player& p ) { return ( i32 )p.get_move_type( ); }, XOR( "GetEFlags" ),
			[ & ]( sdk::cs_player& p ) { return p.get_eflags( ); }, XOR( "GetLifestate" ), [ & ]( sdk::cs_player& p ) { return p.get_lifestate( ); },
			XOR( "GetOrigin" ),
			[ & ]( sdk::cs_player& p ) {
				auto what = p.get_origin( );
				return *( ::lua::vec* )&what;
			},
			XOR( "GetSimTime" ), [ & ]( sdk::cs_player& p ) { return p.get_sim_time( ); }, XOR( "GetEffects" ),
			[ & ]( sdk::cs_player& p ) { return p.get_effects( ); }, XOR( "GetTeam" ), [ & ]( sdk::cs_player& p ) { return p.get_team( ); } );
		state.new_usertype< sdk::player >(
			XOR( "Player" ), sol::constructors< sdk::player( ), sdk::player( sdk::cs_player* ), sdk::player( const sdk::player& ) >( ),
			XOR( "IsValid" ), &sdk::player::valid, XOR( "GetRef" ), &sdk::player::get, XOR( "GetPlayerInfo" ), &sdk::player::get_player_info );
		state.new_usertype< entity_cacher >( XOR( "_PlayerCache" ), XOR( "ForEach" ), &entity_cacher::for_each, XOR( "GetSize" ),
		                                     []( const entity_cacher& c ) { return c.m_players.size( ); } );

		state.new_usertype< sdk::cvar >(
			XOR( "ConVar" ), XOR( "GetName" ), [ & ]( sdk::cvar& cv ) { return std::string_view{ cv.m_name }; }, XOR( "GetString" ),
			[ & ]( sdk::cvar& cv ) { return cv.get_string( ); }, XOR( "GetFloat" ), &sdk::cvar::get_float, XOR( "GetInt" ), &sdk::cvar::get_int,
			XOR( "SetString" ), [ & ]( sdk::cvar& cv, std::string_view str ) { cv.set_value( str ); }, XOR( "SetFloat" ),
			[ & ]( sdk::cvar& cv, f32 f ) { cv.set_value( f ); }, XOR( "SetInt" ), [ & ]( sdk::cvar& cv, i32 i ) { cv.set_value( i ); } );

		state.new_usertype< sdk::interfaces::cvars >(
			XOR( "_ConVars" ), XOR( "FindVar" ), [ & ]( sdk::interfaces::cvars& cv, std::string&& s ) { return cv.find_var( s ); },
			XOR( "ConsolePrint" ),
			[ & ]( sdk::interfaces::cvars& cv, std::string&& s ) {
				cv.console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
				cv.console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "cavalcade" ) );
				cv.console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
				cv.console_color_printf( render::color( 255, 255, 255, 255 ), ( s + "\n" ).c_str( ) );
			} );

		state.new_usertype< sdk::auxiliary::c_hud_chat >( XOR( "HudChat" ), XOR( "Print" ), [ & ]( sdk::auxiliary::c_hud_chat& h, std::string&& s ) {
			h.chat_printf( 0, 0, io::format( XOR( "<<<NO_TRANSLATE>>> {}" ), std::move( s ) ).c_str( ) );
		} );
		state.new_usertype< sdk::client_mode_shared >( XOR( "_ClientModeShared" ), XOR( "m_ChatElement" ), &sdk::client_mode_shared::m_chat_element );
		state.new_usertype< sdk::globals >(
			XOR( "_Globals" ), XOR( "m_RealTime" ), &sdk::globals::m_realtime, XOR( "m_FrameCount" ), &sdk::globals::m_framecount,
			XOR( "m_AbsoluteFrameTime" ), &sdk::globals::m_absoluteframetime, XOR( "m_AbsoluteFrameStartTime" ),
			&sdk::globals::m_absoluteframestarttime, XOR( "m_CurTime" ), &sdk::globals::m_curtime, XOR( "m_FrameTime" ), &sdk::globals::m_frametime,
			XOR( "m_MaxClients" ), &sdk::globals::m_max_clients, XOR( "m_TickCount" ), &sdk::globals::m_tickcount, XOR( "m_IntervalPerTick" ),
			&sdk::globals::m_interval_per_tick, XOR( "m_InterpolationAmount" ), &sdk::globals::m_interpolation_amount, XOR( "m_SimTicksThisFrame" ),
			&sdk::globals::m_sim_ticks_this_frame );

		// Global important variables
		state.script( XOR( "g_Cmd = UserCmd.new()" ) );
		state.script( XOR( "g_FrameStage = 0" ) );
		state.set( XOR( "g_PlayerCache" ), &g_entity_cacher );
		state.set( XOR( "g_Local" ), &g_ctx.m_local );
		state.set( XOR( "g_ConVars" ), g_csgo.m_cvars );
		state.set( XOR( "g_ClientModeShared" ), g_csgo.m_client_mode_shared );
		state.set( XOR( "g_Globals" ), g_csgo.m_globals );

		// Global enums
		state.script( R"(
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

        Fonts = {
            MENU = 0,
            ESP = 1,
            INDICATOR_BIG = 2,
            INDICATOR_SMALL = 3
        }

        FrameStages = {
            UNDEFINED = -1,
            START = 0,
            NET_UPDATE_START = 1,
            NET_UPDATE_POSTDATAUPDATE_START = 2,
	    	NET_UPDATE_POSTDATAUPDATE_END = 3,
	    	NET_UPDATE_END = 4,
	    	RENDER_START = 5,
	    	RENDER_END = 6
        }

        MoveTypes = {
            NONE = 0,
            ISOMETRIC = 1,
            WALK = 2,
            STEP = 3,
            FLY = 4,
            FLY_GRAVITY = 5,
            VPHYSICS = 6,
            PUSH = 7,
            NOCLIP = 8,
            LADDER = 9,
            OBSERVER = 10,
            CUSTOM = 11
        }
        
        local bit = require('bit')
        Flags = {
            ONGROUND = bit.lshift(1, 0),
            DUCKING = bit.lshift(1, 1),
            ANIMDUCKING = bit.lshift(1, 2),
            WATERJUMP = bit.lshift(1, 3),
            ONTRAIN = bit.lshift(1, 4),
            INRAIN = bit.lshift(1, 5),
            FROZEN = bit.lshift(1, 6),
            ATCONTROLS = bit.lshift(1, 7),
            CLIENT = bit.lshift(1, 8),
            FAKECLIENT = bit.lshift(1, 9),
            INWATER = bit.lshift(1, 10),
            FLY = bit.lshift(1, 11),
            SWIM = bit.lshift(1, 12),
            CONVEYOR = bit.lshift(1, 13),
            NPC = bit.lshift(1, 14),
            GODMODE = bit.lshift(1, 15),
            NOTARGET = bit.lshift(1, 16),
            AIMTARGET = bit.lshift(1, 17),
            PARTIALGROUND = bit.lshift(1, 18),
            STATICPROP = bit.lshift(1, 19),
            GRAPHED = bit.lshift(1, 20),
            GRENADE = bit.lshift(1, 21),
            STEPMOVEMENT = bit.lshift(1, 22),
            DONTTOUCH = bit.lshift(1, 23),
            BASEVELOCITY = bit.lshift(1, 24),
            WORLDBRUSH = bit.lshift(1, 25),
            OBJECT = bit.lshift(1, 26),
            KILLME = bit.lshift(1, 27),
            ONFIRE = bit.lshift(1, 28),
            DISSOLVING = bit.lshift(1, 29),
            TRANSRAGDOLL = bit.lshift(1, 30),
            UNBLOCKABLE_BY_PLAYER = bit.lshift(1, 31)
        }

        LifeStates = {
            ALIVE = 0,
            DYING = 1,
            DEAD = 2,
            RESPAWNABLE = 3,
            DISCARDBODY = 4
        }

        Teams = {
            UNDEFINED = -1,
            UNASSIGNED = 0,
            SPECTATOR = 1,
            TERRORIST = 2,
            COUNTER_TERRORIST = 3
        }
        )" );

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

		state[ XOR( "_Ctx" ) ][ XOR( "GetMapName" ) ] = [ & ]( ) { return g_csgo.m_map_name; };
		state[ XOR( "_Ctx" ) ][ XOR( "GetSkyName" ) ] = [ & ]( ) { return g_csgo.m_sky_name; };

		state[ XOR( "_Ctx" ) ][ XOR( "PushCallback" ) ] = [ & ]( std::string&& at, sol::protected_function&& what ) {
			if ( map.contains( at ) ) {
				map[ at ].push_back( what );
			} else {
				g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
				g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "cavalcade" ) );
				g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
				g_csgo.m_cvars->console_color_printf(
					render::color( 255, 255, 255, 255 ),
					io::format( XOR( "Trying to register callback for \"{}\", not present in callbacks dictionary" ), at ).c_str( ) );
			}
		};
		state.script( XOR( "g_Ctx = _Ctx.new()" ) );
	}

	auto load = state.load( code );

	if ( !load.valid( ) ) {
		sol::error err = load;
		g_io.log( XOR( "{} {}" ), ( i32 )load.status( ), err.what( ) );
		// alert...
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "cavalcade" ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
		g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ),
		                                      io::format( XOR( "Error status: {}, error: {}" ), ( i32 )load.status( ), err.what( ) ).c_str( ) );
		return;
	}

	// Initialize
	load( );

	m_callbacks.emplace_back( std::move( state ), std::move( map ) );
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

	g_io.log( XOR( "initialized ctx" ) );

	return true;
}