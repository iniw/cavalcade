#pragma once

template< typename... VA >
inline std::string io::format( std::string_view fmt, VA&&... args ) {
	return fmt::vformat( fmt, fmt::make_format_args( std::forward< VA >( args )... ) );
}

template< auto state >
bool io::impl::key_state( u8 key_id ) {
	auto& key = m_input.m_keys[ key_id ];

	u64 time = LI_FN( GetTickCount64 )( );

	// NOTE(wini): this is hacky af but windows api forces me
	// desperate times require desperate actions.
	if constexpr ( state == key_state::RELEASED ) {
		if ( key.m_state == state )                              // 1. if our key is released
			if ( time - key.m_time <= input::MAX_RELEASED_TIME ) // 2. if it was released recently enough
				return key.m_state = key_state::UP;              // ok, the key has been released in a valid time delta, reset it and return true
			else                                                 // 2. it's been too long, reset it
				key.m_state = key_state::UP;

		return false; // return false if any of our criterias fail
	} else
		return key.m_state == state;
}

template< io::log_level level, typename... VA >
void io::impl::log( std::string_view str, VA&&... args ) {
#if _DEBUG
	auto fmt = io::format( str, std::forward< VA >( args )... );

	m_console.print< level >( fmt );
	m_files.log< level >( fmt );
#endif
}

template< io::log_level level, typename... VA >
void io::impl::print( std::string_view str, VA&&... args ) {
#if _DEBUG
	auto fmt = io::format( str, std::forward< VA >( args )... );

	m_console.print< level >( fmt );
#endif
}