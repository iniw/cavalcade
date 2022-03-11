#include "esp.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"
#include "../../gui/cfg/cfg.hpp"
#include "../hack.hpp"

static constexpr const char* get_weapon_icon( sdk::def_idx weapon_id ) {
	switch ( weapon_id ) {
	case sdk::def_idx::KNIFE_BAYONET:
		return XOR( "1" );
	case sdk::def_idx::KNIFE_CT:
		return XOR( "[" );
	case sdk::def_idx::KNIFE_T:
		return XOR( "[" );
	case sdk::def_idx::KNIFE_SURVIVAL_BOWIE:
		return XOR( "7" );
	case sdk::def_idx::KNIFE_BUTTERFLY:
		return XOR( "8" );
	case sdk::def_idx::KNIFE_FALCHION:
		return XOR( "0" );
	case sdk::def_idx::KNIFE_FLIP:
		return XOR( "2" );
	case sdk::def_idx::KNIFE_GUT:
		return XOR( "3" );
	case sdk::def_idx::KNIFE_KARAMBIT:
		return XOR( "4" );
	case sdk::def_idx::KNIFE_M9_BAYONET:
		return XOR( "5" );
	case sdk::def_idx::KNIFE_TACTICAL:
		return XOR( "6" );
	case sdk::def_idx::KNIFE_PUSH:
		return XOR( "]" );
	case sdk::def_idx::DEAGLE:
		return XOR( "A" );
	case sdk::def_idx::ELITE:
		return XOR( "B" );
	case sdk::def_idx::FIVESEVEN:
		return XOR( "C" );
	case sdk::def_idx::GLOCK:
		return XOR( "D" );
	case sdk::def_idx::HKP2000:
		return XOR( "E" );
	case sdk::def_idx::P250:
		return XOR( "F" );
	case sdk::def_idx::USP_SILENCER:
		return XOR( "G" );
	case sdk::def_idx::TEC9:
		return XOR( "H" );
	case sdk::def_idx::REVOLVER:
		return XOR( "A" );
	case sdk::def_idx::MAC10:
		return XOR( "K" );
	case sdk::def_idx::UMP45:
		return XOR( "L" );
	case sdk::def_idx::BIZON:
		return XOR( "M" );
	case sdk::def_idx::MP7:
		return XOR( "N" );
	case sdk::def_idx::MP9:
		return XOR( "O" );
	case sdk::def_idx::P90:
		return XOR( "P" );
	case sdk::def_idx::GALILAR:
		return XOR( "Q" );
	case sdk::def_idx::FAMAS:
		return XOR( "R" );
	case sdk::def_idx::M4A1_SILENCER:
		return XOR( "S" );
	case sdk::def_idx::M4A1:
		return XOR( "T" );
	case sdk::def_idx::AUG:
		return XOR( "U" );
	case sdk::def_idx::SG556:
		return XOR( "V" );
	case sdk::def_idx::AK47:
		return XOR( "W" );
	case sdk::def_idx::G3SG1:
		return XOR( "X" );
	case sdk::def_idx::SCAR20:
		return XOR( "Y" );
	case sdk::def_idx::AWP:
		return XOR( "Z" );
	case sdk::def_idx::SSG08:
		return XOR( "a" );
	case sdk::def_idx::XM1014:
		return XOR( "b" );
	case sdk::def_idx::SAWEDOFF:
		return XOR( "c" );
	case sdk::def_idx::MAG7:
		return XOR( "d" );
	case sdk::def_idx::NOVA:
		return XOR( "e" );
	case sdk::def_idx::NEGEV:
		return XOR( "f" );
	case sdk::def_idx::M249:
		return XOR( "g" );
	case sdk::def_idx::TASER:
		return XOR( "h" );
	case sdk::def_idx::FLASHBANG:
		return XOR( "i" );
	case sdk::def_idx::HEGRENADE:
		return XOR( "j" );
	case sdk::def_idx::SMOKEGRENADE:
		return XOR( "k" );
	case sdk::def_idx::MOLOTOV:
		return XOR( "l" );
	case sdk::def_idx::DECOY:
		return XOR( "m" );
	case sdk::def_idx::INCGRENADE:
		return XOR( "n" );
	case sdk::def_idx::C4:
		return XOR( "o" );
	case sdk::def_idx::CZ75A:
		return XOR( "I" );
	default:
		return XOR( " " );
	}
}

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
	static auto& boxb    = gui::cfg::get< bool >( HASH_CT( "main:group1:box bob" ) );
	static auto& box_w   = gui::cfg::get< i32 >( HASH_CT( "main:group1:bw" ) );
	static auto& box_h   = gui::cfg::get< i32 >( HASH_CT( "main:group1:bh" ) );
	static auto& ht      = gui::cfg::get< bool >( HASH_CT( "main:group1:health" ) );
	static auto& nm      = gui::cfg::get< bool >( HASH_CT( "main:group1:name" ) );
	static auto& wp      = gui::cfg::get< bool >( HASH_CT( "main:group1:weapon" ) );
	static auto& dlights = gui::cfg::get< bool >( HASH_CT( "main:group1:dlights" ) );

	g_entity_cacher.for_each( [ & ]( auto& p ) {
		if ( !p )
			return;

		if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || !p.get( ).is_enemy( g_ctx.m_local ) )
			return;

		animator& anim = m_alpha[ p.get( ).get_networkable_index( ) ];
		anim.bake( !p.get( ).is_dormant( ), animation{ 3.F, easing::out_expo }, animation{ 3.F, easing::out_expo } );

		static auto f  = &g_render.m_fonts[ render::font::ESP ];
		static auto fi = &g_render.m_fonts[ render::font::ESP_ICON ];

		if ( !p.get( ).is_dormant( ) ) {
			if ( dlights ) {
				sdk::auxiliary::dlight* light = g_csgo.m_effects->alloc_dlight( p.get( ).get_networkable_index( ) );
				light->m_die                  = g_csgo.m_globals->m_curtime + .05F;
				light->m_radius               = 50.F;
				light->m_color                = render::color( 255, 255, 255, /* exp */ 5 );
				light->m_key                  = p.get( ).get_networkable_index( );
				light->m_decay                = light->m_radius / 5.F;
				light->m_origin               = p.get( ).get_abs_origin( ) + math::v3f( 0, 0, 2 );
			}
		}

		if ( anim.m_animation_factor > 0.F ) {
			std::pair< render::point, render::point > bbox;
			auto box = bounding_box( p, bbox );
			if ( box ) {
				auto& [ aa, bb ] = bbox;

				if ( boxb )
					aa[ 1 ] += 16 - ( 16 * anim.m_animation_factor );

				auto clr = render::color( 0xffffffff ).frac_alpha( anim.m_animation_factor );
				if ( box_w > 0 || box_h > 0 ) {
					auto cw = .5F * ( box_w * .01F );
					auto ch = .5F * ( box_h * .01F );
					auto ow = box_w >= 99 ? 1 : 0;
					auto oh = box_h >= 99 ? 1 : 0;

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
				}

				if ( nm ) {
					auto name   = std::string( p.get_name( ) );
					auto offset = /* HACK */ name.find( XOR( "ygjpq" ) ) != std::string::npos ? 6 : 0;
					auto text   = std::make_shared< render::geometry::text >( f, aa, std::move( name ), clr );
					auto ts     = text->calc_size( );
					text->m_point[ 0 ] += bb[ 0 ] / 2;
					text->m_point[ 0 ] -= ts[ 0 ] / 2;
					text->m_point[ 1 ] -= ts[ 1 ] + 3 + offset;
					g_render.m_safe.draw_shape_p( std::move( text ) );
				}

				constexpr auto max_health = 100;
				auto health               = std::min( max_health, p.get( ).get_health( ) );
				auto height               = health * bb[ 1 ] / max_health;

				if ( ht ) {
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
				{
					if ( g_hack.m_backtrack.m_records.contains( p.get( ).get_networkable_index( ) ) ) {
						const auto& recs = g_hack.m_backtrack.m_records[ p.get( ).get_networkable_index( ) ];
						for ( const auto& rec : recs ) {
							auto hitbox_pos = p.get( ).get_hitbox_position( sdk::e_hitbox::HEAD, rec.m_matrix );
							math::v3f out{ };
							if ( !g_csgo.m_debug_overlay->screen_position( hitbox_pos, out ) ) {
								render::point aa{ out[ 0 ] - 2, out[ 1 ] - 2 };
								g_render.m_safe.draw_shape< render::geometry::rect_filled >( aa, aa + 4,
								                                                             render::color( 0xffffffff ).mod_alpha( clr.m_a ) );
							}
						}
					}
				}
				// bottom row
				int padding = 2;
				if ( wp ) {
					auto weapon = p.get( ).get_active_weapon( ).get< sdk::weapon_cs_base* >( );
					if ( weapon ) {
						auto shape =
							std::make_shared< render::geometry::text >( fi, aa + render::point{ bb[ 0 ] / 2, bb[ 1 ] + padding + 2 },
						                                                std::string( get_weapon_icon( weapon->get_item_definition_index( ) ) ),
						                                                render::color( 0xffffffff ).mod_alpha( clr.m_a ) );
						shape->m_point[ 0 ] -= shape->calc_size( )[ 0 ] / 2;
						g_render.m_safe.draw_shape_p( std::move( shape ) );
					}
				}
			}
			{
				auto angles = g_csgo.m_engine->get_view_angles( );
				auto ss     = g_render.get_screen_size( );

				math::v3f screen;
				g_csgo.m_debug_overlay->screen_position( p.get( ).get_origin( ), screen );

				if ( screen[ 0 ] < 0 || screen[ 1 ] < 0 || screen[ 0 ] > ss[ 0 ] || screen[ 1 ] > ss[ 1 ] ) {
					auto eye      = g_ctx.m_local.get( ).get_eye_position( );
					auto rotation = angles.yaw - eye.calculate_angle( p.get( ).get_origin( ) )[ 1 ] - 90;

					constexpr auto DEG2RAD     = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };
					const auto rotate_triangle = [ & ]( std::vector< render::point >& points, f32 rotation ) {
						constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };
						render::point points_center{ };

						for ( const auto& e : points )
							points_center += e;

						points_center /= points.size( );

						for ( auto& point : points ) {
							point = point - points_center;

							const auto temp_x = point[ 0 ];
							const auto temp_y = point[ 1 ];

							const auto theta = DEG2RAD( rotation );
							const auto c     = std::cos( theta );
							const auto s     = std::sin( theta );

							point[ 0 ] = temp_x * c - temp_y * s;
							point[ 1 ] = temp_x * s + temp_y * c;

							point = point + points_center;
						}
					};

					auto distance = 100;
					auto size     = 15;

					auto angle_yaw         = angles.yaw - eye.calculate_angle( p.get( ).get_origin( ) )[ 1 ] - 90;
					auto angle_yaw_rad     = DEG2RAD( angle_yaw );
					const auto new_point_x = ss[ 0 ] / 2 + distance * cosf( angle_yaw_rad );
					const auto new_point_y = ss[ 1 ] / 2 + distance * sinf( angle_yaw_rad );

					std::vector< render::point > points{ { new_point_x - size, new_point_y - size },
						                                 { new_point_x + size, new_point_y },
						                                 { new_point_x - size, new_point_y + size } };
					rotate_triangle( points, angle_yaw );

					g_render.m_safe.draw_shape< render::geometry::polyfill >(
						std::move( points ), render::color( 255, 255, 255, 255 ).frac_alpha( anim.m_animation_factor ) );
				}
			}
		}
	} );
}

void hack::esp::clear( ) {
	m_alpha.clear( );
}