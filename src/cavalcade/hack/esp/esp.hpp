

#pragma once

#include "../../../other/other.hpp"
#include "../../../other/lib/include/animator/animator.hpp"

namespace hack {
	struct esp {
		void run( );
		void clear( );

		std::unordered_map< i32, animator > m_alpha{ };
	};
} // namespace hack