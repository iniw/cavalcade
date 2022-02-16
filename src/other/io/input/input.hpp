#pragma once

#include "../../math/types/vector.hpp"

namespace io {
	enum key_state
	{
		NONE,
		UP,
		DOWN,
		RELEASED
	};

	struct input {
	private:

		struct key_info {
			key_state m_state;
			u64 m_time;

			constexpr key_info( ) : m_state{ key_state::NONE }, m_time{ 0 } { }

			constexpr key_info( key_state state, u64 time ) : m_state{ state }, m_time{ time } { }
		};

	public:

		static inline WNDPROC s_og;

		constexpr static inline u64 MAX_RELEASED_TIME = 100;

		std::array< key_info, 255 > m_keys{ };
		std::array< std::string, 255 > m_key_names{ };

		struct {
			math::v2i pos;
			math::v2i delta;
			i32 scroll_amt;
			i32 scroll_buffer;
		} m_mouse;

	public:

		bool init( LONG_PTR wnd_proc );

		void unload( );

		BOOL think( UINT msg, WPARAM w_param, LPARAM l_param );
	};
} // namespace io