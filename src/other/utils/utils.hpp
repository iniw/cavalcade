#ifndef UTILS_HPP
#define UTILS_HPP

#include "cx/cx.hpp"
#include "str/str.hpp"

namespace utils {
	BOOL create_thread( DWORD WINAPI function( unk ), unk parameter );

	void sleep( u32 time );

	template< bool abort, typename... VA >
	inline void enforce( bool expression, std::string_view id, std::string_view msg, VA&&... fmt );
} // namespace utils

// normal enforce, replaces assert( )
#define ENFORCE( expression, msg, ... ) utils::enforce< false >( expression, XOR( #expression ), XOR( msg ), __VA_ARGS__ )
// hard enforce, unloads the cheat on fail
#define HENFORCE( expression, msg, ... ) utils::enforce< true >( expression, XOR( #expression ), XOR( msg ), __VA_ARGS__ )

#include "utils.inl"

#endif // UTILS_HPP