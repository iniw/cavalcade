#pragma once

#include "objects/objects.hpp"

namespace gui {
	struct impl {
		// NOTE(wini): this is some future-proofing for when we want to;
		// create more windows, like spectator lists and stuff
		std::vector< std::unique_ptr< objects::window > > window_list;

		void init( );

		void think( );

		void animate( );

		void render( );
	};
} // namespace gui

inline gui::impl g_gui;