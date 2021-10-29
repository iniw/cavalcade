#ifndef VECTOR_HPP
#define VECTOR_HPP

// just to make accessing vector elements less annoying
enum
{
	X = 0,
	Y,
	Z,

	X2 = 2,
	Y2 = 3,

	WIDTH  = 2,
	HEIGHT = 3
};

namespace math {
	template< typename T > concept number = std::is_arithmetic_v< T >;

	// credits to: https://www.unknowncheats.me/forum/c-and-c/157292-variadic-vector-class.html

	template< number T, u32 N >
	struct vector {
	private:

		using arr_t = std::array< T, N >;
		using vec_t = vector< T, N >;

		arr_t m_data;

	public:

		constexpr vector( ) {
			static_assert( N >= 2, "vector requires a min. dimension of 2" );

			fill( static_cast< T >( 0 ) );
		}

		constexpr explicit vector( const arr_t& arr ) : m_data{ arr } {
			static_assert( N >= 2, "vector requires a min. dimension of 2" );
		}

		constexpr vector( const vec_t& other ) : m_data{ other.data( ) } {
			static_assert( N >= 2, "vector requires a min. dimension of 2" );
		}

		// this is extremely unsafe, oh well
		constexpr vector( const T* ptr ) {
			static_assert( N >= 2, "vector requires a min. dimension of 2" );

			std::copy( ptr, ptr + N, m_data.begin( ) );
		}

		// thanks para
		template< number... VA >
		constexpr vector( VA... args ) : m_data{ static_cast< T >( std::forward< VA >( args ) )... } {
			static_assert( N >= 2, "vector requires a min. dimension of 2" );
		}

		constexpr auto operator<=>( const vec_t& ) const = default;

		constexpr T& operator[]( u32 idx ) { return m_data[ idx ]; }

		constexpr const T& operator[]( u32 idx ) const { return m_data[ idx ]; }

		constexpr T& at( u32 idx ) { return m_data.at( idx ); }

		constexpr const T& at( u32 idx ) const { return m_data.at( idx ); }

		constexpr vector& operator=( const vec_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] = other[ i ];

			return *this;
		}

		constexpr vector& operator=( const arr_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] = other[ i ];

