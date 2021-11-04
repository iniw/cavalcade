#pragma once

template< io::log_level level >
void io::console::print( std::string_view str ) {
#ifdef IO_LOG_CONSOLE

	auto& level_options = level_handler[ level ];

	auto local_time = fmt::localtime( std::time( nullptr ) );

	fmt::print( "[ {:%H:%M:%S} ] ", local_time );
	fmt::print( level_options.get_style( ), "{}{}\n", level_options.get_prefix( ), str );

#endif
}

#pragma once