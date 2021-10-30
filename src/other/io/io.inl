#ifndef IO_INL
#define IO_INL

template< typename... VA >
inline std::string io::format( std::string_view fmt, VA&&... args ) {
	return fmt::vformat( fmt, fmt::make_format_args( args... ) );
}

template< auto state >
bool io::io::key_state( u8 key_id ) {
	auto& key_info = m_input.m_keys[ key_id ];

	if constexpr ( state == key_state::RELEASED ) {
		u64 time_delta = LI_FN( GetTickCount64 )( ) - key_info.m_time;

		if ( key_info.m_state == key_state::RELEASED )
			if ( time_delta <= input::MAX_TIME_DELTA )
				return true;
			else {
				key_info.m_state = key_state::UP;
				return false;
			}

		return false;

	} else
		return key_info.m_state == state;
}

template< io::log_level level, typename... VA >
void io::io::log( std::string_view str, VA&&... args ) {
	auto fmt = format( str, std::forward< VA >( args )... );

	m_console.print< level >( fmt );

	m_files.log< level >( fmt );
}

template< io::log_level level, typename... VA >
void io::io::print( std::string_view str, VA&&... args ) {
	auto fmt = format( str, std::forward< VA >( args )... );

	m_console.print< level >( fmt );
}

#endif // IO_INL