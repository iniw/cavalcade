#pragma once

namespace utils::cx {
	template< u32 N = 0 >
	struct string {
		char m_data[ N + 1 ]{ };

		constexpr string( ) : m_data{ } { }

		constexpr string( const char ( &str )[ N ] ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] = str[ i ];
		}

		constexpr string( const string< N >& str ) {
			for ( u32 i = 0; i < N; i++ )
				m_data[ i ] = str.m_data[ i ];
		}

		constexpr operator cstr( ) {
			return &m_data[ 0 ];
		}

		constexpr operator std::string_view( ) {
			return std::string_view( data( ), N );
		}

		constexpr operator fmt::string_view( ) {
			return fmt::string_view( data( ), N );
		}

		constexpr cstr data( ) const {
			return &m_data[ 0 ];
		}
	};
} // namespace utils::cx

template< u32 N >
struct fmt::formatter< utils::cx::string< N > > : fmt::formatter< std::string > {
	auto format( const utils::cx::string< N >& p, format_context& ctx ) {
		return formatter< std::string >::format( p.data( ), ctx );
	}
};