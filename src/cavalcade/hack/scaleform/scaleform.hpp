

#pragma once

#include "../../../sdk/types/interfaces/panorama.hpp"

namespace hack {
	struct scaleform {
		void install( );
		void update( );
		void reset( );

		sdk::interfaces::ui_panel* m_focus = nullptr;
		i32 m_old_hud_color{ -1 };
		i32 m_old_weaponselect{ -1 };
		i32 m_pending_ticks{ -1 };
	};
} // namespace hack
