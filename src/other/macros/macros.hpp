#ifndef MACROS_HPP
#define MACROS_HPP

#define CONCAT_IMPL( A, B ) A##B
#define CONCAT( A, B )      CONCAT_IMPL( A, B )
#define PAD( size )                                                                                                              \
private:                                                                                                                         \
                                                                                                                                 \
	u8 CONCAT( __pad, __COUNTER__ )[ size ];                                                                                     \
                                                                                                                                 \
public:

#define VFUNC( type, name, idx, args, ... )                                                                                      \
	type name args { return mem::call_v_func< type, idx >( this, __VA_ARGS__ ); }

#define OFFSET_THIS( type ) *reinterpret_cast< type* >( reinterpret_cast< ptr >( this ) + offset )

#define PTR_OFFSET_THIS( type ) reinterpret_cast< type* >( reinterpret_cast< ptr >( this ) + offset )

#define OFFSET( type, name, value )                                                                                              \
	type& name( ) {                                                                                                              \
		static const auto offset = value;                                                                                        \
		return OFFSET_THIS( type );                                                                                              \
	}

#define PTR_OFFSET( type, name, value )                                                                                          \
	type* name( ) {                                                                                                              \
		static const auto offset = value;                                                                                        \
		return PTR_OFFSET_THIS( type );                                                                                          \
	}

#define NETVAR( type, name, netvar ) OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) )

#define NETVAR_PTR( type, name, netvar ) PTR_OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) )

#define NETVAR_OFFSET( type, name, netvar, offset ) OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) + offset )

#define NETVAR_OFFSET_PTR( type, name, netvar, offset ) PTR_OFFSET( type, name, g_netvars.get( HASH_CT( netvar ) ) + offset )

#define NETVAR_DATAMAP( type, name, netvar ) OFFSET( type, name, g_netvars.find_in_datamap( get_data_map( ), HASH_CT( netvar ) ) )

#define NETVAR_DATAMAP_PRED( type, name, netvar )                                                                                \
	OFFSET( type, name, g_netvars.find_in_datamap( get_pred_data_map( ), HASH_CT( netvar ) ) )

#endif // MACROS_HPP