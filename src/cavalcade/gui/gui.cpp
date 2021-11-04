#include "gui.hpp"
#include "objects/objects.hpp"

bool gui::impl::init( ) {
	auto window = objects::window::make( "main", "cavalde_gui", { 300, 300 } );
	{
		auto column = window->add< objects::column >( );
		{
			column->add< objects::checkbox >( "check2", "test2" );
			column->add< objects::checkbox >( "check3", "test3" );

			auto group = column->add< objects::groupbox >( "group1", "label", -1 );
			{
				group->add< objects::checkbox >( "check4", "test4" );
				group->add< objects::checkbox >( "check5", "test5" );
			}
		}
		window->add< objects::checkbox >( "check", "test" );
	}

	g_io.log( "initialized gui, objects: {}", objects::base_object::s_object_count );

	m_windows.emplace_back( window );

	return !m_windows.empty( );
}

void gui::impl::render( ) {
	// NOTE(wini): this may look weird, because it is, but it has a purpose:
	// imgui creates the font atlas on-demand, at the beginning of each frame (if it has to),
	// because of that, we need to make sure that it HAS a font atlas before initializing
	// and the easiest way to have that guarantee is to run it after calling g_render.begin()
	// on end_scene.
	if ( m_windows.empty( ) )
		init( );

	for ( auto& window : m_windows )
		window->render( );
}

void gui::impl::think( ) {
	if ( m_windows.empty( ) )
		return;

	for ( auto& window : m_windows )
		window->think( );
}