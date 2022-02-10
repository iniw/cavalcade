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
	m_old_impulse           = g_ctx.m_local.get( ).get_impulse( );
	m_old_buttons           = g_ctx.m_local.get( ).get_buttons( );
	m_old_base_velocity     = g_ctx.m_local.get( ).get_base_velocity( );
	m_old_eflags            = g_ctx.m_local.get( ).get_eflags( );
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
	g_ctx.m_local.get( ).get_impulse( )       = m_old_impulse;
	g_ctx.m_local.get( ).get_buttons( )       = m_old_buttons;
	g_ctx.m_local.get( ).get_base_velocity( ) = m_old_base_velocity;
	g_ctx.m_local.get( ).get_eflags( )        = m_old_eflags;
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

	m_in_jumpbug = true;
	if ( g_ctx.m_local.get( ).get_flags( ) & 1 ) {
		if ( !( m_base_flags & 1 ) )
			g_ctx.m_cmd->m_buttons |= ( 1 << 2 );

		g_ctx.m_cmd->m_buttons &= ~( 1 << 1 );
	}

	// NOTE(para) : I know at least second last check is pedantic
	m_jumpbugged = ( ( g_ctx.m_local.get( ).get_velocity( )[ 2 ] > m_old_velocity_z ) &&
	                 ( !g_ctx.m_local.get( ).get_ground_entity( ).get( ) && !( m_base_flags & 1 ) && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) ) &&
	               !local_on_ladder_or_noclip( );

	if ( m_jumpbugged )
		g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
			0, 0, XOR( "<<<NO_TRANSLATE>>> <font color=\"#fbf7f5\">movement</font><font color=\"#B9B9B9\"> | jumpbugged</font>" ) );
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

void hack::movement::ladderjump( ) {
	if ( !( true && g_io.key_state< io::key_state::DOWN >( 'V' ) ) ) {
		m_ladder_jump_tick = 0;
		m_in_ladderjump    = false;
		return;
	}

	m_in_ladderjump = false;

	static auto& claj = gui::cfg::get< bool >( HASH_CT( "main:group1:crouch after ladderjump" ) );

	auto base_move_type = g_ctx.m_local.get( ).get_move_type( );

	g_hack.m_prediction.start( );
	g_hack.m_prediction.apply( );
	g_hack.m_prediction.restore( );

	if ( base_move_type == sdk::move_type::LADDER && g_ctx.m_local.get( ).get_move_type( ) != sdk::move_type::LADDER ) {
		m_ladder_jump_tick = g_csgo.m_globals->m_tickcount;
		g_ctx.m_cmd->m_buttons |= ( 1 << 1 );
		g_ctx.m_cmd->m_forward_move = 0;
		g_ctx.m_cmd->m_side_move    = 0;
		g_ctx.m_cmd->m_buttons &= ~( 1 << 3 );
		g_ctx.m_cmd->m_buttons &= ~( 1 << 4 );
		g_ctx.m_cmd->m_buttons &= ~( 1 << 9 );
		g_ctx.m_cmd->m_buttons &= ~( 1 << 10 );
		m_in_ladderjump = true;
	}

	if ( claj ) {
		if ( ( g_csgo.m_globals->m_tickcount - m_ladder_jump_tick ) > 1 && ( g_csgo.m_globals->m_tickcount - m_ladder_jump_tick ) < 15 ) {
			g_ctx.m_cmd->m_forward_move = 0;
			g_ctx.m_cmd->m_side_move    = 0;
			g_ctx.m_cmd->m_buttons &= ~( 1 << 3 );
			g_ctx.m_cmd->m_buttons &= ~( 1 << 4 );
			g_ctx.m_cmd->m_buttons &= ~( 1 << 9 );
			g_ctx.m_cmd->m_buttons &= ~( 1 << 10 );
			g_ctx.m_cmd->m_buttons |= ( 1 << 2 );
			m_in_ladderjump = true;
		}
	}
}

void hack::movement::pixelsurf_calculator( ) { }

void hack::movement::post( ) {
	jumpbug( );
	longjump( );
	ladderjump( );

	// if ( *( g_csgo.m_main_view_origin )[ 2 ] )
	edgebug( );

	m_pixelsurf.run( );
	m_pixelsurf.autoalign( );
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
	} else if ( base_velocity < -6.25F && floor( g_ctx.m_local.get( ).get_velocity( )[ 2 ] ) > floor( base_velocity ) &&
	            g_ctx.m_local.get( ).get_velocity( )[ 2 ] < -6.25F && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
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
		m_in_edgebug  = false;
		return;
	}

	m_in_edgebug = true;
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

				if ( base_flags & 1 || base_velocity > 0.F || g_ctx.m_local.get( ).get_velocity( )[ 2 ] > 0.F ) {
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
			else
				g_ctx.m_cmd->m_buttons &= 0xFFFFFFFB;
		} else {
			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0, XOR( "<<<NO_TRANSLATE>>> <font color=\"#fbf7f5\">movement</font><font color=\"#B9B9B9\"> | edgebugged</font>" ) );

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

