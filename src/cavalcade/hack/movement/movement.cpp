#include "../hack.hpp"
#include "movement.hpp"
#include "../../ctx/ctx.hpp"
#include "../../gui/cfg/cfg.hpp"

static bool local_on_ladder_or_noclip( ) {
	return g_ctx.m_local &&
	       ( g_ctx.m_local.get( ).get_move_type( ) == sdk::move_type::LADDER || g_ctx.m_local.get( ).get_move_type( ) == sdk::move_type::NOCLIP );
}

void hack::movement::backup::store( ) {
	m_old_flags             = g_ctx.m_local.get( ).get_flags( );
	m_old_velocity_modifier = g_ctx.m_local.get( ).get_velocity_modifier( );
	m_old_move_type         = g_ctx.m_local.get( ).get_move_type( );
	m_old_origin            = g_ctx.m_local.get( ).get_origin( );
	m_old_abs_origin        = g_ctx.m_local.get( ).get_abs_origin( );
	m_old_view_offset       = g_ctx.m_local.get( ).get_view_offset( );
	m_old_velocity          = g_ctx.m_local.get( ).get_velocity( );
	m_old_duck_amount       = g_ctx.m_local.get( ).get_duck_amount( );
	m_old_duck_speed        = g_ctx.m_local.get( ).get_duck_speed( );
	m_old_fall_velocity     = g_ctx.m_local.get( ).get_fall_velocity( );
	m_old_sim_time          = g_ctx.m_local.get( ).get_sim_time( );
	m_old_ground_entity     = g_ctx.m_local.get( ).get_ground_entity( );
}

void hack::movement::backup::set( ) {
	g_ctx.m_local.get( ).get_flags( )             = m_old_flags;
	g_ctx.m_local.get( ).get_velocity_modifier( ) = m_old_velocity_modifier;
	g_ctx.m_local.get( ).get_move_type( )         = m_old_move_type;
	g_ctx.m_local.get( ).get_origin( )            = m_old_origin;
	g_ctx.m_local.get( ).set_abs_origin( m_old_abs_origin );
	g_ctx.m_local.get( ).get_view_offset( )   = m_old_view_offset;
	g_ctx.m_local.get( ).get_velocity( )      = m_old_velocity;
	g_ctx.m_local.get( ).get_duck_amount( )   = m_old_duck_amount;
	g_ctx.m_local.get( ).get_duck_speed( )    = m_old_duck_speed;
	g_ctx.m_local.get( ).get_fall_velocity( ) = m_old_fall_velocity;
	g_ctx.m_local.get( ).get_sim_time( )      = m_old_sim_time;
	g_ctx.m_local.get( ).get_ground_entity( ) = m_old_ground_entity;
}

void hack::movement::pre( ) {
	m_jumpbugged     = false;
	m_longjumped     = false;
	m_old_velocity_z = g_ctx.m_local.get( ).get_velocity( )[ 2 ];
	m_base_flags     = g_ctx.m_local.get( ).get_flags( );
	m_lj_grounded    = g_ctx.m_local.get( ).get_ground_entity( ).get( ) && g_ctx.m_local.get( ).get_flags( ) & 1;

	no_duck_delay( );
	bunnyhop( );

	m_edgebug.prepare( );
}

// NOTE(para): this is CERTAINLY red!!
void hack::movement::no_duck_delay( ) {
	g_ctx.m_cmd->m_buttons |= ( 1 << 22 );
}

// NOTE(para): this technically could be 'red' but I doubt it, but we should mark it for good measure (low warning)
// anyway again it should be just fine
void hack::movement::bunnyhop( ) {
	if ( !true || local_on_ladder_or_noclip( ) || m_in_jumpbug )
		return;

	if ( g_ctx.m_cmd->m_buttons & ( 1 << 1 ) && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) )
		g_ctx.m_cmd->m_buttons &= ~( 1 << 1 );
}

