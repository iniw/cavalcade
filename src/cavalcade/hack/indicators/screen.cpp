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
	if ( !g_csgo.m_engine->is_in_game( ) )
		return;

	if ( g_ctx.m_local.valid( ) && g_ctx.m_local.get( ).is_alive( ) ) {
		auto y = g_hack.m_velgraph.get_upmost_y_scenario( );
		y -= 60;
		static auto fa = &g_render.m_fonts[ render::font::IND_BIG ];
		static auto fb = &g_render.m_fonts[ render::font::IND_SMALL ];

		m_anim_last_vel.bake( m_last_vel.has_value( ) && ( m_time > g_csgo.m_globals->m_curtime ), animation{ 3.F, easing::out_expo },
		                      animation{ 3.F, easing::out_expo } );

		auto text = std::make_shared< render::geometry::text >(
			fa, render::point{ g_render.get_screen_size( )[ 0 ] / 2, y },
			std::to_string( static_cast< int >( g_ctx.m_local.get( ).get_velocity( ).length_2d( ) ) ), render::color( 255, 255, 255, 255 ) );
		text->m_point[ 0 ] -= text->calc_size( )[ 0 ] / 2;
		if ( m_last_vel.has_value( ) && m_anim_last_vel.m_animation_factor > 0.F ) {
			auto size = text->calc_size( );

			auto new_text = std::make_shared< render::geometry::text >( fb, render::point{ g_render.get_screen_size( )[ 0 ] / 2, y },
			                                                            _( "[" ) + std::to_string( m_last_vel.value( ) ) + _( "]" ),
			                                                            render::color( 185, 185, 185, 255 ) );
			new_text->m_point[ 0 ] -= new_text->calc_size( )[ 0 ] / 2;
			new_text->m_point[ 1 ] -= ( size[ 1 ] - 5 ) * m_anim_last_vel.m_animation_factor;
			new_text->m_color = render::color( 185, 185, 185, 255 ).frac_alpha( m_anim_last_vel.m_animation_factor );
			g_render.m_safe.draw_shape_p( std::move( new_text ) );
		}
		g_render.m_safe.draw_shape_p( std::move( text ) );
	}
}

void hack::indicators::screen::clear( ) {
	m_previously_on_ground = false;
	m_time                 = -9999.F;
	m_last_vel             = std::nullopt;
}