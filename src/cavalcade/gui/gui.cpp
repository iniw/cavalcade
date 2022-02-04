#include "gui.hpp"
#include "objects/objects.hpp"

void gui::impl::init( ) {
	auto window = objects::window::make( XOR( "main" ), XOR( "cavalcade_menu" ), { 300, 300 } );

	auto gb = window->add< objects::groupbox >( XOR( "group1" ), XOR( "test" ), 70 );
	{
		gb->add< objects::checkbox >( XOR( "check1" ), "check 1" );
		gb->add< objects::checkbox >( XOR( "zeus bug" ), XOR( "zeus bug" ) );
		gb->add< objects::checkbox >( XOR( "trainer" ), XOR( "trainer" ) );
		gb->add< objects::checkbox >( XOR( "graph" ), XOR( "graph" ) );
		gb->add< objects::slider< i32 > >( XOR( "graph size" ), XOR( "graph size" ), 10, 200 );
		gb->add< objects::slider< f32 > >( XOR( "graph scale" ), XOR( "graph scale" ), 1.F, 4.5F );
		gb->add< objects::checkbox >( XOR( "velocity ind" ), XOR( "velocity ind" ) );
		gb->add< objects::checkbox >( XOR( "binds ind" ), XOR( "binds ind" ) );
		gb->add< objects::checkbox >( XOR( "wasd ind" ), XOR( "wasd ind" ) );
		gb->add< objects::checkbox >( XOR( "box bob" ), XOR( "box bob" ) );
		gb->add< objects::checkbox >( XOR( "longjump on edge" ), XOR( "longjump on edge" ) );
		gb->add< objects::checkbox >( XOR( "crouch after ladderjump" ), XOR( "crouch after ladderjump" ) );
		gb->add< objects::checkbox >( XOR( "nightmode" ), XOR( "nightmode" ) );
		gb->add< objects::slider< i32 > >( XOR( "factor" ), XOR( "factor" ), 0, 100 );
		gb->add< objects::checkbox >( XOR( "fog" ), XOR( "fog" ) );
		gb->add< objects::slider< f32 > >( XOR( "fog end" ), XOR( "fog end" ), 0.F, 1.F );
		gb->add< objects::slider< f32 > >( XOR( "fog density" ), XOR( "fog density" ), 0.F, 1.F );
		gb->add< objects::slider< i32 > >( XOR( "edgebug scaling" ), XOR( "edgebug scaling" ), 0, 100 );
		/* NOTE: default value: 32 */
		gb->add< objects::slider< i32 > >( XOR( "edgebug radius" ), XOR( "edgebug radius" ), 0, 128 );
		/* NOTE: default value: 0.4 */
		gb->add< objects::slider< f32 > >( XOR( "edgebug margin of error" ), XOR( "edgebug margin of error" ), 0.F, 1.F );
		gb->add< objects::checkbox >( XOR( "on attack" ), XOR( "on attack" ) );
		gb->add< objects::slider< i32 > >( XOR( "fov" ), XOR( "fov" ), 0, 180 );
		gb->add< objects::slider< i32 > >( XOR( "smoothing" ), XOR( "smoothing" ), 0, 100 );
		gb->add< objects::slider< i32 > >( XOR( "bw" ), XOR( "bw" ), 0, 100 );
		gb->add< objects::slider< i32 > >( XOR( "bh" ), XOR( "bh" ), 0, 100 );
	}

	auto gb3 = window->add< objects::groupbox >( XOR( "group2" ), ( "test 3" ), 90 );
	{
		auto tab1 = gb3->add< objects::tab >( XOR( "tab1" ), XOR( "tab" ) );
		{ }
		auto tab2 = gb3->add< objects::tab >( XOR( "tab2" ), XOR( "tab" ) );
		{
			tab2->add< objects::checkbox >( XOR( "check3" ), XOR( "check 2" ) );
			tab2->add< objects::slider< i32 > >( XOR( "slider4" ), XOR( "slider 2" ), 0, 100 );
			tab2->add< objects::slider< f32 > >( XOR( "slider5" ), XOR( "slider 3" ), 0.f, 100.f );
			tab2->add< objects::slider< f32 > >( XOR( "slider6" ), XOR( "slider 4" ), 0.f, 100.f );
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