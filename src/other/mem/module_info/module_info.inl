#ifndef MODULE_INFO_INL
#define MODULE_INFO_INL

template< u32 size >
mem::address mem::module_info::find_pattern( const std::array< i32, size >& pattern, u32 section_hash ) {
	auto& section = m_sections[ section_hash ];

	auto result = search_byte_array( pattern.data( ), pattern.size( ), section );
	if ( !result )
		return nullptr;

	return result;
}

template< u32 size >
void mem::module_info::add_pattern( u32 name_hash, const std::array< i32, size >& pattern, bool relative ) {
	auto address = find_pattern( pattern );

	ENFORCE( address, "bad pattern, pattern: {} | name: {}", pattern, name_hash );

	if ( relative )
		address = address.relative( );

	ENFORCE( address, "bad relative address, pattern: {} | name: {}", pattern, name_hash );

	m_addresses[ name_hash ] = address;
}

template< typename T >
T mem::module_info::export_fn( u32 name_hash, bool in_memory ) {
	auto exports = reinterpret_cast< IMAGE_EXPORT_DIRECTORY* >( rva_to_offset(
		m_module_base.add( m_nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress ),
		m_nt_headers, in_memory ) );

	auto names = m_module_base.add< u32* >( rva_to_offset( exports->AddressOfNames, m_nt_headers, in_memory ) );

	auto ordinal_index = static_cast< u32 >( -1 );

	for ( u32 i = 0; i < exports->NumberOfFunctions; i++ ) {
		auto function_name = m_module_base.add< cstr >( rva_to_offset( names[ i ], m_nt_headers, in_memory ) );

		if ( HASH_RT( function_name ) == name_hash ) {
			ordinal_index = i;
			break;
		}
	}

	if ( ordinal_index > exports->NumberOfFunctions )
		return T( );

	auto ordinals  = m_module_base.add< u16* >( rva_to_offset( exports->AddressOfNameOrdinals, m_nt_headers, in_memory ) );
	auto addresses = m_module_base.add< u32* >( rva_to_offset( exports->AddressOfFunctions, m_nt_headers, in_memory ) );

	return m_module_base.add< T >( rva_to_offset( addresses[ ordinals[ ordinal_index ] ], m_nt_headers, in_memory ) );
}

template< typename T >
T mem::module_info::get_og( u32 name_hash ) {
	ENFORCE( m_addresses.contains( name_hash ), "invalid og name: {}", name_hash );
	return ( T )m_addresses.at( name_hash ).m_og;
}

template< typename T >
T mem::module_info::get_address( u32 name_hash ) {
	ENFORCE( m_addresses.contains( name_hash ), "invalid address name: {}", name_hash );
	return m_addresses.at( name_hash ).as< T >( );
}

#endif // MODULE_INFO_INL