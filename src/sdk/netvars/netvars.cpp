#include "netvars.hpp"

#include "../csgo/csgo.hpp"

bool sdk::netvars::init( ) {
	auto list = g_csgo.m_client->get_all_classes( );
	if ( !list )
		return false;

#ifdef NETVARS_DUMP_FILE

	m_file.open( g_io.directory( io::dirs::NETVARS ).append( "netvars.bm" ), std::ios::trunc );
	if ( !m_file.good( ) )
		return false;

	m_file << io::format( "[ cavalcade - {:%H:%M:%S} ] netvars dump\n", fmt::localtime( std::time( nullptr ) ) );

#endif

	while ( list ) {
		store( list->m_recv_table->m_net_table_name, list->m_recv_table );

		list = list->m_next;
	}

#ifdef NETVARS_DUMP_FILE

	m_file.close( );

#endif

	g_io.log( XOR( "initialized netvars" ) );

	return !m_offsets.empty( );
}

ptr sdk::netvars::get( u32 netvar_hash ) {
	return m_offsets.at( netvar_hash );
}

ptr sdk::netvars::find_in_datamap( const data_map* map, u32 netvar_hash ) {
	while ( map ) {
		for ( auto i = 0; i < map->m_data_fields; ++i ) {
			auto field_name = map->m_data_desc[ i ].m_field_name;
			if ( !field_name )
				continue;

			if ( HASH_RT( field_name ) == netvar_hash )
				return map->m_data_desc[ i ].m_field_offset[ TD_OFFSET_NORMAL ];

			if ( map->m_data_desc[ i ].m_field_type == field_type::EMBEDDED )
				if ( map->m_data_desc[ i ].m_type_description )
					if ( auto offset = find_in_datamap( map->m_data_desc[ i ].m_type_description, netvar_hash ) )
						return offset;
		}

		map = map->m_base_map;
	}

	return 0;
}

void sdk::netvars::store( std::string_view table_name, const recv_table* table, ptrdiff offset, i32 depth ) {
#ifdef NETVARS_DUMP_FILE

	std::string spacing( depth, '\t' );

	m_file << spacing << table->m_net_table_name << ":\n";

#endif

	for ( auto i = 0; i < table->m_props; ++i ) {
		auto prop = &table->m_props_ptr[ i ];
		if ( !prop || std::isdigit( prop->m_var_name[ 0 ] ) )
			continue;

		if ( HASH_RT( prop->m_var_name ) == HASH_CT( "baseclass" ) )
			continue;

		auto child = prop->m_data_table;

		if ( child && child->m_props && child->m_net_table_name[ 0 ] == 'D' && prop->m_recv_type == prop_type::DATATABLE )
			store( table_name, child, prop->m_offset + offset, depth + 1 );

		auto hash = HASH_RT( io::format( XOR( "{}->{}" ), table_name, prop->m_var_name ) );

		if ( !m_offsets[ hash ] ) {
			m_offsets[ hash ] = prop->m_offset + offset;

#ifdef NETVARS_DUMP_FILE

			m_file << io::format( "{}\t{} {} = 0x{:X}; \n", spacing, get_type( prop ), prop->m_var_name,
			                      prop->m_offset + offset );

#endif
		}
	}
}

std::string sdk::netvars::get_type( const recv_prop* prop ) {
	static auto std_recv_proxies = g_csgo.m_client->get_std_recv_proxies( );

	switch ( prop->m_recv_type ) {
	case prop_type::INT:

		if ( prop->m_proxy_fn == std_recv_proxies->int32_to_int8 )
			return XOR( "i8" );
		else if ( prop->m_proxy_fn == std_recv_proxies->int32_to_int16 )
			return XOR( "i16" );

		return XOR( "i32" );

	case prop_type::FLOAT:
		return XOR( "f32" );

	case prop_type::VECTOR:
		return XOR( "v3f" );

	case prop_type::VECTOR2D:
		return XOR( "v2f" );

	case prop_type::STRING:
		return io::format( XOR( "char[{}]" ), prop->m_string_buffer_size );

	case prop_type::ARRAY:
		return io::format( XOR( "array[{}]" ), prop->m_elements );

	case prop_type::DATATABLE:
		return XOR( "unk" );

	case prop_type::INT64:
		return XOR( "i64" );

	default:
		return "";
	}
}