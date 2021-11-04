#pragma once

#include "objects/objects.hpp"

namespace gui {
	struct impl {
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

inline gui::impl g_gui;