void hack::movement::pixelsurf::run( ) {
	/* soar style comment */
	/* credits: bitcheat */

	if ( !( true && g_io.key_state< io::key_state::DOWN >( 'C' ) ) || local_on_ladder_or_noclip( ) ) {
		clear( );
		return;
	}

	i32 ticks       = 0;
	bool predicted  = false;
	static auto& pt = gui::cfg::get< i32 >( HASH_CT( "main:group1:pixelsurf ticks" ) );

	if ( pt > 0 ) {
		do {
			auto base_velocity = g_ctx.m_local.get( ).get_velocity( )[ 2 ];

			g_hack.m_prediction.start( );
			g_hack.m_prediction.apply( );

			// NOTE(para): I have no idea why this is running <<during>> prediction, but whatever the boss says
			if ( base_velocity < -6.25F && g_ctx.m_local.get( ).get_velocity( )[ 2 ] < 6.25F &&
			     g_ctx.m_local.get( ).get_velocity( )[ 2 ] > base_velocity && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
				predicted     = true;
				m_duration    = ticks;
				m_old_buttons = g_ctx.m_cmd->m_buttons;
			}
			g_hack.m_prediction.restore( );

			++ticks;
		} while ( ticks < pt );

		if ( predicted )
			goto apply;
	}

	if ( g_ctx.m_local.get( ).get_velocity( )[ 2 ] == -6.25F || g_ctx.m_local.get( ).get_velocity( )[ 2 ] == -3.125 ) {
	apply:
		m_autoalign = true;
		g_ctx.m_cmd->m_buttons |= 4;
	} else {
		m_autoalign = false;
	}

	if ( m_duration == -1 ) {
		m_lock_mouse = false;
	} else {
		--m_duration;
		m_lock_mouse           = 1;
		g_ctx.m_cmd->m_buttons = m_old_buttons;
	}
}

constexpr static int positions[] = { 16.0399, 0,   0,   16.0399, 0,   0,   0,   30,  60,  142, 81,  120, 61,  141, 82,
	                                 121,     268, 121, 249,     141, 270, 120, 249, 142, 255, 255, 255, 255, -1,  -16.0399 };

// NOTE(para): thanks @soar for help here
static int trace( ) {
	float v1 = 0;
	int v2   = 1;
	int v11;
	int v17 = 0;
	float i;

	for ( auto i = 1; i < sizeof( positions ) / sizeof( int ); ++i ) {
		if ( i > sizeof( positions ) / sizeof( int ) )
			break;

		sdk::trace trace{ };
		sdk::trace_filter filter( g_ctx.m_local );
		math::v3f dest{ };
		auto origin = g_ctx.m_local.get( ).get_origin( );
		dest[ 0 ]   = positions[ i - 1 ] + origin[ 0 ];
		dest[ 1 ]   = positions[ i ] + origin[ 1 ];
		dest[ 2 ]   = origin[ 2 ];
		sdk::ray ray( origin, dest );

		g_csgo.m_engine_trace->trace_ray( ray, ( uint32_t )-1, filter, trace );

		if ( trace.m_fraction < 1.F || trace.m_all_solid || trace.m_start_solid ) {
			auto is_player = trace.m_entity->is_player( );
			v11            = v17;
			if ( !is_player )
				v11 = v2;
			v17 = v11;
		} else {
			v11 = v17;
		}

		if ( ++v2 > 4 )
			break;
	}

	return v11;
}

