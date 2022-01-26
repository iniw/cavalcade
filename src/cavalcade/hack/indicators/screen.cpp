#include "screen.hpp"
#include "../../ctx/ctx.hpp"
#include "../hack.hpp"

void hack::indicators::screen::gather( ) {
	if ( m_previously_on_ground && !( g_ctx.m_local.get( ).get_flags( ) & sdk::flags::ONGROUND ) ) {
		m_time     = g_csgo.m_globals->m_curtime + 1.F;
		m_last_vel = static_cast< i32 >( g_ctx.m_local.get( ).get_velocity( ).length_2d( ) );
	}

	m_previously_on_ground = g_ctx.m_local.get( ).get_flags( ) & sdk::flags::ONGROUND;
}

void hack::indicators::screen::draw( ) {
	auto y = g_hack.m_velgraph.get_upmost_y_scenario( );
	y -= 60;
	static auto fa = &g_render.m_fonts[ render::font::IND_BIG ];
	static auto fb = &g_render.m_fonts[ render::font::IND_SMALL ];
	auto ss        = g_render.get_screen_size( );

	m_anim_last_vel.bake( m_last_vel.has_value( ) && ( m_time > g_csgo.m_globals->m_curtime ), animation{ 3.F, easing::out_expo },
	                      animation{ 3.F, easing::out_expo } );

	auto text = std::make_shared< render::geometry::text >( fa, render::point{ ss[ 0 ] / 2, y },
	                                                        std::to_string( static_cast< int >( g_ctx.m_local.get( ).get_velocity( ).length_2d( ) ) ),
	                                                        render::color( 255, 255, 255, 255 ) );
	text->m_point[ 0 ] -= text->calc_size( )[ 0 ] / 2;
	i32 container_pad = 0;
	if ( m_last_vel.has_value( ) && m_anim_last_vel.m_animation_factor > 0.F ) {
		auto size = text->calc_size( );

		auto new_text = std::make_shared< render::geometry::text >( fb, render::point{ ss[ 0 ] / 2, y },
		                                                            XOR( "[" ) + std::to_string( m_last_vel.value( ) ) + XOR( "]" ),
		                                                            render::color( 185, 185, 185, 255 ) );
		new_text->m_point[ 0 ] -= new_text->calc_size( )[ 0 ] / 2;
		container_pad = ( size[ 1 ] - 5 ) * m_anim_last_vel.m_animation_factor;
		new_text->m_point[ 1 ] -= container_pad + 2;
		new_text->m_color = render::color( 185, 185, 185, 255 ).frac_alpha( m_anim_last_vel.m_animation_factor );
		g_render.m_safe.draw_shape_p( std::move( new_text ) );
	}

	struct flag {
		flag( bool inc, std::string&& text ) : m_inc( inc ), m_text( std::move( text ) ) { }

		bool m_inc{ false };
		std::string m_text;
		i32 m_pad{ 0 };
	};

	struct container {
		void push_back( flag&& f ) {
			auto size = m_flags.size( );
			if ( m_string.empty( ) ) {
				m_string += f.m_text;
				f.m_pad = 0;
			} else {
				f.m_pad = fa->calc_size( " " + m_string )[ 0 ];
				m_string += " " + f.m_text;
			}

			m_flags.push_back( std::move( f ) );
		}

		std::vector< flag > m_flags;
		std::string m_string{ };
	};

	container flags;
	if ( g_io.key_state< io::key_state::DOWN >( VK_XBUTTON1 ) )
		flags.push_back( flag( g_hack.m_movement.m_jumpbugged, "JB" ) );
	if ( g_io.key_state< io::key_state::DOWN >( VK_XBUTTON2 ) )
		flags.push_back( flag( g_hack.m_movement.m_longjumped, "LJ" ) );
	if ( g_io.key_state< io::key_state::DOWN >( VK_MBUTTON ) )
		flags.push_back( flag( g_hack.m_movement.m_edgebug.m_predicted, "EB" ) );

	auto container_text = std::make_shared< render::geometry::text >( fa, render::point{ ss[ 0 ] / 2, text->m_point[ 1 ] - container_pad },
	                                                                  flags.m_string, render::color( 255, 255, 255, 255 ) );

	auto css = container_text->calc_size( );
	container_text->m_point[ 0 ] -= css[ 0 ] / 2;
	container_text->m_point[ 1 ] -= css[ 1 ] + 2;

	auto old_x = container_text->m_point[ 0 ];
	g_render.m_safe.draw_shape_p( std::move( container_text ) );
	for ( auto& e : flags.m_flags ) {
		if ( e.m_inc ) {
			g_render.m_safe.draw_shape< render::geometry::text >( fa, render::point{ old_x + e.m_pad, container_text->m_point[ 1 ] }, e.m_text,
			                                                      render::color{ 255, 255, 0, 255 } );
		}
	}

	g_render.m_safe.draw_shape_p( std::move( text ) );
}

void hack::indicators::screen::clear( ) {
	m_previously_on_ground = false;
	m_time                 = -9999.F;
	m_last_vel             = std::nullopt;
}