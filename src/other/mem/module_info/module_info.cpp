#include "../mem.hpp"

mem::module_info::module_info( address dll_base ) {
	m_module_base = dll_base;

	m_bitmap = m_module_base.as< u8* >( );

	m_dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( m_module_base.as< ptr >( ) );
	m_nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( m_module_base + m_dos_header->e_lfanew );

	m_size = m_nt_headers->OptionalHeader.SizeOfImage;

	PIMAGE_SECTION_HEADER section_list = IMAGE_FIRST_SECTION( this->m_nt_headers );
	for ( u16 i = 0; i < m_nt_headers->FileHeader.NumberOfSections; i++ ) {
		auto name = reinterpret_cast< cstr >( section_list->Name );

		m_sections[ HASH_RT( name ) ] = { section_list->PointerToRawData, section_list->SizeOfRawData };

		section_list++;
	}

	// for when we want to scan the entire module
	m_sections[ NULL ] = { 0x0, m_size };
}

i32 mem::module_info::rva_to_offset( u32 rva, PIMAGE_NT_HEADERS nt_headers, bool in_memory ) {
	if ( !rva || !in_memory )
		return static_cast< i32 >( rva );

	auto sec = IMAGE_FIRST_SECTION( nt_headers );

	for ( u32 i = 0; i < nt_headers->FileHeader.NumberOfSections; i++ ) {
		if ( rva >= sec->VirtualAddress && rva < sec->VirtualAddress + sec->Misc.VirtualSize )
			break;

		sec++;
	}

	return static_cast< i32 >( rva - sec->VirtualAddress + sec->PointerToRawData );
}

mem::address mem::module_info::search_byte_array( const i32* bytes, u32 size, const section& section ) {
	address result;

	for ( u32 i = section.m_start; i < section.m_start + ( section.m_size - size ); i++ ) {
		bool found = true;

		for ( u32 j = 0; j < size; j++ ) {
			found = m_bitmap[ i + j ] == bytes[ j ] || bytes[ j ] == -1;
			if ( !found )
				break;
		}

		if ( !found )
			continue;

		result = &m_bitmap[ i ];
		break;
	}

	return result;
}

void mem::module_info::add_address( u32 name_hash, address address ) {
	ENFORCE( address, "bad address, name: {}", name_hash );
	m_addresses[ name_hash ] = address;
}

bool mem::module_info::validate( ) {
	if ( m_addresses.empty( ) )
		return true;

	for ( auto& [ k, v ] : m_addresses )
		if ( !v )
			return false;

	return true;
}

bool mem::module_info::hook( u32 name_hash, unk custom_fn ) {
	ENFORCE( m_addresses.contains( name_hash ), "invalid hook name: {}", name_hash );
	return m_addresses.at( name_hash ).hook( custom_fn );
}

void mem::module_info::restore( ) {
	if ( m_addresses.empty( ) )
		return;

	for ( auto& [ k, v ] : m_addresses )
		v.unhook( );
}