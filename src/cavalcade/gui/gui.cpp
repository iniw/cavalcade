#include "gui.hpp"
#include "objects/objects.hpp"

bool gui::gui::init( ) {
	auto& window = m_windows.emplace_back( objects::window::make( "cavalde_gui", { 300, 300 } ) );

	return window != nullptr;
}

void gui::gui::render( ) {
	for ( auto& window : m_windows )
		window->render( );
}

void gui::gui::think( ) {
	for ( auto& window : m_windows )
		window->think( );
}