void hack::movement::jumpbug( ) {
	if ( !( true && g_io.key_state< io::key_state::DOWN >( VK_XBUTTON1 ) ) || local_on_ladder_or_noclip( ) ) {
		m_in_jumpbug = false;
		return;
	}

	m_in_jumpbug = false;
	if ( g_ctx.m_local.get( ).get_flags( ) & 1 ) {
		m_in_jumpbug = true;
		if ( !( m_base_flags & 1 ) )
			g_ctx.m_cmd->m_buttons |= ( 1 << 2 );

		g_ctx.m_cmd->m_buttons &= ~( 1 << 1 );
	}
}

// NOTE(para): this could be red but should be explicitly advertised as so
void hack::movement::longjump( ) {
	static auto& oe = gui::cfg::get< bool >( HASH_CT( "main:group1:longjump on edge" ) );

	if ( !( true && g_io.key_state< io::key_state::DOWN >( VK_XBUTTON2 ) ) || local_on_ladder_or_noclip( ) ) {
		m_lj_ducked_ticks    = 0;
		m_wait_till_grounded = false;
		return;
	}

	if ( m_wait_till_grounded ) {
		if ( !m_lj_grounded )
			return;
		else
			m_lj_grounded = false;
	}

	if ( oe ? ( !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) : true ) {
		g_ctx.m_cmd->m_buttons &= ~( 1 << 3 );
		g_ctx.m_cmd->m_buttons &= ~( 1 << 4 );
		g_ctx.m_cmd->m_forward_move = 0;

		bool ever = false;
		if ( m_base_flags & 1 ) {
			ever = true;
			g_ctx.m_cmd->m_buttons |= ( 1 << 1 );
			m_lj_ducked_ticks = 0;
		}

		if ( m_lj_ducked_ticks < 2 ) {
			ever = true;
			g_ctx.m_cmd->m_buttons |= ( 1 << 2 );
			++m_lj_ducked_ticks;
		}

		m_longjumped = ever;

		if ( !oe ) {
			m_wait_till_grounded = true;
		}
	}
}

void hack::movement::post( ) {
	jumpbug( );
	longjump( );

	// NOTE(para) : I know at least second last check is pedantic
	m_jumpbugged = ( g_ctx.m_local.get( ).get_velocity( )[ 2 ] > m_old_velocity_z ) &&
	               ( !g_ctx.m_local.get( ).get_ground_entity( ).get( ) && !( m_base_flags & 1 ) && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) &&
	               !local_on_ladder_or_noclip( );

	edgebug( );
}

void hack::movement::edgebug::prepare( ) {
	m_backup.store( );
	g_csgo.m_prediction->setup_move( g_ctx.m_local, g_ctx.m_cmd, g_csgo.m_move_helper, &m_move_data );
}

void hack::movement::edgebug::set( ) {
	g_csgo.m_prediction->finish_move( g_ctx.m_local, g_ctx.m_cmd, &m_move_data );
	m_backup.m_old_flags |= ( 1 << 1 );
	m_move_data.m_buttons |= ( 1 << 2 );
	m_backup.set( );
}

void hack::movement::edgebug::change_move_data( ) {
	if ( m_should_duck )
		g_hack.m_prediction.m_move_data.m_buttons |= ( 1 << 2 );

	g_hack.m_prediction.m_move_data.m_buttons &= ~( 1 << 9 );
	g_hack.m_prediction.m_move_data.m_buttons &= ~( 1 << 10 );
	g_hack.m_prediction.m_move_data.m_side_move = 0;

	g_hack.m_prediction.m_move_data.m_buttons &= ~( 1 << 3 );
	g_hack.m_prediction.m_move_data.m_buttons &= ~( 1 << 4 );
	g_hack.m_prediction.m_move_data.m_forward_move = 0;
}

