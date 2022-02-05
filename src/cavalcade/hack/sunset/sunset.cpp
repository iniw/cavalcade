#include "sunset.hpp"
#include "../../ctx/ctx.hpp"
#include "../../gui/cfg/cfg.hpp"
#include "../../entity_cacher/entity_cacher.hpp"

void hack::sunset::run( ) {
	static auto& ss = gui::cfg::get< bool >( HASH_CT( "main:group1:sunset" ) );
	for ( i32 i = 1; i <= g_csgo.m_ent_list->get_highest_entity_index( ); ++i ) {
		auto e = g_csgo.m_ent_list->get< sdk::base_entity* >( i );
		if ( !e || !e->get_client_class( ) || e->get_client_class( )->m_class_id != sdk::class_id::CASCADE_LIGHT )
			continue;

		if ( m_update ) {
			m_max_shadow_dist        = e->max_shadow_dist( );
			m_light_shadow_direction = e->light_shadow_direction( );
			m_update                 = false;
		}

		if ( ss ) {
			e->max_shadow_dist( )        = 800.F;
			e->light_shadow_direction( ) = math::v3f( 0.F, 0.F, 0.F );
		} else {
			e->max_shadow_dist( )        = m_max_shadow_dist;
			e->light_shadow_direction( ) = m_light_shadow_direction;
		}
	}
}

void hack::sunset::reset( ) {
	m_update = true;
}