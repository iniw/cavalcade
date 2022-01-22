#include "esp.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"
#include "../../gui/cfg/cfg.hpp"

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
	if ( !g_csgo.m_engine->is_in_game( ) )
		return;

	if ( !g_ctx.m_local )
		return;

	static auto& box_w = gui::cfg::get< i32 >( HASH_CT( "main:group1:bw" ) );
	static auto& box_h = gui::cfg::get< i32 >( HASH_CT( "main:group1:bh" ) );

	g_entity_cacher.for_each( [ & ]( auto& p ) {
		if ( !p.valid( ) )
			return;

		if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || !p.get( ).is_enemy( g_ctx.m_local ) )
			return;

		animator& anim = m_alpha[ p.get( ).get_networkable_index( ) ];
		anim.bake( !p.get( ).is_dormant( ), animation{ 3.F, easing::out_quart }, animation{ 3.F, easing::out_quart } );

		static auto f = &g_render.m_fonts[ render::font::ESP ];

		if ( anim.m_animation_factor > 0.F ) {
			std::pair< render::point, render::point > bbox;
			auto box = bounding_box( p, bbox );
			if ( box ) {
				const auto& [ aa, bb ] = bbox;

				if ( box_w > 0 || box_h > 0 ) {
					auto cw = .5F * ( box_w * .01F );
					auto ch = .5F * ( box_h * .01F );
					auto ow = box_w >= 99 ? 1 : 0;
					auto oh = box_h >= 99 ? 1 : 0;

					auto clr = render::color( 0xffffffff ).frac_alpha( anim.m_animation_factor );

					if ( ow && oh ) {
						g_render.m_safe.draw_shape< render::geometry::rect >(
							aa, aa + bb, render::color( 0xffffffff ).frac_alpha( anim.m_animation_factor ), 1.F );
					} else {
						// NOTE(para): this is absolutely disgusting and probably a hog. the only alternative though is to batch a set of vertices and
						// pass them

						// @iniw do you think we could reduce this to a polyline at least?

						// top left
						g_render.m_safe.draw_shape< render::geometry::line >( aa, render::point{ ow + aa[ 0 ] + bb[ 0 ] * cw, aa[ 1 ] }, clr );

						// top right
						g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa[ 0 ] + bb[ 0 ] - 1, aa[ 1 ] },
						                                                      render::point{ aa[ 0 ] + bb[ 0 ] * ( 1.F - cw ) - 1, aa[ 1 ] }, clr );

						// bottom left
						g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa[ 0 ], aa[ 1 ] + bb[ 1 ] - 1 },
						                                                      render::point{ ow + aa[ 0 ] + bb[ 0 ] * cw, aa[ 1 ] + bb[ 1 ] - 1 },
						                                                      clr );

						// bottom right
						g_render.m_safe.draw_shape< render::geometry::line >(
							render::point{ aa[ 0 ] + bb[ 0 ] - 1, aa[ 1 ] + bb[ 1 ] - 1 },
							render::point{ aa[ 0 ] + bb[ 0 ] * ( 1.F - cw ) - 1, aa[ 1 ] + bb[ 1 ] - 1 }, clr );

						// left top
						g_render.m_safe.draw_shape< render::geometry::line >( aa, render::point{ aa[ 0 ], oh + aa[ 1 ] + bb[ 1 ] * ch }, clr );

						// left bottom
						g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa[ 0 ], aa[ 1 ] + bb[ 1 ] - 1 },
						                                                      render::point{ aa[ 0 ], aa[ 1 ] + bb[ 1 ] * ( 1.F - ch ) }, clr );

						// right top
						g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa[ 0 ] + bb[ 0 ] - 1, aa[ 1 ] },
						                                                      render::point{ aa[ 0 ] + bb[ 0 ] - 1, oh + aa[ 1 ] + bb[ 1 ] * ch },
						                                                      clr );

						// right bottom
						g_render.m_safe.draw_shape< render::geometry::line >(
							render::point{ aa[ 0 ] + bb[ 0 ] - 1, aa[ 1 ] + bb[ 1 ] - 1 },
							render::point{ aa[ 0 ] + bb[ 0 ] - 1, aa[ 1 ] + bb[ 1 ] * ( 1.F - ch ) }, clr );
					}

					auto name   = std::string( p.get_name( ) );
					auto offset = /* HACK */ name.find( _( "ygjpq" ) ) != std::string::npos ? 4 : 0;
					auto text   = std::make_shared< render::geometry::text >( f, aa, std::move( name ), clr );
					text->m_point[ 1 ] -= text->calc_size( )[ 1 ] + 3 + offset;
					g_render.m_safe.draw_shape_p( std::move( text ) );

					constexpr auto max_health = 100;
					auto health               = std::min( max_health, p.get( ).get_health( ) );
					auto height               = health * bb[ 1 ] / max_health;

					{
						auto health_aa = render::point{ aa[ 0 ] - 7, aa[ 1 ] + bb[ 1 ] - height + 1 };
						g_render.m_safe.draw_shape< render::geometry::rect_filled >(
							health_aa, health_aa + render::point{ 3, height - 2 },
							render::color( 0x00000044 ).frac_alpha( anim.m_animation_factor ) );
						g_render.m_safe.draw_shape< render::geometry::rect_filled >( health_aa, health_aa + render::point{ 3, height - 2 },
						                                                             render::color( 0xffff00ff ).mod_alpha( clr.m_a ) );
					}
					{
						auto health_aa = render::point{ aa[ 0 ] - 8, aa[ 1 ] };
						g_render.m_safe.draw_shape< render::geometry::rect >( health_aa, health_aa + render::point{ 5, bb[ 1 ] }, clr, 1.F );
					}
				}
			}
		}
	} );
}

void hack::esp::clear( ) {
	m_alpha.clear( );
}