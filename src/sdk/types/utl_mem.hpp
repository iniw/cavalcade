#ifndef UTL_MEM_HPP
#define UTL_MEM_HPP

namespace sdk {
	template< typename T, typename N = i32 >
	struct utl_mem {
		T* m_mem;
		i32 m_alloc_count;
		i32 m_grow_size;

		T& operator[]( N idx ) { return m_mem[ idx ]; }

		const T& operator[]( N idx ) const { return m_mem[ idx ]; }

		const T* base( ) const { return m_mem; }

		void purge( ) {
			std::free( m_mem );
			m_mem = nullptr;
		}
	};
} // namespace sdk

#endif // UTL_MEM_HPP
