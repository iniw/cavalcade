#include "ctx.hpp"

void cavalcade::ctx::lua::push( std::string_view code ) {
	auto dummy_state = sol::state{ };
	auto dummy_map   = std::unordered_map< std::string, std::vector< std::function< void( ) > > >{ };
	// Initialize callbacks dictionary
	dummy_map[ "FrameStageNotify" ] = { };
	dummy_map[ "EndScene" ]         = { };

	auto& [ state, map ] = m_callbacks.emplace_back( std::move( dummy_state ), std::move( dummy_map ) );

	state.open_libraries( sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::jit, sol::lib::ffi );

	// Initialize Lua locals
	// Debugging
	{
		state.set_function( "Caval_DbgPrint", [ & ]( std::string&& what ) { g_io.log( XOR( "{}" ), what ); } );
	}

	// Rendering
	{
		state.set_function( "Caval_Rect", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
			g_render.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ), t );
		} );

		state.set_function( "Caval_RectHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
			g_render.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		} );

		state.set_function( "Caval_RectFilled", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u8 r, u8 g, u8 b, u8 a ) {
			g_render.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ) );
		} );

		state.set_function( "Caval_RectFilledHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u32 rgba ) {
			g_render.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ) );
		} );

		state.set_function( "Caval_SafeRect", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ), t );
		} );

		state.set_function( "Caval_SafeRectHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
			g_render.m_safe.draw_shape< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		} );

		state.set_function( "Caval_SafeRectForward", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( r, g, b, a ),
			                                                            t );
		} );

		state.set_function( "Caval_SafeRectForwardHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, f32 t, u32 rgba ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ), t );
		} );

		state.set_function( "Caval_SafeRectFilled", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ),
			                                                             render::color( r, g, b, a ) );
		} );

		state.set_function( "Caval_SafeRectFilledHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u32 rgba ) {
			g_render.m_safe.draw_shape< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ), render::color( rgba ) );
		} );

		state.set_function( "Caval_SafeRectFilledForward", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u8 r, u8 g, u8 b, u8 a ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ),
			                                                                   render::color( r, g, b, a ) );
		} );

		state.set_function( "Caval_SafeRectFilledForwardHex", [ & ]( i32 x, i32 y, i32 x2, i32 y2, u32 rgba ) {
			g_render.m_safe.draw_shape_front< render::geometry::rect_filled >( render::point( x, y ), render::point( x2, y2 ),
			                                                                   render::color( rgba ) );
		} );
	}

	// Context
	{
		state.set_function( "Caval_PushCallback", [ & ]( std::string&& at, std::function< void( ) >&& what ) {
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
            string = 'Hello from Lua'
            local function hello()
                Caval_DbgPrint(string)
                Caval_SafeRectFilled(10, 10, 30, 30, 255, 0, 0, 255)
                Caval_SafeRectFilledHex(10, 50, 30, 100, 0xff00ffff)
            end

            local function hello_again()
                Caval_DbgPrint('Hello from Lua Second Callback in same script')
            end

            local function end_scene()
                Caval_RectFilled(50, 50, 100, 100, 255, 255, 255, 255)
            end


            Caval_PushCallback('FrameStageNotify', hello)
            Caval_PushCallback('FrameStageNotify', hello_again)
            Caval_PushCallback('EndScene', end_scene)
        )" );

	g_io.log( XOR( "initialized ctx" ) );

	return true;
}