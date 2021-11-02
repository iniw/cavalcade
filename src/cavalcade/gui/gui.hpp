#ifndef GUI_HPP
#define GUI_HPP

#include "objects/objects.hpp"

namespace gui {

	// NOTE(wini): these are kinda random and aren't going to be used for long
	namespace pallete {
		inline auto DARK  = render::color( 25, 25, 25 );
		inline auto LIGHT = render::color( 65, 65, 65 );
	}; // namespace pallete

	struct gui {
	private:

		// NOTE(wini): this is some future-proofing for when we want to;
		// create more windows, like spectator lists and stuff
		std::vector< objects::window::ptr > m_windows;

	public:

		bool init( );

		void render( );

		void think( );
	};
} // namespace gui

inline gui::gui g_gui;

#endif // GUI_HPP