#ifndef BIT_VECTOR_HPP
#define BIT_VECTOR_HPP

#include "buffer.hpp"

namespace mem {
	struct bit_vector {
		buffer< u32 > m_storage;

		static bool bit_array_test_bit( const u32* arr, i32 n ) {
			u32 mask = ( u32 )1 << ( n & 31 );
			return ( arr[ n >> 5 ] & mask ) != 0;
		}

		static void bit_array_set_bit( u32* arr, i32 n ) {
			u32 mask = ( u32 )1 << ( n & 31 );
			arr[ n >> 5 ] |= mask;
		}

		static void bit_array_clear_bit( u32* arr, i32 n ) {
			u32 mask = ( u32 )1 << ( n & 31 );
			arr[ n >> 5 ] &= ~mask;
		}

		void create( i32 size ) {
			m_storage.resize( ( size + 31 ) >> 5 );
			memset( m_storage.m_buffer, 0, ( u32 )m_storage.m_size * sizeof( m_storage.m_buffer[ 0 ] ) );
		}

		void clear( ) { m_storage.clear( ); }

		bool test_bit( i32 n ) const { return bit_array_test_bit( m_storage.m_buffer, n ); }
		void set_bit( i32 n ) { bit_array_set_bit( m_storage.m_buffer, n ); }

		void clear_bit( i32 n ) { bit_array_clear_bit( m_storage.m_buffer, n ); }
	};
} // namespace mem

#endif // BIT_VECTOR_HPP
