#ifndef BUFFER_HPP
#define BUFFER_HPP

namespace mem {
	template< typename T >
	struct buffer {
		u32 m_size;
		u32 m_capacity;
		T* m_buffer;

	private:

		u32 m_initial_capacity;

	public:

		constexpr buffer( ) : m_size{ 0u }, m_capacity{ 0u }, m_buffer{ nullptr }, m_initial_capacity{ 0u } { }

		~buffer( ) { clear( ); }

		// no error handling, u better not pass an invalid index

		T& operator[]( const u32 idx ) { return m_buffer[ idx ]; }

		const T& operator[]( const u32 idx ) const { return m_buffer[ idx ]; }

		bool init( const u32 size ) {
			m_initial_capacity = size * sizeof( T );
			m_capacity         = m_initial_capacity;

			m_buffer = static_cast< T* >( std::malloc( m_capacity ) );

			// g_io.log( "initialized {} with {}b ( {}mb )", utils::cx::ct_data<
			// utils::cx::type_name< T >( ) >::value.data( ), m_capacity, m_capacity / 1024 );

			return m_buffer;
		}

		void clear( ) {
			if ( m_buffer && m_capacity )
				std::free( m_buffer );

			m_capacity = 0;
			m_size     = 0;
			m_buffer   = nullptr;
		}

		void destroy_all( ) {
			if ( !m_capacity )
				return;

			for ( u32 i = 0; i < m_size; i++ )
				m_buffer[ i ].~T( );

			clear( );
		}

		void delete_all( ) {
			if ( !m_capacity )
				return;

			for ( u32 i = 0; i < m_size; i++ )
				delete m_buffer[ i ];

			clear( );
		}

		void push_back( const T& val ) {
			if ( reserve( 1 ) ) {
				std::memcpy( &m_buffer[ m_size ], &val, sizeof( val ) );
				m_size++;
			}
		}

		bool grow( const u32 new_capacity ) {
			unk mem = std::malloc( new_capacity );
			if ( !mem )
				return false;

			if ( m_buffer && m_capacity ) {
				std::memcpy( mem, m_buffer, m_capacity );
				std::free( m_buffer );
			}

			m_capacity = new_capacity;

			m_buffer = static_cast< T* >( mem );

			// g_io.log( "grew {} by {}b ( {}mb ), total capacity is now {}b ( {}mb )",
			// utils::cx::ct_data< utils::cx::type_name< T >( ) >::value.data( ), new_capacity,
			// new_capacity / 1024, m_capacity, m_capacity / 1024 );

			return m_buffer;
		}

		bool reserve( const u32 amt, bool add = true ) {
			u32 desired_capacity = 0;

			add ? desired_capacity = ( m_size + amt ) * sizeof( T ) : desired_capacity = amt * sizeof( T );

			if ( desired_capacity > m_capacity )
				return grow( desired_capacity + m_initial_capacity );

			return true;
		}

		bool resize( const u32 new_size ) {
			u32 capacity     = m_capacity ? m_capacity + m_capacity / 2 : 8 * sizeof( T );
			u32 new_capacity = new_size * sizeof( T );

			if ( new_capacity > m_capacity )
				grow( new_capacity > capacity ? new_capacity : capacity );

			m_size = new_size;

			return true;
		}

		bool resize( const u32 new_size, const T& val ) {
			u32 capacity     = m_capacity ? m_capacity + m_capacity / 2 : 8 * sizeof( T );
			u32 new_capacity = new_size * sizeof( T );

			if ( new_capacity > m_capacity )
				grow( new_capacity > capacity ? new_capacity : capacity );

			if ( new_size > m_size )
				for ( u32 i = m_size; i < new_size; i++ )
					std::memcpy( &m_buffer[ i ], &val, sizeof( val ) );

			m_size = new_size;

			return true;
		}

		u32 size( ) const { return m_size; }

		u32 size_in_bytes( ) const { return m_size * sizeof( T ); }

		bool empty( ) const { return !m_size; }

		T* begin( ) { return m_buffer; }

		const T* begin( ) const { return m_buffer; }

		T* end( ) { return m_buffer + m_size; }

		const T* end( ) const { return m_buffer + m_size; }

		T& front( ) { return m_buffer[ 0 ]; }

		const T& front( ) const { return m_buffer[ 0 ]; }

		T& back( ) { return m_buffer[ m_size - 1 ]; }

		const T& back( ) const { return m_buffer[ m_size - 1 ]; }
	};
} // namespace mem

#endif // BUFFER_HPP