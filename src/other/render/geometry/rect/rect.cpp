#include "../types/types.hpp"

#include "rect.hpp"

#include "imgui.h"

void render::geometry::rect::draw( ) {
	s_draw_list->AddRect( m_point1, m_point2, m_color.to_imgui( ), 0.f, 0, m_thickness );
}

void render::geometry::rect::outline( color col, bool innerline ) {
	s_draw_list->AddRect( m_point1 - ( m_thickness - 1.f ) - 1, m_point2 + ( m_thickness - 1.f ) + 1, col.to_imgui( ), 0.f, 0, m_thickness );

	if ( innerline )
		s_draw_list->AddRect( m_point1 + ( m_thickness - 1.f ) + 1, m_point2 - ( m_thickness - 1.f ) - 1, col.to_imgui( ), 0.f, 0, m_thickness );
}