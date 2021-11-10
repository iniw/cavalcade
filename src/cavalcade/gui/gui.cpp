#include "gui.hpp"
#include "objects/objects.hpp"

void gui::impl::init( ) {
	auto window = objects::window::make( "main", "cavalde_gui", { 300, 300 } );

	auto column = window->add< objects::column >( );
	{
		auto col_tab = column->add< objects::tab >( "asd", "asdasd" );
		{
			// the size of this groupbox has to be specified. look at the TODO on tab.cpp
			auto gb = col_tab->add< objects::groupbox >( "group", "test", 100 );
			{
				gb->add< objects::slider< f32 > >( "sl", "labl2", 0.f, 100.f );
				gb->add< objects::slider< f32 > >( "sl", "labl2", 0.f, 100.f, 5 );
			}
		}
	}

	auto gb = window->add< objects::groupbox >( "group2", "test", -1 );
	{
		gb->add< objects::checkbox >( "check4", "check4" );
		gb->add< objects::checkbox >( "check5", "check5" );
		gb->add< objects::slider< i32 > >( "sl", "labl", 0, 100 );
		gb->add< objects::slider< f32 > >( "sl", "labl2", 0.f, 100.f );
		gb->add< objects::slider< f32 > >( "sl", "labl2", 0.f, 100.f, 5 );
	}

	m_windows.emplace_back( window );

	g_io.log( "initialized gui, objects: {}", objects::base_object::s_object_count );
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