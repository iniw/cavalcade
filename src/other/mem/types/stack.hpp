#pragma once

namespace mem {
	struct stack {
		ptr m_address;

		stack( unk address_of_return_address ) : m_address{ reinterpret_cast< ptr >( address_of_return_address ) - sizeof( i32 ) } { }

		stack& next( ) const {
			return *reinterpret_cast< stack* >( m_address );
		}

		template< typename T = address >
		T local( ptrdiff offset ) const {
			return static_cast< T >( m_address - offset );
		}

		template< typename T = address >
		T arg( ptrdiff offset ) const {
			return static_cast< T >( m_address + offset );
		}

		address return_address( ) const {
			if ( !m_address )
				return nullptr;

			return *reinterpret_cast< ptr* >( m_address + sizeof( i32 ) );
		}

		address address_of_return_address( ) const {
			if ( !m_address )
				return nullptr;

			return m_address + sizeof( u32 );
		}
	};
} // namespace mem