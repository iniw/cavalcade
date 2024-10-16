#pragma once

#include <corecrt_math_defines.h>

// just to make accessing vector elements less annoying
enum vec_accessors
{
	X = 0,
	Y,
	Z,
	W,

	X2 = 2,
	Y2 = 3,

	WIDTH  = 2,
	HEIGHT = 3
};

#define VEC_OP( op )                                                                                                                                 \
	constexpr vector operator op( const T& other ) const {                                                                                           \
		vec_t buf;                                                                                                                                   \
		for ( u32 i = 0; i < N; i++ )                                                                                                                \
			buf[ i ] = m_data[ i ] op other;                                                                                                         \
		return buf;                                                                                                                                  \
	}                                                                                                                                                \
	constexpr vector operator op( const arr_t& other ) const {                                                                                       \
		vec_t buf;                                                                                                                                   \
		for ( u32 i = 0; i < N; i++ )                                                                                                                \
			buf[ i ] = m_data[ i ] op other[ i ];                                                                                                    \
		return buf;                                                                                                                                  \
	}                                                                                                                                                \
	constexpr vector operator op( const vec_t& other ) const {                                                                                       \
		vec_t buf;                                                                                                                                   \
		for ( u32 i = 0; i < N; i++ )                                                                                                                \
			buf[ i ] = m_data[ i ] op other[ i ];                                                                                                    \
		return buf;                                                                                                                                  \
	}

#define VEC_SELF_OP( op )                                                                                                                            \
	constexpr vector operator op( const T& other ) {                                                                                                 \
		for ( u32 i = 0; i < N; i++ )                                                                                                                \
			m_data[ i ] op other;                                                                                                                    \
		return *this;                                                                                                                                \
	}                                                                                                                                                \
	constexpr vector operator op( const arr_t& other ) {                                                                                             \
		for ( u32 i = 0; i < N; i++ )                                                                                                                \
			m_data[ i ] op other[ i ];                                                                                                               \
		return *this;                                                                                                                                \
	}                                                                                                                                                \
	constexpr vector operator op( const vec_t& other ) {                                                                                             \
		for ( u32 i = 0; i < N; i++ )                                                                                                                \
			m_data[ i ] op other[ i ];                                                                                                               \
		return *this;                                                                                                                                \
	}

namespace math {
	template< typename T >
	concept Number = std::is_arithmetic_v< T >;

	// credits to: https://www.unknowncheats.me/forum/c-and-c/157292-variadic-vector-class.html

	template< Number T, u32 N >
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
		template< typename... VA >
		constexpr vector( VA... args ) : m_data{ static_cast< T >( args )... } {
			static_assert( N >= 2, "vector requires a min. dimension of 2" );
			static_assert( sizeof...( VA ) == N, "invalid number of arguments" );
		}

		constexpr T& operator[]( u32 idx ) {
			return m_data[ idx ];
		}

		constexpr const T& operator[]( u32 idx ) const {
			return m_data[ idx ];
		}

		constexpr T& at( u32 idx ) {
			return m_data.at( idx );
		}

		constexpr const T& at( u32 idx ) const {
			return m_data.at( idx );
		}

		constexpr bool operator==( const vec_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				if ( m_data[ i ] != other[ i ] )
					return false;

			return true;
		}

		constexpr bool operator!=( const vec_t& other ) {
			for ( u32 i = 0; i < N; i++ )
				if ( m_data[ i ] == other[ i ] )
					return false;

			return true;
		}

		VEC_SELF_OP( = )

		VEC_OP( +);
		VEC_SELF_OP( += )

		VEC_OP( -);
		VEC_SELF_OP( -= )

		VEC_OP( * );
		VEC_SELF_OP( *= )

		VEC_OP( / );
		VEC_SELF_OP( /= )

		constexpr const arr_t& data( ) const {
			return m_data;
		}

		constexpr bool empty( ) const {
			for ( u32 i = 0; i < N; i++ )
				if ( m_data[ i ] != static_cast< T >( 0 ) )
					return false;

			return true;
		}

		constexpr T dist_to( const vec_t& other ) const {
			return ( other - ( *this ) ).length( );
		}

		constexpr f32 length_2d( ) const {
			return std::hypotf( m_data[ X ], m_data[ Y ] );
		}

		constexpr T length( ) const {
			return std::sqrt( length_sqr( ) );
		}

		constexpr T length_sqr( ) const {
			T buf = static_cast< T >( 0 );

			for ( const auto& i : m_data )
				buf += ( i * i );

			return buf;
		}

		constexpr void fill( const T& value ) {
			m_data.fill( value );
		}

		constexpr void normalize( ) requires( std::is_same_v< T, float > ) {
			if ( length( ) != 0.F )
				( *this ) /= length( );
			else
				fill( 0.f );
		}

