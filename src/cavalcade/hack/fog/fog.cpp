#include "fog.hpp"
#include "../../ctx/ctx.hpp"
#include "../../gui/cfg/cfg.hpp"
#include "../../entity_cacher/entity_cacher.hpp"

void hack::fog::run( ) {
	static auto& fog = gui::cfg::get< bool >( HASH_CT( "main:group1:fog" ) );
	static auto& fe  = gui::cfg::get< f32 >( HASH_CT( "main:group1:fog end" ) );
	static auto& fd  = gui::cfg::get< f32 >( HASH_CT( "main:group1:fog density" ) );
	for ( i32 i = 1; i <= g_csgo.m_ent_list->get_highest_entity_index( ); ++i ) {
		auto e = g_csgo.m_ent_list->get< sdk::base_entity* >( i );
		if ( !e || !e->get_client_class( ) || e->get_client_class( )->m_class_id != sdk::class_id::FOG_CONTROLLER )
			continue;

		if ( m_update ) {
			m_fog_enabled         = e->fog_enabled( );
			m_fog_start           = e->fog_start( );
			m_fog_end             = e->fog_end( );
			m_fog_max_density     = e->fog_max_density( );
			m_fog_color_primary   = e->fog_color_primary( );
			m_fog_color_secondary = e->fog_color_secondary( );
			m_update              = false;
		}

		if ( fog ) {
			e->fog_enabled( )         = true;
			e->fog_start( )           = 0;
			e->fog_end( )             = fe * 10000.F;
			e->fog_max_density( )     = fd;
			e->fog_color_primary( )   = render::color( 255, 255, 255, 128 );
			e->fog_color_secondary( ) = render::color( 255, 255, 255, 128 );
		} else {
			e->fog_enabled( )         = m_fog_enabled;
			e->fog_start( )           = m_fog_start;
			e->fog_end( )             = m_fog_end;
			e->fog_max_density( )     = m_fog_max_density;
			e->fog_color_primary( )   = m_fog_color_primary;
			e->fog_color_secondary( ) = m_fog_color_secondary;
		}

		break;
	}
}

void hack::fog::reset( ) {
	m_update = true;
}