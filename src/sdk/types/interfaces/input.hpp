#pragma once

#include "../user_cmd.hpp"

namespace sdk {
	struct verified_user_cmd {
		sdk::user_cmd m_cmd;
		CRC32_t m_crc;
	};
	namespace interfaces {
		struct input {
			verified_user_cmd* get_verified_cmd( i32 slot ) {
				auto verified_cmds = *( verified_user_cmd** )( ( uintptr_t )this + 0xF8 );
				if ( verified_cmds )
					return &verified_cmds[ slot ];
				return nullptr;
			}
		};
	} // namespace interfaces
} // namespace sdk