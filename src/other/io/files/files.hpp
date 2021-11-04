#pragma once

#include "../log_level/log_level.hpp"

namespace io {
	enum dirs
	{
		DOCS,    // user/documents
		CHEAT,   // user/documents/cavalcade
		LOGS,    // user/documents/cavalcade/logs
		CFG,     // user/documents/cavalcade/cfg
		NETVARS, // user/documents/cavalcade/netvars
		MAX
	};

	struct files {
		std::array< fs::path, dirs::MAX > m_directories;

	private:

		std::ofstream m_log_file;

	public:

		bool init( );

		void unload( );

		template< log_level level >
		void log( std::string_view str );

	private:

		bool create_directories( );
	};
} // namespace io

#include "files.inl"