

#pragma once

#include "auxiliary/dlight.hpp"

namespace sdk::interfaces {
	struct effects {
		VFUNC( auxiliary::dlight*, alloc_dlight, 4, ( i32 key ), key );
	};
} // namespace sdk::interfaces