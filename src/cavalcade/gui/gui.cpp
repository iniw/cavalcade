#include "gui.hpp"

void gui::impl::init( ) {
	using namespace objects;

	auto window = window::make( "cavalcade", { 550, 500 } );

	auto tab1 = window->add< tab >( "test" );
	{
		auto col = tab1->add< column >( );
		{
			auto gb1 = col->add< groupbox >( "poop", groupbox::preset::HALF );
			{
				gb1->add< checkbox >( XOR( "scaleform" ) );
				gb1->add< checkbox >( XOR( "zeus bug" ) );
				gb1->add< checkbox >( XOR( "trainer" ) );
				gb1->add< checkbox >( XOR( "graph" ) );
				gb1->add< slider< i32 > >( XOR( "graph size" ), 10, 200 );
				gb1->add< slider< f32 > >( XOR( "graph width scale" ), 1.F, 4.5F );
				gb1->add< slider< f32 > >( XOR( "graph height scale" ), 0.F, 1.F );
				gb1->add< checkbox >( XOR( "velocity ind" ) );
				gb1->add< checkbox >( XOR( "binds ind" ) );
				gb1->add< checkbox >( XOR( "wasd ind" ) );
				gb1->add< checkbox >( XOR( "box bob" ) );
				gb1->add< checkbox >( XOR( "longjump on edge" ) );
				gb1->add< checkbox >( XOR( "crouch after ladderjump" ) );
				gb1->add< checkbox >( XOR( "nightmode" ) );
				gb1->add< slider< i32 > >( XOR( "factor" ), 0, 100 );
				gb1->add< checkbox >( XOR( "sunset" ) );
				gb1->add< checkbox >( XOR( "fog" ) );
				gb1->add< slider< f32 > >( XOR( "fog end" ), 0.F, 1.F );
				gb1->add< slider< f32 > >( XOR( "fog density" ), 0.F, 1.F );
				gb1->add< slider< i32 > >( XOR( "edgebug scaling" ), 0, 100 );
				gb1->add< slider< i32 > >( XOR( "edgebug radius" ), 0, 128 );
				gb1->add< slider< i32 > >( XOR( "pixelsurf radius" ), 0, 4 );
				gb1->add< checkbox >( XOR( "on attack" ) );
				gb1->add< checkbox >( XOR( "silent" ) );
				gb1->add< slider< i32 > >( XOR( "fov" ), 0, 180 );
				gb1->add< slider< i32 > >( XOR( "smoothing" ), 0, 100 );
				gb1->add< checkbox >( XOR( "dlights" ) );
				gb1->add< slider< i32 > >( XOR( "bw" ), 0, 100 );
				gb1->add< slider< i32 > >( XOR( "bh" ), 0, 100 );
			}

			auto gb2 = col->add< groupbox >( "poop 2", groupbox::preset::HALF );
			{
				listbox::options vals{ };
				for ( int i = 1; i < 20; i++ )
					vals.push_back( fmt::format( "asd {}", i ) );

				auto lbox = gb2->add< listbox >( "listbox", vals, 5 );

				auto select = gb2->add< selectable >( "selectableIPFGODPDO", selectable::options{ "test", "yeah" } );

				for ( int i = 1; i < 5; i++ )
					gb2->add< checkbox >( fmt::format( "checkbox {}", i ) );
			}
		}

		auto tab_gb = tab1->add< groupbox >( "poopage", groupbox::preset::FULL );
		{
			auto inner_tab1 = tab_gb->add< tab >( "tab 1" );
			{
				auto gb1 = inner_tab1->add< groupbox >( "gb1", groupbox::preset::HALF );
				{
					for ( int i = 1; i <= 5; i++ )
						gb1->add< checkbox >( fmt::format( "checkbox {}", i ) );

					selectable::options vals{ };
					for ( int i = 1; i < 20; i++ )
						vals.push_back( fmt::format( "asd {}", i ) );

					auto select = gb1->add< selectable >( "selectable", vals );

					auto master = gb1->add< master_checkbox >( "mastah" );
					for ( int i = 1; i <= 5; i++ )
						master->add< slider< int > >( fmt::format( "slider {}", i ), 0, i * 20 );

					for ( int i = 0; i < 4; i++ )
						gb1->add< button >( "button", []( auto& ) {} );
				}

				auto gb2 = inner_tab1->add< groupbox >( "gb2", groupbox::preset::HALF );
				{
					for ( int i = 1; i <= 5; i++ )
						gb2->add< slider< int > >( fmt::format( "slider {}", i ), 0, i * 20 );

					auto mastah = gb2->add< master_checkbox >( "asdasfd" );
					for ( int i = 1; i <= 5; i++ )
						mastah->add< slider< int > >( fmt::format( "slider {}", i ), 0, i * 20 );

					gb2->add< colorpicker >( "colour picker" );
				}
			}

			auto inner_tab2 = tab_gb->add< tab >( "colors" );
			{
				auto colors = inner_tab2->add< groupbox >( "colors", groupbox::preset::FULL );
				{
					auto pm = colors->add< colorpicker >( "primary", false );
					pm->set_var( style::palette::primary );
					auto bg = colors->add< colorpicker >( "background", false );
					bg->set_var( style::palette::background );
					auto sc = colors->add< colorpicker >( "secondary", false );
					sc->set_var( style::palette::secondary );
					auto hl = colors->add< colorpicker >( "highlight", false );
					hl->set_var( style::palette::highlight );
					auto st = colors->add< colorpicker >( "stroke", false );
					st->set_var( style::palette::stroke );
				}
			}
		}
	}

	auto tab2 = window->add< tab >( "debug" );
	{
		tab2->add< button >( "draw all bounds", [ & ]( auto& obj ) { style::debug::render_bounds = !style::debug::render_bounds; } );
	}

	for ( int i = 3; i <= 5; i++ )
		window->add< tab >( fmt::format( "tab {}", i ) );

	window_list.push_back( std::move( window ) );

	g_io.log( XOR( "initialized gui, objects: {}" ), base_object::object_count( ) );
}

void gui::impl::think( ) {
	if ( window_list.empty( ) )
		init( );

	for ( auto& window : window_list ) {
		window->update_flags( );

		if ( window->think( ) )
			break;
	}
}

void gui::impl::animate( ) {
	if ( window_list.empty( ) )
		return;

	for ( auto& window : window_list )
		window->animate( );
}

void gui::impl::render( ) {
	if ( window_list.empty( ) )
		return;

	for ( const auto& window : window_list ) {
		window->render( );

		if ( window->flags( ).test( objects::flags::DEBUG_BOUNDS ) || style::debug::render_bounds )
			window->render_debug_bounds( );
	}
}