#include "hitmarker.hpp"
#include "../../ctx/ctx.hpp"
#include "../hack.hpp"

void hack::hitmarker::push( hitmark&& hm ) {
	m_hits.push_back( std::move( hm ) );
}

void hack::hitmarker::draw( ) {
	if ( !g_ctx.m_local || !g_csgo.m_engine->is_in_game( ) )
		return;

	if ( m_hits.empty( ) )
		return;

	constexpr auto cross = [ & ]( i32 x, i32 y, i32 length, render::color clr ) {
		i32 padding = 8;
		g_render.m_safe.draw_shape< render::geometry::line >( render::point{ x - length - padding, y - length - padding },
		                                                      render::point{ x - length / 4 - padding, y - length / 4 - padding }, clr, 1.F );

		g_render.m_safe.draw_shape< render::geometry::line >( render::point{ x - length - padding, y + length + padding },
		                                                      render::point{ x - length / 4 - padding, y + length / 4 + padding }, clr, 1.F );

		g_render.m_safe.draw_shape< render::geometry::line >( render::point{ x + length + padding, y + length + padding },
		                                                      render::point{ x + length / 4 + padding, y + length / 4 + padding }, clr, 1.F );

		g_render.m_safe.draw_shape< render::geometry::line >( render::point{ x + length + padding, y - length - padding },
		                                                      render::point{ x + length / 4 + padding, y - length / 4 - padding }, clr, 1.F );
	};

	for ( auto i = 0; i < m_hits.size( ); ++i ) {
		auto& e = m_hits[ i ];

		auto expired = g_csgo.m_globals->m_curtime >= ( e.m_time + 1.F );
		if ( expired ) {
			if ( ( --e.m_alpha ) <= 0 ) {
				m_hits.erase( m_hits.begin( ) + i );
				continue;
			}
		}

		math::v3f out{ };
		if ( g_csgo.m_debug_overlay->screen_position( e.m_pos, out ) )
			continue;

		auto x = static_cast< i32 >( out[ 0 ] );
		auto y = static_cast< i32 >( out[ 1 ] );

		cross( x, y, 4, render::color( 255, 255, 255, e.m_alpha ) );
	}

	if ( m_hits.empty( ) )
		return;

	auto& last = m_hits.back( );
	auto ss    = g_render.get_screen_size( );
	cross( ss.w / 2, ss.h / 2, 4, render::color( 255, 255, 255, last.m_alpha ) );
}

void hack::hitmarker::clear( ) {
	m_hits.clear( );
}