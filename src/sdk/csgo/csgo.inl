#pragma once

template< typename T >
T sdk::csgo::find_interface( mem::module_info& the_module, u32 name_hash, bool remove_numbers ) {
	auto create_interface = the_module.export_fn( HASH_CT( "CreateInterface" ) );
	if ( !create_interface )
		return T( );

	auto create_interface_fn = create_interface.add( 0x4 ).relative( );
	if ( !create_interface_fn )
		return T( );

	auto interface_node = create_interface_fn.add( 0x6 ).get< interfaces::registry* >( 2 );

	while ( interface_node != nullptr ) {
		std::string name = interface_node->m_name;

		if ( remove_numbers )
			name.erase( std::remove_if( name.begin( ), name.end( ), []( char ch ) { return std::isdigit( ch ); } ), name.end( ) );

		if ( HASH_RT( name ) == name_hash ) {
			auto interface_address = interface_node->m_create_fn( );
			if ( !interface_address )
				return T( );

			return ( T )( interface_address );
		}

		interface_node = interface_node->m_next;
	}

	ENFORCE( true, "invalid interface name: {}", name_hash );

	return nullptr;
}