			return *this;
		}

		constexpr vector& operator=( const T& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] = other;

			return *this;
		}

		constexpr vector operator+( const vec_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] + other[ i ];

			return buf;
		}

		constexpr vector operator+( const arr_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] + other[ i ];

			return buf;
		}

		constexpr vector operator+( const T& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] + other;

			return buf;
		}

		constexpr vector& operator+=( const vec_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] += other[ i ];

			return *this;
		}

		constexpr vector& operator+=( const arr_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] += other[ i ];

			return *this;
		}

		constexpr vector& operator+=( const T& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] += other;

			return *this;
		}

		constexpr vector operator-( const vec_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] - other[ i ];

			return buf;
		}

		constexpr vector operator-( const arr_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] - other[ i ];

			return buf;
		}

		constexpr vector operator-( const T& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] - other;

			return buf;
		}

		constexpr vector& operator-=( const vec_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] -= other[ i ];

			return *this;
		}

		constexpr vector& operator-=( const arr_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] -= other[ i ];

			return *this;
		}

		constexpr vector& operator-=( const T& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] -= other;

			return *this;
		}

		constexpr vector operator*( const vec_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] * other[ i ];

			return buf;
		}

		constexpr vector operator*( const arr_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] * other[ i ];

			return buf;
		}

		constexpr vector operator*( const T& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] * other;

			return buf;
		}

		constexpr vector& operator*=( const vec_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] *= other[ i ];
			;

			return *this;
		}

		constexpr vector& operator*=( const arr_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] *= other[ i ];

			return *this;
		}

		constexpr vector& operator*=( const T& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] *= other;

			return *this;
		}

		constexpr vector operator/( const vec_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] / other[ i ];

			return buf;
		}

		constexpr vector operator/( const arr_t& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] / other[ i ];

			return buf;
		}

		constexpr vector operator/( const T& other ) const {
			vec_t buf;

			for ( u32 i = 0; i < N; i++ )
				buf[ i ] = m_data[ i ] / other;

			return buf;
		}

		constexpr vector& operator/=( const vec_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] /= other[ i ];

			return *this;
		}

		constexpr vector& operator/=( const arr_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] /= other[ i ];

			return *this;
		}

		constexpr vector& operator/=( const T& other ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] /= other;

			return *this;
		}

		constexpr const arr_t& data( ) const { return m_data; }

		constexpr bool empty( ) const {
			for ( u32 i = 0; i < N; i++ )
				if ( m_data[ i ] != static_cast< T >( 0 ) )
					return false;

			return true;
		}

		constexpr T dist_to( const vec_t& other ) const { return ( other - ( *this ) ).length( ); }

		constexpr f32 length_2d( ) const { return std::hypotf( m_data[ X ], m_data[ Y ] ); }

		constexpr T length( ) const { return std::sqrt( length_sqr( ) ); }

		constexpr T length_sqr( ) const {
			T buf = static_cast< T >( 0 );

			for ( const auto& i : m_data )
				buf += ( i * i );

			return buf;
		}

		constexpr void fill( const T& value ) { m_data.fill( value ); }

		constexpr void normalize( ) { ( *this ) /= length( ); }

		constexpr vec_t cross_product( const vec_t& other, const bool normalize_cross_product = false ) const {
			static_assert( N == 3, "cross product requires a 3 dimensional vector" );

			vec_t vec = { m_data[ 1 ] * other[ 2 ] - m_data[ 2 ] * other[ 1 ],
				          m_data[ 2 ] * other[ 0 ] - m_data[ 0 ] * other[ 2 ], m_data[ 0 ] * other[ 1 ] - m_data[ 1 ] * other[ 0 ]

			};

			if ( normalize_cross_product )
				vec.normalize( );

			return vec;
		}

		constexpr vec_t inversed( ) const {
			constexpr auto one = static_cast< T >( 1 );

			vec_t vec = *this;

			for ( auto& i : vec )
				i = one / i;

			return vec;
		}

		constexpr vec_t normalized( ) const {
			vec_t vec = *this;
			vec.normalize( );

			return vec;
		}

		constexpr T dot_product( const vec_t& other ) const {
			auto dot = static_cast< T >( 0 );

			for ( u32 i = 0; i < N; i++ )
				dot += m_data[ i ] * other[ i ];

			return dot;
		}

		constexpr vec_t& floor( ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] = std::floor( m_data[ i ] );

			return *this;
		}

		constexpr vec_t& ceil( ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] = std::ceil( m_data[ i ] );

			return *this;
		}

		constexpr T width( ) const {
			static_assert( N == 4, "width requires a 4 dimensional vector" );

			return m_data[ X2 ] - m_data[ X ];
		}

		constexpr T height( ) const {
			static_assert( N == 4, "height requires a 4 dimensional vector" );

			return m_data[ Y2 ] - m_data[ Y ];
		}
	};

	using v2f = vector< f32, 2 >;
	using v3f = vector< f32, 3 >;
	using v4f = vector< f32, 4 >;

	using v2i = vector< i32, 2 >;
	using v3i = vector< i32, 3 >;
	using v4i = vector< i32, 4 >;

	using v2s = vector< i8, 2 >;
	using v3s = vector< i8, 3 >;
	using v4s = vector< i8, 4 >;
} // namespace math

template< math::number T, u32 N >
struct fmt::formatter< math::vector< T, N > > : fmt::formatter< std::string > {
	auto format( const math::vector< T, N >& p, format_context& ctx ) {
		std::string buf = "[ ";

		for ( u32 i = 0; i < N; i++ ) {
			buf.append( "{}" );

			if ( i != N - 1 )
				buf.append( " - " );

			buf = fmt::format( buf, p[ i ] );
		}

		buf.append( " ]" );

		return formatter< std::string >::format( buf, ctx );
	}
};

#endif // VECTOR_HPP