void hack::movement::pixelsurf::autoalign( ) {
	if ( m_autoalign ) {
		float i;
		auto origin = g_ctx.m_local.get( ).get_origin( );
		f32 v17     = floor( origin[ 1 ] );
		f32 v20     = 1.F - ( origin[ 1 ] - v17 );
		v17         = floor( origin[ 0 ] );
		f32 v19     = 1.F - ( origin[ 0 ] - v17 );
		v17         = floor( origin[ 1 ] );
		f32 v22     = origin[ 0 ] - v17;
		v17         = floor( origin[ 0 ] );
		f32 v21     = origin[ 0 ] - v17;

		float v8   = 0;
		int v9     = 0;
		int v6     = 0;
		int v7     = 0;
		int v10    = 0;
		float v11  = 0.F;
		int v12    = 0;
		bool state = false;
		if ( ( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
			state = false;
			goto LABEL_35;
		}
		v6 = trace( );
		v7 = v6;

		if ( v6 != -1 ) {
			switch ( v6 ) {
			case 2:
				v9  = 0;
				v11 = g_ctx.m_cmd->m_side_move;
				if ( v11 > 0.F )
					v9 = 2;
				if ( v11 <= 0.F )
					v10 = ( v11 >= 0.F ) - 1;
				else
					v10 = 1;
				goto LABEL_36;
			case 3:
				v8 = g_ctx.m_cmd->m_side_move;
				if ( v8 <= 0.F ) {
					v17 = v8 < 0.F;
					v9  = v17;
				} else {
					v9 = -1;
				}

				if ( v8 > 0.F ) {
					v10 = 1;
					goto LABEL_36;
				}
			LABEL_33:
				v10 = ( v8 >= 0.F ) - 1;
				goto LABEL_36;
			case 4:
				v8 = g_ctx.m_cmd->m_side_move;
				if ( v8 <= 0.F ) {
					v12 = 0;
					if ( v8 < 0.F )
						v12 = -2;

					v17 = v12;
					v9  = v12;
				} else {
					v9 = 0;
				}
				if ( v8 > 0.F ) {
					v10 = 1;
					goto LABEL_36;
				}
				goto LABEL_33;
			}
		LABEL_35:
			v9  = 0;
			v10 = 0;
			goto LABEL_36;
		}

	LABEL_36:
		auto v13 = g_ctx.m_local.get( ).get_velocity( );
		v17      = atan2( 30.F, v13.length_2d( ) );
		auto v14 = v17 * 57.295776;
		if ( v14 <= 90.F ) {
			if ( v14 < 0.F )
				v14 = 0;
		} else {
			v14 = 90.F;
		}

		if ( g_ctx.m_cmd->m_side_move != 0.F && v7 &&
		     ( v19 >= 0.00098999997 && v19 <= 0.03125 || v20 >= 0.00098999997 && v20 <= 0.03125 || v21 >= 0.00098999997 && v21 <= 0.03125 ||
		       v22 >= 0.00098999997 && v22 <= 0.03125 ) ) {
			// NOTE(para): ???? is this a cursed optimization
			for ( i = ( float )( ( float )v10 * v14 ) + ( float )( ( float )v9 * 90.0 ); i < -180.0; i = i + 360.0 )
				;
			for ( ; i > 180.0; i = i - 360.0 )
				;
			{
				constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };

				auto yaw       = i;
				auto old_yaw   = yaw + ( yaw < 0.0f ? 360.0f : 0.0f );
				auto new_yaw   = g_ctx.m_cmd->m_view_angles.yaw + ( g_ctx.m_cmd->m_view_angles.yaw < 0.0f ? 360.0f : 0.0f );
				auto yaw_delta = new_yaw < old_yaw ? fabsf( new_yaw - old_yaw ) : 360.0f - fabsf( new_yaw - old_yaw );
				yaw_delta      = 360.0f - yaw_delta;

				auto forwardmove            = g_ctx.m_cmd->m_forward_move;
				auto sidemove               = g_ctx.m_cmd->m_side_move;
				g_ctx.m_cmd->m_forward_move = cos( DEG2RAD( yaw_delta ) ) * forwardmove + cos( DEG2RAD( yaw_delta + 90.0f ) ) * sidemove;
				g_ctx.m_cmd->m_side_move    = sin( DEG2RAD( yaw_delta ) ) * forwardmove + sin( DEG2RAD( yaw_delta + 90.0f ) ) * sidemove;
				g_ctx.m_cmd->m_forward_move = std::clamp( g_ctx.m_cmd->m_forward_move, -450.0f, 450.0f );
				g_ctx.m_cmd->m_side_move    = std::clamp( g_ctx.m_cmd->m_side_move, -450.0f, 450.0f );
			}
		}
	}
}

void hack::movement::pixelsurf::clear( ) {
	m_duration   = -1;
	m_lock_mouse = 0;
	m_autoalign  = false;
}

void hack::movement::clear( ) {
	m_pixelsurf_calculator_point     = std::nullopt;
	m_jumpbugged                     = false;
	m_longjumped                     = false;
	m_edgebug.m_predicted            = false;
	m_edgebug.m_fail_predict         = false;
	m_edgebug.m_should_duck          = false;
	m_edgebug.m_simulation_tick      = 0;
	m_edgebug.m_simulation_timestamp = 0;
	m_pixelsurf.clear( );
}