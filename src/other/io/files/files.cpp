#include "files.hpp"

bool io::files::init( ) {
	if ( !create_directories( ) )
		return false;

#ifdef IO_LOG_FILE

	m_log_file.open( m_directories[ dirs::LOGS ].append( XOR( "log.bm" ) ), std::ios::trunc );

	return m_log_file.good( );

#endif

	return true;
}

void io::files::unload( ) {
	m_log_file.close( );
}

bool io::files::create_directories( ) {
	bool ret = true;

	if ( PWSTR documents = nullptr; SUCCEEDED( LI_FN( SHGetKnownFolderPath )( FOLDERID_Documents, 0, nullptr, &documents ) ) ) {
		m_directories[ dirs::DOCS ].assign( documents );
		CoTaskMemFree( documents ); // lazy importer crashes on this function dunno why

	} else
		return false;

	m_directories[ dirs::CHEAT ].assign( m_directories[ dirs::DOCS ] ).append( XOR( "ecstasy.dev" ) );
	if ( !fs::exists( m_directories[ dirs::CHEAT ] ) )
		ret = fs::create_directory( m_directories[ dirs::CHEAT ] );

	m_directories[ dirs::LOGS ].assign( m_directories[ dirs::CHEAT ] ).append( XOR( "logs" ) );
	if ( !fs::exists( m_directories[ dirs::LOGS ] ) )
		ret = fs::create_directory( m_directories[ dirs::LOGS ] );

	m_directories[ dirs::CFG ].assign( m_directories[ dirs::CHEAT ] ).append( XOR( "cfg" ) );
	if ( !fs::exists( m_directories[ dirs::CFG ] ) )
		ret = fs::create_directory( m_directories[ dirs::CFG ] );

	m_directories[ dirs::NETVARS ].assign( m_directories[ dirs::CHEAT ] ).append( "netvars" );
	if ( !fs::exists( m_directories[ dirs::NETVARS ] ) )
		ret = fs::create_directory( m_directories[ dirs::NETVARS ] );

	return ret;
}