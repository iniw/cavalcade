#ifndef INPUT_HPP
#define INPUT_HPP

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

		constexpr static inline u64 MAX_TIME_DELTA = 100;

		std::array< key_info, 255 > m_keys;
		static inline std::array< std::string, 255 > s_key_names = { };

	public:

		bool init( LONG_PTR wnd_proc );

		void unload( );

		BOOL think( UINT msg, WPARAM w_param, LPARAM l_param );

		static void get_key_names( );
	};
} // namespace io

#endif // INPUT_HPP
