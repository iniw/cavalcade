#include "gui.hpp"
#include "objects/objects.hpp"

void gui::impl::init( ) {
	auto window = objects::window::make( "main", "cavalcade_menu", { 300, 300 } );

	auto gb = window->add< objects::groupbox >( "group1", "test", 70 );
	{
		gb->add< objects::checkbox >( "check1", "check 1" );
		gb->add< objects::checkbox >( "on attack", "on attack" );
		gb->add< objects::slider< i32 > >( "smoothing", "smoothing", 0, 100 );
		gb->add< objects::slider< i32 > >( "bw", "bw", 0, 100 );
		gb->add< objects::slider< i32 > >( "bh", "bh", 0, 100 );
	}

	auto gb3 = window->add< objects::groupbox >( "group2", "test 3", 90 );
	{
		auto tab1 = gb3->add< objects::tab >( "tab1", "tab" );
		{ }
		auto tab2 = gb3->add< objects::tab >( "tab2", "tab" );
		{
			tab2->add< objects::checkbox >( "check3", "check 2" );
			tab2->add< objects::slider< i32 > >( "slider4", "slider 2", 0, 100 );
			tab2->add< objects::slider< f32 > >( "slider5", "slider 3", 0.f, 100.f );
			tab2->add< objects::slider< f32 > >( "slider6", "slider 4", 0.f, 100.f );
		}
	}

	m_windows.emplace_back( window );

	g_io.log( XOR( "initialized gui, objects: {}" ), objects::base_object::s_object_count );
}

void gui::impl::render( ) {
	// NOTE(wini): this may look weird, because it is, but it has a purpose:
	// imgui creates the font atlas on-demand, at the beginning of each frame (if it has to),
	// because of that, we need to make sure that it HAS a font atlas before initializing
	// and the easiest way to have that guarantee is to run it after calling g_render.begin()
	// on end_scene.
	if ( m_windows.empty( ) )
		init( );

	for ( const auto& window : m_windows )
		window->render( );
}

void gui::impl::think( ) {
	if ( m_windows.empty( ) )
		return;

	for ( auto& window : m_windows )
		window->think( );
}