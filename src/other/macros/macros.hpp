#ifndef MACROS_HPP
#define MACROS_HPP

// used to convert an enum class value to size_t
// to access arrays and stuff
#define ENUM_IDX( enum ) static_cast< size_t >( enum )

#define HASH_RT( str ) hash::xxhash::gen( str, hash::SEED )
#define HASH_CT( str ) utils::cx::data< hash::xxhash::gen( str, hash::SEED ) >::value

#define CONCAT_IMPL( A, B ) A##B
#define CONCAT( A, B )      CONCAT_IMPL( A, B )
#define PAD( size )                                                                                                                                  \
private:                                                                                                                                             \
                                                                                                                                                     \
	u8 CONCAT( __pad, __COUNTER__ )[ size ];                                                                                                         \
                                                                                                                                                     \
public:

#define VFUNC( type, name, idx, args, ... )                                                                                                          \
	type name args {                                                                                                                                 \
		return mem::call_v_func< type, idx >( this, __VA_ARGS__ );                                                                                   \
	}

#define OFFSET_THIS( type ) *reinterpret_cast< type* >( reinterpret_cast< ptr >( this ) + offset )

#define PTR_OFFSET_THIS( type ) reinterpret_cast< type* >( reinterpret_cast< ptr >( this ) + offset )

#define OFFSET( type, name, value )                                                                                                                  \
	type& name( ) {                                                                                                                                  \
		static const auto offset = value;                                                                                                            \
		return OFFSET_THIS( type );                                                                                                                  \
	}

#define PTR_OFFSET( type, name, value )                                                                                                              \
	type* name( ) {                                                                                                                                  \
		static const auto offset = value;                                                                                                            \
		return PTR_OFFSET_THIS( type );                                                                                                              \
	}

#define NETVAR( type, name, netvar ) OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) )

#define PNETVAR( type, name, netvar ) PTR_OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) )

#define NETVAR_OFFSET( type, name, netvar, offset ) OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) + offset )

#define PNETVAR_OFFSET( type, name, netvar, offset ) PTR_OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) + offset )

#define DATAFIELD( type, name, netvar ) OFFSET( type, name, g_netvars.find_in_datamap( get_data_map( ), HASH_CT( netvar ) ) )

#define DATAFIELD_PRED( type, name, netvar ) OFFSET( type, name, g_netvars.find_in_datamap( get_pred_data_map( ), HASH_CT( netvar ) ) )

#define CLIENT_DLL              HASH_CT( "client.dll" )
#define ENGINE_DLL              HASH_CT( "engine.dll" )
#define LOCALIZE_DLL            HASH_CT( "localize.dll" )
#define MATERIALSYSTEM_DLL      HASH_CT( "materialsystem.dll" )
#define VGUI_DLL                HASH_CT( "vguimatsurface.dll" )
#define VGUI2_DLL               HASH_CT( "vgui2.dll" )
#define SHADERAPIDX9_DLL        HASH_CT( "shaderapidx9.dll" )
#define GAMEOVERLAYRENDERER_DLL HASH_CT( "gameoverlayrenderer.dll" )
#define PHYSICS_DLL             HASH_CT( "vphysics.dll" )
#define VSTDLIB_DLL             HASH_CT( "vstdlib.dll" )
#define TIER0_DLL               HASH_CT( "tier0.dll" )
#define INPUTSYSTEM_DLL         HASH_CT( "inputsystem.dll" )
#define STUDIORENDER_DLL        HASH_CT( "studiorender.dll" )
#define DATACACHE_DLL           HASH_CT( "datacache.dll" )
#define STEAM_API_DLL           HASH_CT( "steam_api.dll" )
#define MATCHMAKING_DLL         HASH_CT( "matchmaking.dll" )
#define SERVER_DLL              HASH_CT( "server.dll" )
#define SERVERBROWSER_DLL       HASH_CT( "serverbrowser.dll" )

#endif // MACROS_HPP