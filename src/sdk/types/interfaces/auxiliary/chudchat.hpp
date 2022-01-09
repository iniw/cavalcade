#ifndef CHUDCHAT_HPP
#define CHUDCHAT_HPP

#pragma once

namespace sdk::auxiliary {
	struct c_hud_chat {
	private:

		enum idx
		{ CHAT_PRINTF_INDEX = 27, };

	public:

		template< typename... _Tys >
		__forceinline void chat_printf( int player_index, int filter, const char* fmt, _Tys... args ) noexcept {
			return mem::call_v_func_cdecl< void, idx::CHAT_PRINTF_INDEX >( this, player_index, filter, fmt, args... );
		}
	};
} // namespace sdk::auxiliary

#endif /* CHUDCHAT_HPP */
