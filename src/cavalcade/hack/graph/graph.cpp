#include "graph.hpp"
#include "../../ctx/ctx.hpp"
#include "../../gui/gui.hpp"

constexpr auto g_vel_graph_size  = 80;
constexpr auto g_vel_graph_scale = 4.F;

void hack::graph::gather( ) {
	static auto& gs = gui::cfg::get< i32 >( HASH_CT( "main:group1:graph size" ) );
	if ( m_data.size( ) > gs ) {
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

	static auto& gs  = gui::cfg::get< i32 >( HASH_CT( "main:group1:graph size" ) );
	static auto& gsc = gui::cfg::get< f32 >( HASH_CT( "main:group1:graph scale" ) );

	for ( auto i = 0; i < m_data.size( ) - 1; ++i ) {
		const auto& c = m_data[ i ];
		const auto& n = m_data[ i + 1 ];

		auto ss  = g_render.get_screen_size( );
		auto pad = ceil( ss[ 1 ] * .12F );
		auto pos = render::point{ ss[ 0 ] / 2 + gs * ( gsc / 2.F ), ss[ 1 ] / 1.2F + pad };

		auto alpha = std::max( 10, std::min( 255, c.m_speed + 5 ) ) * .75F;

		g_render.m_safe.draw_shape< render::geometry::line >( render::point{ pos[ 0 ] - ( i - 1 ) * gsc, pos[ 1 ] - c.m_speed },
		                                                      render::point{ pos[ 0 ] - i * gsc, pos[ 1 ] - n.m_speed },
		                                                      render::color( 255, 255, 255, alpha ), 1.F );
	}
}

void hack::graph::clear( ) {
	m_data.clear( );
}

i32 hack::graph::get_upmost_y_scenario( ) {
	auto ss  = g_render.get_screen_size( );
	auto pad = ceil( ss[ 1 ] * .12F );
	return ( ss[ 1 ] / 1.2F + pad ) - 250;
}