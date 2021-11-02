#include "gui.hpp"
#include "objects/objects.hpp"

bool gui::impl::init( ) {
	auto& window = m_windows.emplace_back( objects::window::make( "main", "cavalde_gui", { 300, 300 } ) );

	auto checkbox = window->add< objects::checkbox >( "check", "test" );

	return !m_windows.empty( );
}

void gui::impl::render( ) {
	for ( auto& window : m_windows )
		window->render( );
}

void gui::impl::think( ) {
	for ( auto& window : m_windows )
		window->think( );
}