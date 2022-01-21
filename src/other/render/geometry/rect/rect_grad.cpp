#include "../../types/types.hpp"
#include "rect_grad.hpp"

#include "imgui.h"

void render::geometry::rect_grad::draw( ) {
	if ( m_horizontal )
		s_draw_list->AddRectFilledMultiColor( m_point1, m_point2, m_color.to_imgui( ), m_color2.to_imgui( ), m_color2.to_imgui( ),
		                                      m_color.to_imgui( ) );
	else
		s_draw_list->AddRectFilledMultiColor( m_point1, m_point2, m_color.to_imgui( ), m_color.to_imgui( ), m_color2.to_imgui( ),
		                                      m_color2.to_imgui( ) );
}