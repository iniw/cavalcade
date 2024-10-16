#pragma once

template< io::log_level level >
void io::files::log( std::string_view str ) {
#if _DEBUG
#	ifdef IO_LOG_FILE

	auto& level_options = level_handler[ level ];

	auto local_time = fmt::localtime( std::time( nullptr ) );

	auto fmt_str = fmt::format( XOR( "[ ecstasy.dev - {:%H:%M:%S} ] {}{}\n" ), local_time, level_options.get_prefix( ), str );
	m_log_file << fmt_str;
	m_log_file.flush( );

#	endif
#endif
}