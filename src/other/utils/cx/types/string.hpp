#ifndef STRING_HPP
#define STRING_HPP

namespace utils::cx {
	template< u32 N = 0 >
	struct string {
	private:

		std::array< char, N + 1 > m_data;

	public:

		constexpr string( ) : m_data{ 0 } { }

		constexpr string( std::string_view str ) {
			for ( u32 i = 0; i < str.length( ); i++ )
				m_data[ i ] = str[ i ];

			m_data[ str.length( ) + 1 ] = '\0';
		}

		constexpr operator cstr( ) {
			return &m_data.begin( );
		}

		constexpr operator std::string_view( ) {
			return std::string_view( &m_data.begin( ), N );
		}

		constexpr cstr data( ) const {
			return &m_data.begin( );
		}
	};
} // namespace utils::cx

#endif // STRING_HPP