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
	aa.x             = static_cast< int >( left );
	aa.y             = static_cast< int >( top );
	bb.x             = static_cast< int >( right - left );
	bb.y             = static_cast< int >( bottom - top );

	return true;
}

void hack::esp::run( ) {
	static auto& boxb  = gui::cfg::get< bool >( HASH_CT( "main:group1:box bob" ) );
	static auto& box_w = gui::cfg::get< i32 >( HASH_CT( "main:group1:bw" ) );
	static auto& box_h = gui::cfg::get< i32 >( HASH_CT( "main:group1:bh" ) );

	// LMFAOOOOOOOOOOOOOOOOO I WANNA GO TO SLEEP
	// g_entity_cacher.for_each( [ & ]( auto& p ) {
	// 	if ( !p )
	// 		return;

	// 	if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || !p.get( ).is_enemy( g_ctx.m_local ) )
	// 		return;

	// 	animator& anim = m_alpha[ p.get( ).get_networkable_index( ) ];
	// 	anim.bake( !p.get( ).is_dormant( ), animation{ 3.F, easing::out_expo }, animation{ 3.F, easing::out_expo } );

	// 	static auto f = &g_render.m_fonts[ render::font::ESP ];

	// 	if ( anim.m_animation_factor > 0.F ) {
	// 		sdk::auxiliary::dlight* light = g_csgo.m_effects->alloc_dlight( p.get( ).get_networkable_index( ) );
	// 		light->m_die                  = g_csgo.m_globals->m_curtime + .05F;
	// 		light->m_radius               = 200.F;
	// 		light->m_color                = render::color( 255, 255, 255, /* exp */ 5 );
	// 		light->m_key                  = p.get( ).get_networkable_index( );
	// 		light->m_decay                = light->m_radius / 5.F;
	// 		light->m_origin               = p.get( ).get_abs_origin( ) + math::v3f( 0, 0, 2 );

	// 		std::pair< render::point, render::point > bbox;
	// 		auto box = bounding_box( p, bbox );
	// 		if ( box ) {
	// 			auto& [ aa, bb ] = bbox;

	// 			if ( boxb )
	// 				aa.y += 16 - ( 16 * anim.m_animation_factor );

	// 			auto clr = render::color( 0xffffffff ).frac_alpha( anim.m_animation_factor );
	// 			if ( box_w > 0 || box_h > 0 ) {
	// 				auto cw = .5F * ( box_w * .01F );
	// 				auto ch = .5F * ( box_h * .01F );
	// 				auto ow = box_w >= 99 ? 1 : 0;
	// 				auto oh = box_h >= 99 ? 1 : 0;

	// 				if ( ow && oh ) {
	// 					g_render.m_safe.draw_shape< render::geometry::rect >(
	// 						aa, aa + bb, render::color( 0xffffffff ).frac_alpha( anim.m_animation_factor ), 1.F );
	// 				} else {
	// 					// NOTE(para): this is absolutely disgusting and probably a hog. the only alternative though is to batch a set of vertices and
	// 					// pass them

	// 					// @iniw do you think we could reduce this to a polyline at least?

	// 					// NOTE(wini): this is causing compiler errors and its 4am and i CANNOT BE ARSED

	// 					// // top left
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( aa, render::point{ static_cast< i32 >( ow + aa.x + bb.x * cw ), aa.y
	// 					// },
	// 					//                                                       clr );

	// 					// // top right
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa.x + bb.x - 1, aa.y },
	// 					//                                                       render::point{ aa.x + bb.x * ( 1.F - cw ) - 1, aa.y }, clr );

	// 					// // bottom left
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa.x, aa.y + bb.y - 1 },
	// 					//                                                       render::point{ ow + aa.x + bb.x * cw, aa.y + bb.y - 1 }, clr );

	// 					// // bottom right
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa.x + bb.x - 1, aa.y + bb.y - 1 },
	// 					//                                                       render::point{ aa.x + bb.x * ( 1.F - cw ) - 1, aa.y + bb.y - 1 }, clr
	// 					//                                                       );

	// 					// // left top
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( aa, render::point{ aa.x, oh + aa.y + bb.y * ch }, clr );

	// 					// // left bottom
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa.x, aa.y + bb.y - 1 },
	// 					//                                                       render::point{ aa.x, aa.y + bb.y * ( 1.F - ch ) }, clr );

	// 					// // right top
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa.x + bb.x - 1, aa.y },
	// 					//                                                       render::point{ aa.x + bb.x - 1, oh + aa.y + bb.y * ch }, clr );

	// 					// // right bottom
	// 					// g_render.m_safe.draw_shape< render::geometry::line >( render::point{ aa.x + bb.x - 1, aa.y + bb.y - 1 },
	// 					//                                                       render::point{ aa.x + bb.x - 1, aa.y + bb.y * ( 1.F - ch ) }, clr );
	// 				}
	// 			}

	// 			auto name   = std::string( p.get_name( ) );
	// 			auto offset = /* HACK */ name.find( XOR( "ygjpq" ) ) != std::string::npos ? 4 : 0;
	// 			auto text   = std::make_shared< render::geometry::text >( f, aa, std::move( name ), clr );
	// 			auto ts     = text->calc_size( );
	// 			text->m_point.x += bb.x / 2;
	// 			text->m_point.x -= ts.w / 2;
	// 			text->m_point.y -= ts.h + 3 + offset;
	// 			g_render.m_safe.draw_shape_p( std::move( text ) );

	// 			constexpr auto max_health = 100;
	// 			auto health               = std::min( max_health, p.get( ).get_health( ) );
	// 			auto height               = health * bb.y / max_health;

	// 			{
	// 				auto health_aa = render::point{ aa.x - 7, aa.y + bb.y - height + 1 };
	// 				g_render.m_safe.draw_shape< render::geometry::rect_filled >( health_aa, health_aa + render::point{ 3, height - 2 },
	// 				                                                             render::color( 0x00000044 ).frac_alpha( anim.m_animation_factor ) );
	// 				g_render.m_safe.draw_shape< render::geometry::rect_filled >( health_aa, health_aa + render::point{ 3, height - 2 },
	// 				                                                             render::color( 0xffff00ff ).mod_alpha( clr.m_a ) );
	// 			}
	// 			{
	// 				auto health_aa = render::point{ aa.x - 8, aa.y };
	// 				g_render.m_safe.draw_shape< render::geometry::rect >( health_aa, health_aa + render::point{ 5, bb.y }, clr, 1.F );
	// 			}
	// 		}
	// 		{
	// 			auto angles = g_csgo.m_engine->get_view_angles( );
	// 			auto ss     = g_render.get_screen_size( );

	// 			math::v3f screen;
	// 			g_csgo.m_debug_overlay->screen_position( p.get( ).get_origin( ), screen );

	// 			if ( screen[ 0 ] < 0 || screen[ 1 ] < 0 || screen[ 0 ] > ss.w || screen[ 1 ] > ss.h ) {
	// 				auto eye      = g_ctx.m_local.get( ).get_eye_position( );
	// 				auto rotation = angles.yaw - eye.calculate_angle( p.get( ).get_origin( ) )[ 1 ] - 90;

	// 				constexpr auto DEG2RAD     = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };
	// 				const auto rotate_triangle = [ & ]( std::vector< render::point >& points, f32 rotation ) {
	// 					constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };
	// 					render::point points_center{ };

	// 					for ( const auto& e : points )
	// 						points_center += e;

	// 					points_center /= points.size( );

	// 					for ( auto& point : points ) {
	// 						point = point - points_center;

	// 						const auto temp_x = point[ 0 ];
	// 						const auto temp_y = point[ 1 ];

	// 						const auto theta = DEG2RAD( rotation );
	// 						const auto c     = std::cos( theta );
	// 						const auto s     = std::sin( theta );

	// 						point[ 0 ] = temp_x * c - temp_y * s;
	// 						point[ 1 ] = temp_x * s + temp_y * c;

	// 						point = point + points_center;
	// 					}
	// 				};

	// 				auto distance = 100;
	// 				auto size     = 15;

	// 				auto angle_yaw         = angles.yaw - eye.calculate_angle( p.get( ).get_origin( ) )[ 1 ] - 90;
	// 				auto angle_yaw_rad     = DEG2RAD( angle_yaw );
	// 				const auto new_point_x = ss.w / 2 + distance * cosf( angle_yaw_rad );
	// 				const auto new_point_y = ss.h / 2 + distance * sinf( angle_yaw_rad );

	// 				std::vector< render::point > points{ { new_point_x - size, new_point_y - size },
	// 					                                 { new_point_x + size, new_point_y },
	// 					                                 { new_point_x - size, new_point_y + size } };
	// 				rotate_triangle( points, angle_yaw );

	// 				g_render.m_safe.draw_shape< render::geometry::polyfill >(
	// 					std::move( points ), render::color( 255, 255, 255, 255 ).frac_alpha( anim.m_animation_factor ) );
	// 			}
	// 		}
	// 	}
	// } );
}

void hack::esp::clear( ) {
	m_alpha.clear( );
}