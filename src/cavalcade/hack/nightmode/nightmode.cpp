#include "nightmode.hpp"
#include "../../ctx/ctx.hpp"
#include "../../gui/cfg/cfg.hpp"

void hack::nightmode::add( sdk::static_prop* prop ) {
	m_static_props.emplace_back( prop, prop->m_diffuse_modulation );
}

void hack::nightmode::clear( ) {
	m_old_factor = std::nullopt;
	m_static_props.clear( );
	m_set   = true;
	m_reset = false;
}

void hack::nightmode::run_props( ) {
	static auto& nm = gui::cfg::get< bool >( HASH_CT( "main:group1:nightmode" ) );
	static auto& ft = gui::cfg::get< i32 >( HASH_CT( "main:group1:factor" ) );

	if ( !nm ) {
		// we've turned off nightmode, so next we should update our props
		m_set = true;

		// have we changed color formerly?
		if ( m_reset ) {
			for ( auto& [ prop, clr ] : m_static_props ) {
				prop->m_diffuse_modulation = clr;
			}

			// already ran... don't run again unless it's needed
			m_reset = false;
		}

		return;
	}

	// we've changed color, therefore when we turn off nightmode we should restore it
	m_reset = true;

	// have we called for an update?
	if ( m_set || !m_old_factor.has_value( ) || ( m_old_factor.has_value( ) && m_old_factor.value( ) != ft ) ) {
		for ( auto& [ prop, clr ] : m_static_props ) {
			prop->m_diffuse_modulation = clr * ( ft * 0.0039215F );
		}

		m_old_factor = ft;
		m_set        = false;
	}
}

void hack::nightmode::run( sdk::material* mat, f32& r, f32& g, f32& b ) {
	if ( !g_csgo.m_engine->is_in_game( ) )
		return;

	static auto& nm = gui::cfg::get< bool >( HASH_CT( "main:group1:nightmode" ) );
	static auto& ft = gui::cfg::get< i32 >( HASH_CT( "main:group1:factor" ) );

	if ( nm ) {
		auto material = HASH_RT( mat->get_texture_group_name( ) );

		switch ( material ) {
		case HASH_CT( "World textures" ):
			r *= ( ft * 0.0039215F );
			g *= ( ft * 0.0039215F );
			b *= ( ft * 0.0039215F );
			break;

		default:
			break;
		}
	}

	run_props( );
}