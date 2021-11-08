#include "gui.hpp"
#include "objects/objects.hpp"

bool gui::impl::init( ) {
	auto window = objects::window::make( "main", "cavalde_gui", { 300, 300 } );

	auto column = window->add< objects::column >( );
	{
		auto col_tab = column->add< objects::tab >( "asd", "asdasd" );
		{
			// the size of this groupbox has to be specified. look at the TODO on tab.cpp
			auto gb = col_tab->add< objects::groupbox >( "group", "test", 100 );
			{
				auto tab1 = gb->add< objects::tab >( "tab1", "test1" );
				{
					tab1->add< objects::checkbox >( "check2", "check2" );

					auto gbec = tab1->add< objects::groupbox >( "asd", "f2g" );
					gbec->add< objects::checkbox >( "check2", "check2" );

					auto col = tab1->add< objects::column >( );
					col->add< objects::checkbox >( "check1", "a" );

					tab1->add< objects::checkbox >( "check2", "juju" );
				}

				auto tab2 = gb->add< objects::tab >( "tab2", "test2" );
				{
					auto tab3 = tab2->add< objects::tab >( "o", "kjkj" );
					auto tab4 = tab2->add< objects::tab >( "o", "kjkj" );
					tab3->add< objects::checkbox >( "check3", "check3" );
					tab4->add< objects::checkbox >( "check4", "check4" );
				}
			}
		}
		auto col_tab2 = column->add< objects::tab >( "asdasd", "ooga" );
	}

	auto gb = window->add< objects::groupbox >( "group2", "test", -1 );
	{
		gb->add< objects::checkbox >( "check4", "check4" );
		gb->add< objects::checkbox >( "check5", "check5" );
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