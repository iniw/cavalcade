

#pragma once

#include "../../../sdk/types/interfaces/panorama.hpp"

namespace hack {
	struct scaleform {
		void install( );
		void update( );
		void reset( );

		sdk::interfaces::ui_panel* m_focus = nullptr;
		i32 m_old_hud_color;
	};
} // namespace hack
