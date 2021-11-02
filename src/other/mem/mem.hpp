#ifndef MEM_HPP
#define MEM_HPP

#include "module_info/module_info.hpp"
#include "types/address.hpp"
#include "types/buffer.hpp"
#include "types/detour.hpp"
#include "types/section.hpp"
#include "types/stack.hpp"

#include "win32/win32.hpp"

namespace mem {
	struct mem {
	private:

		std::unordered_map< u32, module_info > m_modules;

	public:

		module_info& operator[]( u32 name_hash ) {
			return m_modules.at( name_hash );
		}

		bool init( );

		bool setup( );

		void unload( );

	private:

		void add_addresses( );

		bool validate_addresses( );
	};

	template< typename T = address >
	inline T get_v_func( address thisptr, u32 idx );

	template< typename T, u32 I, typename... VA >
	inline T call_v_func( address thisptr, VA... args );
} // namespace mem

inline mem::mem g_mem;

#include "mem.inl"

#endif // MEM_HPP