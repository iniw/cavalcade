#pragma once

#include "../types/detour.hpp"
#include "../types/section.hpp"

namespace mem {
	struct module_info {
	private:

		u32 m_size;

		u8* m_bitmap;
		PIMAGE_DOS_HEADER m_dos_header;
		PIMAGE_NT_HEADERS m_nt_headers;

		std::unordered_map< u32, section > m_sections;
		std::unordered_map< u32, detour > m_addresses;

	public:

		address m_module_base;

	public:

		module_info( ) = default;

		module_info( address dll_base );

		operator address( ) {
			return m_module_base;
		}

		template< u32 size >
		address find_pattern( const std::array< i32, size >& pattern, u32 section_hash = HASH_CT( ".text" ) );

		template< u32 size >
		void add_pattern( u32 name_hash, const std::array< i32, size >& pattern, bool relative = false );

		void add_address( u32 name_hash, address address );

		bool validate( );

		bool hook( u32 name_hash, unk custom_fn );

		void restore( );

		template< typename T = address >
		T export_fn( u32 name_hash, bool in_memory = false );

		template< typename T = address >
		T get_address( u32 name_hash );

		template< typename T = address >
		T get_og( u32 name_hash );

	private:

		i32 rva_to_offset( u32 rva, PIMAGE_NT_HEADERS nt_headers, bool in_memory = false );

		address search_byte_array( const i32* bytes, u32 size, const section& section );
	};
} // namespace mem

#include "module_info.inl"