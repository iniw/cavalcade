#ifndef UTL_VECTOR_HPP
#define UTL_VECTOR_HPP

#include "utl_mem.hpp"

namespace sdk {
	template< typename T, class A = utl_mem< T > >
	struct utl_vector {
	private:

		A m_mem;
		u32 m_size;
		T* m_elements;

	public:

		T& operator[]( u32 idx ) { return m_mem[ idx ]; }

		const T& operator[]( u32 idx ) const { return m_mem[ idx ]; }

		const T* begin( ) const { return m_mem.base( ); }

		const T* end( ) const { return m_mem.base( ) + m_size; }

		u32 count( ) const { return m_size; }

		void set_size( u32 val ) { m_size = val; }

		void clear( ) {
			for ( u32 i = m_size; --i >= 0; )
				m_mem[ i ].~T( );

			m_size = 0;
		}

		void purge( ) {
			clear( );
			m_mem.purge( );
		}
	};
} // namespace sdk

#endif // UTL_VECTOR_HPP
