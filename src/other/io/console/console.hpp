#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "../log_level/log_level.hpp"

namespace io {
	struct console {
		bool init( );

		void unload( );

		template< log_level level >
		void print( std::string_view str );
	};
} // namespace io

#include "console.inl"

#endif // CONSOLE_HPP