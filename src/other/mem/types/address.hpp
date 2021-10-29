#ifndef ADDRESS_HPP
#define ADDRESS_HPP

namespace mem {
	struct address {
	protected:

		ptr m_address;

	public:

		constexpr address( ) : m_address{ 0 } { }

		address( ptr address ) : m_address{ address } { }

		address( unk address ) : m_address{ reinterpret_cast< ptr >( address ) } { }

		auto operator<=>( const address& ) const = default;

		operator ptr( ) const { return m_address; }

		operator unk( ) const { return reinterpret_cast< unk >( m_address ); }

		template< typename T = address >
		T to( ) const {
			return *( T* )( m_address );
		}

		template< typename T = address >
		T as( ) const {
			return m_address ? ( T )( m_address ) : T( );
		}

		template< typename T = address >
		T at( ptrdiff offset ) const {
			return m_address ? *( T* )( m_address + offset ) : T( );
		}

		template< typename T = address >
		T add( ptrdiff offset ) const {
			return m_address ? ( T )( m_address + offset ) : T( );
		}

		template< typename T = address >
		T sub( ptrdiff offset ) const {
			return m_address ? ( T )( m_address - offset ) : T( );
		}

		template< typename T = address >
		T get( u8 dereferences = 1 ) const {
			if ( !m_address )
				return T( );

			auto addr = m_address;

			while ( dereferences-- )
				if ( addr )
					addr = *reinterpret_cast< ptr* >( addr );

			return ( T )( addr );
		}

		template< typename T = ptr >
		void set( const T& value ) {
			if ( !m_address )
				return;

			*( T* )( m_address ) = value;
		}

		template< typename T = address >
		T relative( ptrdiff offset = 0x1 ) const {
			if ( !m_address )
				return T( );

			const ptr new_address = m_address + offset;

			const auto relative_offset = *reinterpret_cast< i32* >( new_address );
			if ( !relative_offset )
				return T( );

			return ( T )( new_address + sizeof( i32 ) + relative_offset );
		}
	};
} // namespace mem

#endif // ADDRESS_HPP
