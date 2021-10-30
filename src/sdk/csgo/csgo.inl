#ifndef CSGO_INL
#define CSGO_INL

template< typename T >
T sdk::csgo::find_interface( mem::module_info& the_module, u32 name_hash, bool remove_numbers ) {
	const auto create_interface = the_module->export_fn( HASH_CT( "CreateInterface" ) );
	if ( !create_interface )
		return T( );

	const auto create_interface_fn = create_interface.add( 0x4 ).relative( );
	if ( !create_interface_fn )
		return T( );

	auto interface_node = create_interface_fn.add( 0x6 ).get< interfaces::registry* >( 2 );

	while ( interface_node != nullptr ) {
		std::string name = interface_node->m_name;

		if ( remove_numbers )
			name.erase( std::remove_if( name.begin( ), name.end( ), []( const char& ch ) { return std::isdigit( ch ); } ),
			            name.end( ) );

		auto hash = HASH_RT( name );

		if ( hash == name_hash ) {
			const auto interface_address = interface_node->m_create_fn( );
			if ( !interface_address )
				return T( );

			return ( T )( interface_address );
		}

		interface_node = interface_node->m_next;
	}

	ENFORCE( true, "invalid interface name: {}", name_hash );

	return nullptr;
}

#endif // CSGO_INL