#pragma once

#include "../../../sdk/types/interfaces/physics_collisions.hpp"

namespace hack {
	struct weather {
		void run( );
		void reset( );

		sdk::interfaces::vcollide m_collide{ };

		bool m_created{ false };
		std::optional< i32 > m_last_type = std::nullopt;
	};
} // namespace hack