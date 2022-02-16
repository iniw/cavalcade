#include "rect.hpp"
#include "../../io/io.hpp"

bool render::rect::hovered( ) const {
	return g_io.mouse_pos( ).in_rect( *this );
}

f32 render::rect::mouse_percent_x( bool uncapped ) const {
	f32 x_offset = g_io.mouse_pos( ).x - x;

	f32 percent = x_offset / w;
	if ( !uncapped )
		percent = std::clamp( percent, 0.f, 1.f );

	return percent;
}

f32 render::rect::mouse_percent_y( bool uncapped ) const {
	f32 y_offset = g_io.mouse_pos( ).y - y;

	f32 percent = y_offset / h;
	if ( !uncapped )
		percent = std::clamp( percent, 0.f, 1.f );

	return percent;
}

void render::rect::clamp_to( const rect& rect ) {
	// first try to fit the rect inside the other rect by keeping the same width
	int overflow_x = x2( ) - rect.x2( );
	if ( overflow_x > 0 ) {
		x -= overflow_x;
	}

	int overflow_y = y2( ) - rect.y2( );
	if ( overflow_y > 0 ) {
		y -= overflow_y;
	}

	// if the width still doesnt fit after being shifted then clamp the width
	int underflow_x = rect.x - x;
	if ( underflow_x > 0 ) {
		x += underflow_x;
	}

	int underflow_y = rect.y - y;
	if ( underflow_y > 0 ) {
		y += underflow_y;
	}

	// todo: if it still doesnt fit change width
}