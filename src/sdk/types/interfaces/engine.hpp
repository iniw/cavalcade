#pragma once

namespace sdk::interfaces {
	struct engine {
	private:

		void get_screen_size( i32& w, i32& h ) {
			return mem::call_v_func< void, 5, i32&, i32& >( this, w, h );
		}

	public:

		bool is_in_game( ) {
			return mem::call_v_func< bool, 26 >( this );
		}

		math::v2i get_screen_size( ) {
			i32 w, h;
			get_screen_size( w, h );
			return math::v2i( w, h );
		}
	};
} // namespace sdk::interfaces