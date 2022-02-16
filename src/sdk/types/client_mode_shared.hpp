#pragma once

#include "interfaces/auxiliary/hud_chat.hpp"

namespace sdk {
	struct client_mode_shared {
		PAD( 28 );
		auxiliary::hud_chat* m_chat_element;
		PAD( 8 );
		math::v2i m_root_size;
	};

} // namespace sdk