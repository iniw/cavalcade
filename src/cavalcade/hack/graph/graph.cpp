#include "graph.hpp"
#include "../../ctx/ctx.hpp"

constexpr auto g_vel_graph_size  = 80;
constexpr auto g_vel_graph_scale = 4.F;

void hack::graph::gather( ) {
	if ( m_data.size( ) > g_vel_graph_size ) {
		m_data.pop_back( );
	}

	auto curdata        = data{ };
	curdata.m_speed     = std::min( static_cast< i32 >( g_ctx.m_local.get( ).get_velocity( ).length_2d( ) ), 250 );
	curdata.m_on_ground = g_ctx.m_local.get( ).get_flags( ) & sdk::flags::ONGROUND;

	m_data.push_front( std::move( curdata ) );
}

void hack::graph::draw( ) {
	if ( m_data.size( ) < 2 )
		return;

	for ( auto i = 0; i < m_data.size( ) - 1; ++i ) {
		const auto& c = m_data[ i ];
		const auto& n = m_data[ i + 1 ];

		auto pos = render::point{ g_render.get_screen_size( )[ 0 ] / 2 + g_vel_graph_size * ( g_vel_graph_scale / 2.F ),
			                      g_render.get_screen_size( )[ 1 ] / 1.2F + 130 };

		auto alpha = std::max( 10, std::min( 255, c.m_speed + 5 ) ) * .75F;

		g_render.m_safe.draw_shape< render::geometry::line >( render::point{ pos[ 0 ] - ( i - 1 ) * g_vel_graph_scale, pos[ 1 ] - c.m_speed },
		                                                      render::point{ pos[ 0 ] - i * g_vel_graph_scale, pos[ 1 ] - n.m_speed },
		                                                      render::color( 255, 255, 255, alpha ), 1.F );
	}
}

void hack::graph::clear( ) {
	m_data.clear( );
}

i32 hack::graph::get_upmost_y_scenario( ) {
	return ( g_render.get_screen_size( )[ 1 ] / 1.2F + 130 ) - 250;
}