		constexpr vec_t cross_product( const vec_t& other, const bool normalize_cross_product = false ) const {
			// static_assert( N == 3, "cross product requires a 3 dimensional vector" );

			vec_t vec = { m_data[ 1 ] * other[ 2 ] - m_data[ 2 ] * other[ 1 ], m_data[ 2 ] * other[ 0 ] - m_data[ 0 ] * other[ 2 ],
				          m_data[ 0 ] * other[ 1 ] - m_data[ 1 ] * other[ 0 ] };

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

		constexpr T dot_product( const T* other ) const {
			auto dot = static_cast< T >( 0 );

			for ( u32 i = 0; i < N; i++ )
				dot += m_data[ i ] * other[ i ];

			return dot;
		}

		void vector_angles( const vec_t& forward ) {
			if ( forward[ 1 ] == 0.f && forward[ 0 ] == 0.f ) {
				m_data[ 0 ] = ( forward[ 2 ] > 0.f ) ? 270.f : 90.f; // pitch
				m_data[ 1 ] = 0.f;                                   // yaw
			} else {
				m_data[ 0 ] = std::atan2( -forward[ 2 ], forward.length_2d( ) ) * ( -180.f / M_PI );
				m_data[ 1 ] = std::atan2( forward[ 1 ], forward[ 0 ] ) * ( 180.f / M_PI );

				if ( m_data[ 1 ] > 90.f )
					m_data[ 1 ] -= 180.f;
				else if ( m_data[ 1 ] < 90.f )
					m_data[ 1 ] += 180.f;
				else if ( m_data[ 1 ] == 90.f )
					m_data[ 1 ] = 0.f;
			}

			m_data[ 2 ] = 0.F;
		}

		inline vec_t calculate_angle( const vec_t& dst ) requires( N == 3 ) {
			vec_t a    = { 0.F, 0.F, 0.F };
			auto delta = ( ( *this ) - dst );

			delta.normalize( );
			a.vector_angles( delta );

			return a;
		}

		inline vec_t to_angle( ) {
			constexpr auto RAD2DEG = []( const float x ) { return x * ( 180.f / M_PI ); };
			return vec_t( RAD2DEG( std::atan2( -m_data[ Z ], length_2d( ) ) ), RAD2DEG( std::atan2( m_data[ Y ], m_data[ X ] ) ), 0.F );
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

		// NOTE(wini): these aren't animated.
		constexpr vec_t shrink( const T& amt ) const {
			// static_assert( N == 4, "shrink requires a 4 dimensional vector" );

			vec_t vec = *this;

			for ( u32 i = 0; i < 2; i++ )
				vec[ i ] += amt;

			for ( u32 i = 2; i < 4; i++ )
				vec[ i ] -= amt * 2;

			return vec;
		}

		constexpr vec_t expand( const T& amt ) const {
			// static_assert( N == 4, "expand requires a 4 dimensional vector" );

			vec_t vec = *this;

			for ( u32 i = 0; i < 2; i++ )
				vec[ i ] -= amt;

			for ( u32 i = 2; i < 4; i++ )
				vec[ i ] += amt * 2;

			return vec;
		}

		constexpr bool in_rect( const vector< T, 4 >& rect ) const {
			// static_assert( N == 2, "in_rect requires a 2 dimensional vector" );

			return m_data[ X ] >= rect[ X ] && m_data[ Y ] >= rect[ Y ] && m_data[ X ] <= rect[ X ] + rect[ X2 ] &&
			       m_data[ Y ] <= rect[ Y ] + rect[ Y2 ];
		}

		constexpr vector< T, 2 > pos( ) const {
			// static_assert( N == 4, "pos requires a 4 dimensional vector" );

			return vector< T, 2 >( m_data[ X ], m_data[ Y ] );
		}

		constexpr vec_t& pos( const vector< T, 2 >& other ) {
			// static_assert( N == 4, "pos requires a 4 dimensional vector" );

			m_data[ X ] = other[ X ];
			m_data[ Y ] = other[ Y ];

			return *this;
		}

		constexpr vec_t& size( const vector< T, 2 >& other ) {
			// static_assert( N == 4, "size requires a 4 dimensional vector" );

			m_data[ X2 ] = other[ X ];
			m_data[ Y2 ] = other[ Y ];

			return *this;
		}

		constexpr vector< T, 2 > size( ) const {
			// static_assert( N == 4, "size requires a 4 dimensional vector" );

			return vector< T, 2 >( m_data[ X2 ], m_data[ Y2 ] );
		}

		constexpr T width( ) const {
			// static_assert( N == 4, "width requires a 4 dimensional vector" );

			return m_data[ X2 ] - m_data[ X ];
		}

		constexpr T height( ) const {
			// static_assert( N == 4, "height requires a 4 dimensional vector" );

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

#undef VEC_OP
#undef VEC_SELF_OP
#undef VEC_CMP_OP

template< math::Number T, u32 N >
struct fmt::formatter< math::vector< T, N > > : fmt::formatter< std::string > {
	auto format( const math::vector< T, N >& p, format_context& ctx ) {
		std::string buf = XOR( "[ " );

		for ( u32 i = 0; i < N; i++ ) {
			buf.append( XOR( "{}" ) );

			if ( i != N - 1 )
				buf.append( XOR( " - " ) );

			buf = fmt::format( buf, p[ i ] );
		}

		buf.append( XOR( " ]" ) );

		return formatter< std::string >::format( buf, ctx );
	}
};