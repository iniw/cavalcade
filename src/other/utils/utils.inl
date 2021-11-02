#ifndef UTILS_INL
#define UTILS_INL

template< bool abort, typename... VA >
bool utils::enforce( bool expression, std::string_view id, std::string_view msg, VA&&... fmt ) {
	constexpr io::log_level level = abort ? io::log_level::DISASTER : io::log_level::WARNING;

	if ( !expression ) {
		std::string fmt_msg = io::format( msg, std::forward< VA >( fmt )... );

		g_io.log< level >( XOR( "enforce failed: id=\"{}\" - msg=\"{}\"" ), id, fmt_msg );

#ifndef NDEBUG

		MessageBox( cavalcade::window, fmt_msg.data( ), XOR( "enforce failed" ), MB_OK );

#endif

		if constexpr ( abort )
			cavalcade::unload( EXIT_FAILURE );
	}

	return expression;
}

#endif // UTILS_INL