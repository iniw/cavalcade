

#pragma once

#include "../../../other/other.hpp"

namespace hack {
	struct trainer {
		std::vector< std::pair< math::v3f, math::ang > > m_checkpoints;
		i32 m_entry;

		void go( );
		void run( );
		void clear( );
	};
} // namespace hack