void hack::movement::edgebug::predict( i32 base_flags, f32 base_velocity ) {
	if ( static_cast< int >( g_ctx.m_local.get( ).get_velocity( )[ 2 ] ) == 0 || base_flags & 1 ) {
		m_predicted    = false;
		m_fail_predict = true;
	} else if ( base_velocity < -6.F && g_ctx.m_local.get( ).get_velocity( )[ 2 ] > base_velocity &&
	            g_ctx.m_local.get( ).get_velocity( )[ 2 ] < -6.F && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
		auto zvel = g_ctx.m_local.get( ).get_velocity( )[ 2 ];

		g_hack.m_prediction.start( );
		change_move_data( );
		g_hack.m_prediction.apply( );
		g_hack.m_prediction.restore( );

		auto gravity_velocity = roundf( ( -g_ctx.m_cvars.sv_gravity->get_float( ) ) * g_csgo.m_globals->m_interval_per_tick + zvel );

		if ( gravity_velocity == roundf( g_ctx.m_local.get( ).get_velocity( )[ 2 ] ) ) {
			m_predicted    = true;
			m_fail_predict = false;
		} else {
			m_predicted    = false;
			m_fail_predict = true;
		}
	}
}

void hack::movement::edgebug::run( i32 base_flags, f32 base_velocity ) {
	static auto& rd = gui::cfg::get< i32 >( HASH_CT( "main:group1:edgebug radius" ) );
	if ( !( true && g_io.key_state< io::key_state::DOWN >( VK_MBUTTON ) ) || local_on_ladder_or_noclip( ) ) {
		m_predicted   = false;
		m_should_duck = false;
		return;
	}

	g_ctx.m_cmd->m_buttons &= ~( 1 << 2 );
	if ( !m_predicted ) {
		m_should_duck = false;

		for ( auto runs = 0; runs < 2; ++runs ) {
			if ( runs == 1 ) {
				m_should_duck = true;
				set( );
			}

			if ( runs == 0 )
				g_ctx.m_local.get( ).get_flags( ) = m_backup.m_old_flags;

			for ( auto radius = 1; radius <= rd; ++radius ) {
				if ( runs == 1 ) {
					g_hack.m_prediction.start( );
					change_move_data( );
					g_hack.m_prediction.apply( );
					g_hack.m_prediction.restore( );
				}

				if ( base_flags & 1 || base_velocity > 0.F ) {
					m_predicted = false;
					break;
				}

				predict( base_flags, base_velocity );

				if ( m_predicted ) {
					m_simulation_tick      = radius;
					m_simulation_timestamp = g_csgo.m_globals->m_tickcount;
					break;
				}

				if ( m_fail_predict ) {
					m_fail_predict = false;
					break;
				}

				if ( runs == 0 ) {
					g_hack.m_prediction.start( );
					change_move_data( );
					g_hack.m_prediction.apply( );
					g_hack.m_prediction.restore( );
				}
			}

			if ( m_predicted )
				break;
		}
	} else {
		if ( g_csgo.m_globals->m_tickcount < ( m_simulation_tick + m_simulation_timestamp ) ) {
			g_ctx.m_cmd->m_buttons &= ~( 1 << 3 );
			g_ctx.m_cmd->m_buttons &= ~( 1 << 4 );
			g_ctx.m_cmd->m_forward_move = 0.F;
			g_ctx.m_cmd->m_side_move    = 0.F;

			if ( m_should_duck )
				g_ctx.m_cmd->m_buttons |= ( 1 << 2 );
		} else {
			m_predicted   = false;
			m_should_duck = false;
		}
	}
}

void hack::movement::edgebug( ) {
	m_edgebug.run( m_base_flags, m_old_velocity_z );
}

void hack::movement::edgebug_scale_mouse( f32& x ) {
	static auto& es = gui::cfg::get< i32 >( HASH_CT( "main:group1:edgebug scaling" ) );
	if ( ( true && g_io.key_state< io::key_state::DOWN >( VK_MBUTTON ) ) && m_edgebug.m_predicted )
		x *= ( 1.F - ( .01F * es ) );
}

void hack::movement::clear( ) {
	m_jumpbugged                     = false;
	m_longjumped                     = false;
	m_edgebug.m_predicted            = false;
	m_edgebug.m_fail_predict         = false;
	m_edgebug.m_should_duck          = false;
	m_edgebug.m_simulation_tick      = 0;
	m_edgebug.m_simulation_timestamp = 0;
}