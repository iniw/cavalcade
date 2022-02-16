#include "../../types/types.hpp"
#include "rect_grad.hpp"

#include "imgui/imgui.h"

void render::geometry::rect_grad::draw( ) {
	if ( m_horizontal )
		s_draw_list->AddRectFilledMultiColor( m_point1, m_point2, m_color.convert( ), m_color2.convert( ), m_color2.convert( ), m_color.convert( ) );
	else
		s_draw_list->AddRectFilledMultiColor( m_point1, m_point2, m_color.convert( ), m_color.convert( ), m_color2.convert( ), m_color2.convert( ) );
}