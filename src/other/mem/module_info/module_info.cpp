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

static auto generate_bad_char_table( const i32* bytes, u32 last_idx ) {
	std::array< u32, 256 > table = { };

	MOCKING_TRY;

	MOCK bytes;
	MOCK last_idx;

	MOCKING_CATCH( return table );

	// purely functional (no side effect)
	constexpr auto rfind = []( const i32* bytes, u32 last_idx ) -> u32 {
		u32 indice = last_idx;

		for ( auto entry = bytes + indice; entry && *entry != -1 && indice > 0; --entry ) {
			--indice;
		}

		return indice;
	};

	const auto last_wildcard = rfind( bytes, last_idx );

	const auto default_shift = std::max( u32{ 1 }, last_idx - last_wildcard );
	table.fill( default_shift );

	for ( auto i = last_wildcard; i < last_idx; ++i ) {
		table[ bytes[ i ] == -1 ? '\?' : bytes[ i ] ] = last_idx - i;
	}

	return table;
}

mem::address mem::module_info::search_byte_array( const i32* bytes, u32 size, const section& section ) {
	address result;

	const auto last_idx       = size - 1;
	const auto bad_char_table = generate_bad_char_table( bytes, last_idx );
	auto start                = m_bitmap + section.m_start;
	const auto end            = start + section.m_size - size;

	while ( start <= end ) {
		i32 it = last_idx;
		while ( it >= 0 && ( bytes[ it ] == -1 || bytes[ it ] == start[ it ] ) ) {
			--it;
		}

		if ( it < 0 ) {
			result = start;
			break;
		}

		start += bad_char_table[ start[ size - 1 ] ];
	}

	return result;
}

void mem::module_info::add_address( u32 name_hash, address address ) {
	MOCKING_TRY;

	MOCK ENFORCE( address, "bad address, name: {}", name_hash );

	MOCKING_CATCH( return );

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
	MOCKING_TRY;

	MOCK ENFORCE( m_addresses.contains( name_hash ), "invalid hook, name: {}", name_hash );

	MOCKING_CATCH( return false );

	return m_addresses.at( name_hash ).hook( custom_fn );
}

void mem::module_info::restore( ) {
	if ( m_addresses.empty( ) )
		return;

	for ( auto& [ k, v ] : m_addresses )
		v.unhook( );
}