#include "esp.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"

static bool bounding_box( sdk::player& p, std::pair< render::point, render::point >& out ) {
	math::v3f mins{ }, maxs{ };
	if ( !p.get( ).get_collideable( )->world_space_surrounding_bounds( &mins, &maxs ) )
		return false;

	math::v3f world[ 8 ] = { { mins[ 0 ], mins[ 1 ], mins[ 2 ] }, { mins[ 0 ], maxs[ 1 ], mins[ 2 ] }, { maxs[ 0 ], maxs[ 1 ], mins[ 2 ] },
		                     { maxs[ 0 ], mins[ 1 ], mins[ 2 ] }, { maxs[ 0 ], maxs[ 1 ], maxs[ 2 ] }, { mins[ 0 ], maxs[ 1 ], maxs[ 2 ] },
		                     { mins[ 0 ], mins[ 1 ], maxs[ 2 ] }, { maxs[ 0 ], mins[ 1 ], maxs[ 2 ] } };

	math::v3f screen[ 8 ]{ };

	for ( auto i = 0; i < 8; ++i ) {
		if ( g_csgo.m_debug_overlay->screen_position( world[ i ], screen[ i ] ) )
			return false;
	}

	auto left   = screen[ 3 ][ 0 ];
	auto right  = screen[ 3 ][ 0 ];
	auto top    = screen[ 3 ][ 1 ];
	auto bottom = screen[ 3 ][ 1 ];

	for ( auto i = 0; i < 8; ++i ) {
		if ( left > screen[ i ][ 0 ] )
			left = screen[ i ][ 0 ];

		if ( right < screen[ i ][ 0 ] )
			right = screen[ i ][ 0 ];

		if ( bottom < screen[ i ][ 1 ] )
			bottom = screen[ i ][ 1 ];

		if ( top > screen[ i ][ 1 ] )
			top = screen[ i ][ 1 ];
	}

	auto& [ aa, bb ] = out;
	aa[ 0 ]          = static_cast< int >( left );
	aa[ 1 ]          = static_cast< int >( top );
	bb[ 0 ]          = static_cast< int >( right - left );
	bb[ 1 ]          = static_cast< int >( bottom - top );

	return true;
}

void hack::esp::run( ) {
	if ( !g_ctx.m_local )
		return;

	if ( !g_csgo.m_engine->is_in_game( ) )
		return;

	g_entity_cacher.for_each( [ & ]( auto& p ) {
		if ( !p )
			return;

		if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || !p.get( ).is_enemy( g_ctx.m_local ) )
			return;

		animator& anim = m_alpha[ p.get( ).get_networkable_index( ) ];
		anim.bake( !p.get( ).is_dormant( ), animation{ 3.F, easing::out_quart }, animation{ 3.F, easing::out_quart } );

		if ( anim.m_animation_factor > 0.F ) {
			std::pair< render::point, render::point > bbox;
			auto box = bounding_box( p, bbox );
			if ( box ) {
				const auto& [ aa, bb ] = bbox;
				auto clr               = render::color( 0xffffffff ).frac_alpha( anim.m_animation_factor );
				g_render.m_safe.draw_shape< render::geometry::rect >( aa, aa + bb, clr.to_imgui( ), 1.F );
			}
		}
	} );
}

void hack::esp::clear( ) {
	m_alpha.clear( );
}