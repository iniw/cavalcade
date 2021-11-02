#include "window.hpp"

void gui::objects::window::render( ) {
	g_render.rectangle( m_static_rect, render::color::black( ) ).outline( render::color::red( ) );

	return render_children( );
}

bool gui::objects::window::think( ) {
	if ( g_io.mouse_pos( ).in_rect( m_static_rect ) )
		g_io.print( "test" );

	return think_children( );
}