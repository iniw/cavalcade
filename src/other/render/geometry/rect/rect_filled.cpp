#include "../../types/types.hpp"
#include "rect_filled.hpp"

#include "imgui.h"

void render::geometry::rect_filled::draw( ) {
	s_draw_list->AddRectFilled( m_point1, m_point2, m_color, 0 );
}