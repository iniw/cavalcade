#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#pragma once

#include "auxiliary/dlight.hpp"

namespace sdk::interfaces {
	struct effects {
		VFUNC( auxiliary::dlight*, alloc_dlight, 5, ( i32 key ), key );
	};
} // namespace sdk::interfaces

#endif /* EFFECTS_HPP */
