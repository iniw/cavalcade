#include "../../types/types.hpp"

#include "imgui.h"

#include "rect_filled.hpp"

void render::geometry::rect_filled::draw( ) {
	s_draw_list->AddRectFilled( m_point1, m_point2, m_color, 0 );
}