

#pragma once

#include "material.hpp"

struct Rect_t {
	int x, y;
	int width, height;
};
namespace sdk {
	struct texture {
		VFUNC( i32, get_actual_width, 3, ( ) );
		VFUNC( i32, get_actual_height, 4, ( ) );
	};

	struct mat_render_context {
		VFUNC( i32, release, 1, ( ) );
		VFUNC( void, set_render_target, 6, ( texture * t ), t );
		VFUNC( void, get_render_target_dimensions, 8, ( i32 & w, i32& h ), std::ref( w ), std::ref( h ) );
		VFUNC( void, set_frame_buffer_copy_texture, 20, ( texture * t, i32 id ), t, id );
		VFUNC( void, get_viewport, 41, ( i32 & x, i32& y, i32& w, i32& h ), std::ref( x ), std::ref( y ), std::ref( w ), std::ref( h ) );
		VFUNC( void, draw_screen_space_rectangle, 114,
		       ( material * mat, i32 dest_x, i32 dest_y, i32 width, i32 height, f32 src_texture_x1, f32 src_texture_y1, f32 src_texture_x2,
		         f32 src_texture_y2, i32 src_texture_width, i32 src_texture_height, void* client_renderable = nullptr, i32 x_dice = 1,
		         i32 y_dice = 1 ),
		       mat, dest_x, dest_y, width, height, src_texture_x1, src_texture_y1, src_texture_x2, src_texture_y2, src_texture_width,
		       src_texture_height, client_renderable, x_dice, y_dice );
		VFUNC( void, push_render_target_and_viewport, 119, ( ) );
		VFUNC( void, pop_render_target_and_viewport, 120, ( ) );
		VFUNC( void, begin_pix_event, 143, ( unsigned long clr, cstr name ), clr, name );
		VFUNC( void, end_pix_event, 144, ( unsigned long clr, cstr name ), clr, name );
		VFUNC( void, copy_Render_target_to_texture_ex, 122, ( texture * t, i32 id, Rect_t* src, Rect_t* dest ), t, id, src, dest );
	};
} // namespace sdk