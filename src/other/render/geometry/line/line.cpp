#include "line.hpp"

#include "imgui.h"

void render::geometry::line::draw( ) {
	s_draw_list->AddLine( m_point1, m_point2, m_color.to_imgui( ), m_thickness );
}