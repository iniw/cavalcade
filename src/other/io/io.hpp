#ifndef IO_HPP
#define IO_HPP

#include "console/console.hpp"
#include "files/files.hpp"
#include "input/input.hpp"
#include "log_level/log_level.hpp"

namespace io {
	template< typename... VA >
	inline std::string format( std::string_view fmt, VA&&... args );

	BOOL CALLBACK wnd_proc( HWND window, UINT msg, WPARAM w_param, LPARAM l_param );

	struct io {
	private:

		console m_console;
		input m_input;
		files m_files;

	public:

		bool init( );

		void unload( );

		BOOL think( UINT msg, WPARAM w_param, LPARAM l_param );

		fs::path& directory( dirs id );

		math::v2i mouse_pos( );

		f32 mouse_scroll( );

		template< auto state = key_state::DOWN >
		bool key_state( u8 key_id );

		template< log_level level = log_level::NORMAL, typename... VA >
		void log( std::string_view str, VA&&... args );

		template< log_level level = log_level::NORMAL, typename... VA >
		void print( std::string_view str, VA&&... args );
	};
} // namespace io

inline io::io g_io;

#include "io.inl"

#endif // IO_HPP