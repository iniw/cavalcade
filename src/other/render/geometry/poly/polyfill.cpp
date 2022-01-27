#include "polyfill.hpp"

#include "imgui.h"

void render::geometry::polyfill::draw( ) {
	s_draw_list->AddConvexPolyFilled( m_points.data( ), m_points.size( ), m_color.to_imgui( ) );
}