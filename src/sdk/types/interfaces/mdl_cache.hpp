#pragma once

namespace sdk::interfaces {
	struct mdl_cache {
		VFUNC( void, begin_lock, 33, ( ) );

		VFUNC( void, end_lock, 34, ( ) );
	};

} // namespace sdk::interfaces