#include "graph.hpp"
#include "../../ctx/ctx.hpp"
#include "../../gui/gui.hpp"

constexpr auto g_vel_graph_size  = 80;
constexpr auto g_vel_graph_scale = 4.F;

void hack::graph::gather( ) {
	static auto& g = gui::cfg::get< bool >( HASH_CT( "graph" ) );

	if ( !g )
		return;

	static auto& gs = gui::cfg::get< i32 >( HASH_CT( "graph size" ) );
	if ( m_data.size( ) > gs ) {
		m_data.pop_back( );
	}

	auto curdata        = data{ };
	curdata.m_speed     = std::min( static_cast< i32 >( g_ctx.m_local.get( ).get_velocity( ).length_2d( ) ), 250 );
	curdata.m_on_ground = g_ctx.m_local.get( ).get_flags( ) & sdk::flags::ONGROUND;

	m_data.push_front( std::move( curdata ) );
}

void hack::graph::draw( ) {
	static auto& g    = gui::cfg::get< bool >( HASH_CT( "graph" ) );
	static auto& gs   = gui::cfg::get< i32 >( HASH_CT( "graph size" ) );
	static auto& gsc  = gui::cfg::get< f32 >( HASH_CT( "graph width scale" ) );
	static auto& gsch = gui::cfg::get< f32 >( HASH_CT( "graph height scale" ) );

	if ( !g ) {
		m_data.clear( );
		return;
	}

	if ( gs == 0 || gsc == 0.F )
		return;

	if ( gs != m_old_gs ) {
		m_old_gs = gs;
		m_data.clear( );
		return;
	}

	if ( gsc != m_old_gsc ) {
		m_old_gsc = gsc;
		m_data.clear( );
		return;
	}

	if ( m_data.size( ) < 2 )
		return;

	static auto& wasd = gui::cfg::get< bool >( HASH_CT( "wasd ind" ) );

	for ( auto i = 0; i < m_data.size( ) - 1; ++i ) {
		const auto& c = m_data[ i ];
		const auto& n = m_data[ i + 1 ];

		auto ss  = g_render.get_screen_size( );
		auto pad = ceil( ss[ 1 ] * .12F );
		auto pos = render::point( ss[ 0 ] / 2 + gs * ( gsc / 2.F ), ss[ 1 ] / 1.2F + pad - ( wasd ? 5 : 0 ) );

		auto alpha = std::max( 25, std::min( 255, c.m_speed + 5 ) ) * .75F;

		g_render.m_safe.draw_shape< render::geometry::line >( render::point( pos[ 0 ] - ( i - 1 ) * gsc, pos[ 1 ] - ( c.m_speed * gsch ) ),
		                                                      render::point( pos[ 0 ] - i * gsc, pos[ 1 ] - ( n.m_speed * gsch ) ),
		                                                      render::color( 255, 255, 255, alpha ), 1.F );
	}
}

void hack::graph::clear( ) {
	m_data.clear( );
}

i32 hack::graph::get_upmost_y_scenario( ) {
	static auto& g    = gui::cfg::get< bool >( HASH_CT( "graph" ) );
	static auto& gs   = gui::cfg::get< i32 >( HASH_CT( "graph size" ) );
	static auto& gsc  = gui::cfg::get< f32 >( HASH_CT( "graph width scale" ) );
	static auto& gsch = gui::cfg::get< f32 >( HASH_CT( "graph height scale" ) );

	if ( !g )
		return get_bottommost_y_scenario( );

	if ( gs == 0 || gsc == 0.F )
		return get_bottommost_y_scenario( );

	auto ss  = g_render.get_screen_size( );
	auto pad = ceil( ss[ 1 ] * .12F );
	return ( ss[ 1 ] / 1.2F + pad ) - ( 250 * gsch );
}

i32 hack::graph::get_bottommost_y_scenario( ) {
	auto ss  = g_render.get_screen_size( );
	auto pad = ceil( ss[ 1 ] * .12F );
	return ( ss[ 1 ] / 1.2F + pad );
}