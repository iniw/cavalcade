#ifndef MOCKING_BIRD_HPP
#define MOCKING_BIRD_HPP

// False state mocking utility with pointer overloads
namespace mocking_bird {
	struct exception {
		constexpr exception( const std::source_location& source_location ) : m_source_location( source_location ) { }

		constexpr auto what( ) const {
			return m_source_location;
		}

	private:

		std::source_location m_source_location;
	};

	template< typename T >
	concept Pointer = ( std::is_pointer_v< T > || std::is_same_v< T, nullptr_t > );

	template< typename T >
	concept Operable = requires( T a ) {
		{ a == nullptr } -> std::convertible_to< bool >;
	};

	struct raiser {
		raiser( ) = delete;

		constexpr raiser( const std::source_location& source_location ) : m_source_location( source_location ) { }

		template< typename T >
			requires Pointer< T > && Operable< T >
		constexpr void set( T from ) {
			m_content = ( from != nullptr );
		}

		constexpr void set( bool from ) {
			m_content = from;
		}

		constexpr bool get( ) const {
			return ( m_content == 0 );
		}

		constexpr auto test( ) const {
			return get( ) ? ( std::make_optional( exception( m_source_location ) ) ) : std::nullopt;
		}

		template< typename T >
			requires Pointer< T > && Operable< T >
		constexpr void operator,( T lhs ) {
			set( lhs );

			const auto ex = test( );
			if ( ex.has_value( ) ) {
				throw ex.value( );
			}
		}

		constexpr void operator,( bool lhs ) {
			set( lhs );

			const auto ex = test( );
			if ( ex.has_value( ) ) {
				throw ex.value( );
			}
		}

	private:

		bool m_content = false;
		std::source_location m_source_location;
	};
} // namespace mocking_bird

#define MOCK mocking_bird::raiser{ std::source_location::current( ) },
// NOTE(para): @iniw your symbol name reusage is very unsafe. example: try MOCKING_REGION on io struct without global preface
#define MOCKING_REGION_IMPL( ... )                                                                                                                   \
	try {                                                                                                                                            \
		__VA_ARGS__                                                                                                                                  \
	} catch ( const mocking_bird::exception& err ) {                                                                                                 \
		g_io.log< ::io::log_level::WARNING >( "MOCK failed, line: {} | file: {} | fn: {}", err.what( ).line( ), err.what( ).file_name( ),            \
		                                      err.what( ).function_name( ) );

#define MOCKING_REGION( ... )                                                                                                                        \
	MOCKING_REGION_IMPL( __VA_ARGS__ ) return false;                                                                                                 \
	}

#define MOCKING_REGION_NO_RET( ... )                                                                                                                 \
	MOCKING_REGION_IMPL( __VA_ARGS__ )                                                                                                               \
	}

#endif /* MOCKING_BIRD_HPP */