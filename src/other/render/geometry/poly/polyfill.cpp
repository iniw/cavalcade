#include "polyfill.hpp"

#include "imgui/imgui.h"

void render::geometry::polyfill::draw( ) {
	std::vector< ImVec2 > im_data( m_points.size( ) );
	for ( auto& p : m_points )
		im_data.push_back( p );
	s_draw_list->AddConvexPolyFilled( im_data.data( ), m_points.size( ), m_color.convert( ) );
}