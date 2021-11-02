#include "gui.hpp"
#include "objects/objects.hpp"

bool gui::gui::init( ) {
	auto window = objects::window::make( "main", "cavalde_gui", { 300, 300 } );

	auto checkbox = window->add< objects::checkbox >( "check", "test" );

	// add all of our windows
	m_windows.push_back( window );

	return !m_windows.empty( );
}

void gui::gui::render( ) {
	for ( auto& window : m_windows )
		window->render( );
}

void gui::gui::think( ) {
	for ( auto& window : m_windows )
		window->think( );
}