

#pragma once

#include "../../../other/other.hpp"

namespace hack {
	struct fog {
		void run( );
		void reset( );

		bool m_update{ true };
		bool m_fog_enabled{ };
		f32 m_fog_start{ };
		f32 m_fog_end{ };
		f32 m_fog_max_density{ };
		render::color m_fog_color_primary;
		render::color m_fog_color_secondary;
	};
} // namespace hack