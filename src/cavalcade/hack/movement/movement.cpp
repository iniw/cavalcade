#include "../hack.hpp"
#include "movement.hpp"
#include "../../ctx/ctx.hpp"
#include "../../gui/cfg/cfg.hpp"
#include <limits>
#include "../aimbot/autowall/autowall.hpp"

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
	m_base_origin    = g_ctx.m_local.get( ).get_origin( );
	m_lj_grounded    = g_ctx.m_local.get( ).get_ground_entity( ).get( ) && g_ctx.m_local.get( ).get_flags( ) & 1;

	no_duck_delay( );

	bunnyhop( );

	m_in_minijump = false;
	if ( ( true && g_io.key_state< io::key_state::DOWN >( 'B' ) ) && !local_on_ladder_or_noclip( ) ) {
		auto buttons = g_ctx.m_cmd->m_buttons;
		if ( ( buttons & 2 ) != 0 ) {
			g_ctx.m_cmd->m_buttons = buttons | 4;
			m_in_minijump          = true;
		}
	}

	m_jumpstats.run( );

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

	// NOTE(para): uncomment this when jumpstast are menu option because it looks ugly with this and em on
	if ( m_jumpbugged && false )
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
		if ( ( g_csgo.m_globals->m_tickcount - m_ladder_jump_tick ) > 3 && ( g_csgo.m_globals->m_tickcount - m_ladder_jump_tick ) < 15 ) {
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

	m_pixelsurf.run( m_base_origin );
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
	if ( roundf( g_ctx.m_local.get( ).get_velocity( )[ 2 ] ) == 0.F || base_flags & 1 ) {
		m_predicted    = false;
		m_fail_predict = true;
	} else if ( base_velocity < -6.25F && floor( g_ctx.m_local.get( ).get_velocity( )[ 2 ] ) > floor( base_velocity ) &&
	            g_ctx.m_local.get( ).get_velocity( )[ 2 ] < -6.25F && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
		auto velocity = g_ctx.m_local.get( ).get_velocity( )[ 2 ];

		g_hack.m_prediction.start( );
		change_move_data( );
		g_hack.m_prediction.apply( );
		g_hack.m_prediction.restore( );

		velocity = velocity - ( float )( g_ctx.m_cvars.sv_gravity->get_float( ) * g_csgo.m_globals->m_interval_per_tick );
		velocity = roundf( velocity );

		if ( velocity == roundf( g_ctx.m_local.get( ).get_velocity( )[ 2 ] ) ) {
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

	static auto addy = g_mem[ CLIENT_DLL ].get_address< void( __stdcall* )( int a, int b ) >( HASH_CT( "RestoreEntityToPredictedFrame" ) );
	addy( 0, g_csgo.m_prediction->m_commands_predicted - 1 );

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
	}

	if ( m_predicted ) {
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

			g_hack.m_movement.m_jumpstats.reset( );

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

void hack::movement::pixelsurf::run( const math::v3f& base_origin ) {
	auto get_align_side = [ & ]( const math::v3f& base_origin ) -> std::optional< math::v3f > {
		f32 best_fraction = std::numeric_limits< f32 >::max( );
		i32 best_trace    = -1;

		auto mins = g_ctx.m_local.get( ).get_mins( );
		auto maxs = g_ctx.m_local.get( ).get_maxs( );

		sdk::trace info[ 4 ];

		for ( auto i = 0; i < 4; ++i ) {
			auto start = base_origin, end = base_origin;
			switch ( i ) {
			case 0:
				start[ 1 ] += mins[ 1 ];
				end[ 1 ] = floor( start[ 1 ] );
				break;
			case 1:
				start[ 0 ] += maxs[ 0 ];
				end[ 0 ] = floor( start[ 0 ] ) + 1.F;
				break;
			case 2:
				start[ 1 ] += maxs[ 1 ];
				end[ 1 ] = floor( start[ 1 ] ) + 1.F;
				break;
			case 3:
				start[ 0 ] += mins[ 0 ];
				end[ 0 ] = floor( start[ 0 ] );
				break;
			}

			sdk::trace_filter filter( g_ctx.m_local );
			sdk::ray ray( base_origin, end );
			auto& trace = info[ i ];
			g_csgo.m_engine_trace->trace_ray( ray, 0x201400b, filter, trace );
		}

		for ( auto i = 0; i < 4; ++i ) {
			const auto& trace = info[ i ];

			if ( ( trace.m_fraction < 1.F || trace.m_all_solid || trace.m_start_solid ) &&
			     ( trace.m_entity ? !trace.m_entity->is_player( ) : true ) ) {
				if ( best_fraction > trace.m_fraction ) {
					best_fraction = trace.m_fraction;
					best_trace    = i;
				}
			}
		}

		if ( best_trace != -1 ) {
			return info[ best_trace ].m_end;
		}

		return std::nullopt;
	};
	/* soar style comment */
	/* credits: bitcheat */

	if ( !( true && g_io.key_state< io::key_state::DOWN >( 'C' ) ) || local_on_ladder_or_noclip( ) ) {
		clear( );
		return;
	}

	static auto addy = g_mem[ CLIENT_DLL ].get_address< void( __stdcall* )( int a, int b ) >( HASH_CT( "RestoreEntityToPredictedFrame" ) );
	addy( 0, g_csgo.m_prediction->m_commands_predicted - 1 );

	auto side         = get_align_side( base_origin );
	auto backup_fmove = g_ctx.m_cmd->m_forward_move;
	auto backup_smove = g_ctx.m_cmd->m_side_move;
	if ( side.has_value( ) && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) && g_ctx.m_cmd->m_side_move == 0.F && g_ctx.m_cmd->m_forward_move == 0.F ) {
		constexpr auto normalize = []( const math::ang& a ) {
			math::ang b = a;
			b.pitch     = std::clamp( b.pitch, -89.f, 89.f );

			float rot;
			float& angle = b.yaw;
			// bad number.
			if ( !std::isfinite( angle ) ) {
				angle = 0.f;
				return b;
			}

			// no need to normalize this angle.
			if ( angle >= -180.f && angle <= 180.f )
				return b;

			// get amount of rotations needed.
			rot = std::round( std::abs( angle / 360.f ) );

			// normalize.
			angle = ( angle < 0.f ) ? angle + ( 360.f * rot ) : angle - ( 360.f * rot );

			b.roll = 0.f;
			return b;
		};
		auto _diff = ( ( side.value( ) - base_origin ).to_angle( ) - ( *( math::v3f* )&g_ctx.m_cmd->m_view_angles ) );
		auto diff  = normalize( *( math::ang* )&_diff );

		auto move = math::v3f( 450.F, 0.F, 0.F );
		auto len  = move.length( );
		move.normalize( );

		if ( len ) {
			math::v3f move_angle{ };
			move_angle.vector_angles( move );

			auto delta = ( ( g_ctx.m_cmd->m_view_angles.yaw + diff.yaw ) - g_ctx.m_cmd->m_view_angles.yaw );
			move_angle[ 1 ] += delta;

			auto dir = angle_vectors( *( math::ang* )&move_angle );
			dir *= len;

			if ( g_ctx.m_cmd->m_view_angles.pitch < -90.F || g_ctx.m_cmd->m_view_angles.pitch > 90 )
				dir[ 0 ] = -dir[ 0 ];

			g_ctx.m_cmd->m_forward_move = -dir[ 0 ];
			g_ctx.m_cmd->m_side_move    = dir[ 1 ];

			/*g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ),
			                                      io::format( "-> {} {}\n", g_ctx.m_cmd->m_forward_move, g_ctx.m_cmd->m_side_move ).c_str( ) );*/
		}
	}

	i32 ticks       = 0;
	bool predicted  = false;
	static auto& pt = gui::cfg::get< i32 >( HASH_CT( "main:group1:pixelsurf ticks" ) );

	m_in_pixelsurf = false;

	if ( pt > 0 ) {
		do {
			auto base_velocity = g_ctx.m_local.get( ).get_velocity( )[ 2 ];
			g_hack.m_prediction.start( );
			g_hack.m_prediction.apply( );

			// NOTE(para): I have no idea why this is running <<during>> prediction, but whatever the boss says
			if ( base_velocity < -6.25F && g_ctx.m_local.get( ).get_velocity( )[ 2 ] < -6.25F &&
			     g_ctx.m_local.get( ).get_velocity( )[ 2 ] > base_velocity && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
				predicted     = true;
				m_duration    = ticks;
				m_old_buttons = g_ctx.m_cmd->m_buttons;
			}

			g_hack.m_prediction.restore( );

			if ( predicted )
				break;
			++ticks;
		} while ( ticks < pt );

		if ( predicted )
			goto apply;
	}

	if ( g_ctx.m_local.get( ).get_velocity( )[ 2 ] == -6.25F || g_ctx.m_local.get( ).get_velocity( )[ 2 ] == -3.125F ) {
	apply:
		m_autoalign = false;
		g_ctx.m_cmd->m_buttons |= 4;
		// m_old_buttons |= 4;

		m_in_pixelsurf = true;
	} else {
		// g_ctx.m_cmd->m_forward_move = backup_fmove;
		// g_ctx.m_cmd->m_side_move    = backup_smove;
		// m_autoalign = true;
	}

	// if ( !predicted ) {
	// 	g_ctx.m_cmd->m_forward_move = backup_fmove;
	// 	g_ctx.m_cmd->m_side_move    = backup_smove;
	// }

	if ( m_duration == -1 ) {
		m_lock_mouse = false;
	} else {
		--m_duration;
		m_lock_mouse           = 1;
		g_ctx.m_cmd->m_buttons = m_old_buttons;
	}
}

// constexpr static float positions[] = { 16.0399, 0,   0,   16.0399, 0,   0,   0,   30,  60,  142, 81,  120,
// 61,  141, 82, 	                                 121,     268, 121, 249,     141, 270, 120, 249, 142, 255,
// 255, 255, 255, -1,  -16.0399 };
constexpr static float positions[] = { 0.F, 0.F, 16.0399F, -16.0399F, 0.F, 0.F, -16.0399F, 0.F, 0.F };

// NOTE(para): thanks @soar for help here
static int trace( ) {
	float v1 = 0;
	int v2   = 1;
	int v11;
	int v17 = 0;
	float i;

	for ( auto i = 1; i < sizeof( positions ) / sizeof( float ); ++i ) {
		if ( i > sizeof( positions ) / sizeof( float ) )
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

	// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), io::format( XOR( "chiggy bungus: {}\n" ), v11 ).c_str( ) );

	return v11;
}

void hack::movement::pixelsurf::autoalign( ) {
	if ( m_autoalign ) {
		// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - inside\n" ) );

		i32 AlignmentValue = 0;
		i32 StateInteger   = 0;
		i32 v6             = 0;
		f32 v8             = 0.F;
		bool v10           = false;
		bool v11           = false;
		f32 v9             = 0.F;
		f32 v12            = 0.F;
		f32 v13            = 0.F;
		f32 v16            = 0.F;
		bool v17           = false;
		bool v18           = false;
		i32 v56            = 0;
		f32 v57            = 0.F;
		bool v14           = false;
		bool v15           = false;

		f32 v23  = 0.F;
		f32 __Za = 0.F;
		f32 __Zb = 0.F;
		f32 v24  = 0.F;
		f32 v25  = 0.F;
		f32 v26  = 0.F;
		f32 v27  = 0.F;
		f32 v28  = 0.F;
		f32 v29  = 0.F;
		f32 v30  = 0.F;
		f32 v31  = 0.F;
		f32 v32  = 0.F;
		f32 v33  = 0.F;
		f32 v34  = 0.F;
		f32 v35  = 0.F;

		f32 StateIntegera = 0.F;
		f32 StateIntegerb = 0.F;
		f32 StateIntegerc = 0.F;
		auto origin       = g_ctx.m_local.get( ).get_origin( );
		f32 __floor       = floor( origin[ 1 ] );
		f32 __X           = 1.F - ( origin[ 1 ] - __floor );
		v17               = floor( origin[ 0 ] );
		f32 __Y           = 1.F - ( origin[ 0 ] - __floor );
		v17               = floor( origin[ 1 ] );
		f32 __Z           = origin[ 0 ] - __floor;
		v17               = floor( origin[ 0 ] );
		f32 __W           = origin[ 0 ] - __floor;

		if ( g_ctx.m_local.get( ).get_flags( ) & 1 ) {
			AlignmentValue = 0;
			StateInteger   = 0;
			// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - F AIL\n" ) );
		} else {
			v6             = trace( );
			AlignmentValue = v6;
			// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - chiggy bungus - passed trace\n" )
			// );

			switch ( v6 ) {
			case 1:
				// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - case 1\n" ) );

				v8 = g_ctx.m_cmd->m_side_move;
				if ( v8 <= 0.0 ) {
					v10          = v8 > 0.0;
					v11          = 0.0 == v8;
					StateInteger = -1;
					v9           = 0.0;
					if ( v10 || v11 )
						StateInteger = 0;
				} else {
					v9           = 0.0;
					StateInteger = 1;
				}
				v12 = g_ctx.m_cmd->m_side_move;
				if ( v12 > v9 ) {
					v56 = 1;
					goto LABEL_39;
				}
			LABEL_15:
				if ( v12 < v9 ) {
					v56 = -1;
					goto LABEL_39;
				}
				goto LABEL_38;
			case 2:
				// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - case 2\n" ) );

				StateInteger = 2;
				v9           = 0.0;
				if ( g_ctx.m_cmd->m_side_move <= 0.0 )
					StateInteger = 0;
				v12 = g_ctx.m_cmd->m_side_move;
				if ( v12 > 0.0 ) {
					v56 = 1;
					goto LABEL_39;
				}
				goto LABEL_15;
			case 3:
				// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - case 3\n" ) );

				v13 = g_ctx.m_cmd->m_side_move;
				if ( v13 <= 0.0 ) {
					v14          = v13 > 0.0;
					v15          = 0.0 == v13;
					StateInteger = 1;
					v9           = 0.0;
					if ( v14 || v15 )
						StateInteger = 0;
				} else {
					v9           = 0.0;
					StateInteger = -1;
				}
				v12 = g_ctx.m_cmd->m_side_move;
				if ( v12 > v9 ) {
					v56 = 1;
					goto LABEL_39;
				}
				goto LABEL_15;
			case 4:
				// g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - case 4\n" ) );

				v57 = g_ctx.m_cmd->m_side_move;
				v16 = v57;
				if ( v57 <= 0.0 ) {
					v17          = v16 > 0.0;
					v18          = 0.0 == v16;
					StateInteger = -2;
					v9           = 0.0;
					if ( !v17 && !v18 )
						goto LABEL_34;
				} else {
					v9 = 0.0;
				}
				StateInteger = 0;
			LABEL_34:
				v12 = v57;
				if ( v57 > v9 ) {
					v56 = 1;
					goto LABEL_39;
				}
				goto LABEL_15;
			}
			StateInteger = 0;
		}

	LABEL_38:
		v56 = 0;
	LABEL_39:
		auto v46 = g_ctx.m_local.get( ).get_velocity( ).length_2d( );
		auto v47 = 4.5 / v46;
		auto v48 = atan( v47 );
		auto v49 = v48 * 57.2957763671875;
		auto v50 = v49 * 6.283185482025146;
		f32 v20  = 0.F;
		f32 v21  = 0.F;
		f32 v22  = 0.F;
		f32 v51  = 0.F;
		if ( v50 > 90.0 ) {
			v20 = 90.0;
			v21 = 90.0;
		LABEL_44:
			v51 = v21;
			v22 = 0.0;
			goto LABEL_45;
		}
		if ( v50 >= 0.0 ) {
			v21 = v50;
			v20 = 90.0;
			goto LABEL_44;
		}
		v20 = 90.0;
		v22 = 0.0;
		v51 = 0.0;

	LABEL_45:
		if ( g_ctx.m_cmd->m_side_move == v22 || !AlignmentValue )
			return;
		if ( __Y < 0.00050000002 ) {
			v23 = 0.03125;
		LABEL_52:
			if ( ( __X < 0.00050000002 || __X > v23 ) && ( __W < 0.00050000002 || __W > v23 ) && ( __Z < 0.00050000002 || v23 < __Z ) )
				return;
			goto LABEL_49;
		}
		v23 = 0.03125;
		if ( __Y > 0.03125 )
			goto LABEL_52;
	LABEL_49:
		__Za          = ( float )v56;
		v24           = __Za * v51;
		__Zb          = ( float )StateInteger;
		StateIntegerc = v20 * __Zb + v24;
		v25           = StateIntegerc;
		v26           = 360.0;
		if ( StateIntegerc < -180.0 ) {
			while ( 1 ) {
				v28           = v26;
				v29           = v25 + v26;
				v30           = v28;
				StateIntegera = v29;
				if ( StateIntegera >= -180.0 )
					break;
				v26 = v30;
				v25 = StateIntegera;
			}
			v31 = v30;
			v25 = StateIntegera;
			v27 = v31;
		} else {
			v27 = 360.0;
		}
		v32          = 180.0;
		f32 setangle = 0.F;
		if ( v25 > 180.0 ) {
			while ( 1 ) {
				v33           = v32;
				v34           = v25;
				v35           = v33;
				StateIntegerb = v34 - v27;
				if ( StateIntegerb <= v33 )
					break;
				v32 = v35;
				v25 = StateIntegerb;
			}
			setangle = StateIntegerb;
		} else {
			setangle = v25;
		}

		{
			constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };
			float rotation         = DEG2RAD( g_ctx.m_cmd->m_view_angles.yaw - setangle );

			float cos_rot = std::cos( rotation );
			float sin_rot = std::sin( rotation );

			auto fmove = g_ctx.m_cmd->m_forward_move;
			auto smove = g_ctx.m_cmd->m_side_move;

			float new_forwardmove = ( cos_rot * fmove ) - ( sin_rot * smove );
			float new_sidemove    = ( sin_rot * fmove ) + ( cos_rot * smove );

			g_ctx.m_cmd->m_forward_move = new_forwardmove;
			g_ctx.m_cmd->m_side_move    = new_sidemove;
		}
		// } else {
		// 	g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "chiggy bungus - FAIL\n" ) );
		// }
	}
}

void hack::movement::pixelsurf::clear( ) {
	m_duration   = -1;
	m_lock_mouse = 0;
	// m_autoalign    = false;
	m_in_pixelsurf = false;
}

void hack::movement::jumpstats::run( ) {
	if ( local_on_ladder_or_noclip( ) ) {
		reset( );
		return;
	}

	bool landed   = false;
	auto velocity = g_ctx.m_local.get( ).get_velocity( );

	i32 strafe_ticks       = 0;
	i32 perfect_sync_ticks = 0;

	if ( g_ctx.m_cmd->m_buttons & 2 && !m_calculate_height ) {
		m_jump_origin      = g_ctx.m_local.get( ).get_origin( );
		m_calculate_height = true;
	}

	if ( !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
		m_air_tick = g_csgo.m_globals->m_tickcount;

		if ( g_ctx.m_cmd->m_moused_x < 0 ) {
			++strafe_ticks;

			if ( !m_strafe_left && !m_strafe_right )
				m_strafe_right = true;
		} else if ( g_ctx.m_cmd->m_moused_x > 0 ) {
			++strafe_ticks;

			if ( !m_strafe_left && !m_strafe_right )
				m_strafe_left = true;
		}

		if ( g_ctx.m_cmd->m_moused_x < 0 && m_strafe_left && !m_strafe_right ) {
			m_strafe_right = true;
			m_strafe_left  = false;

			if ( g_ctx.m_cmd->m_buttons & ( 1 << 10 ) && !( g_ctx.m_cmd->m_buttons & ( 1 << 9 ) ) )
				++perfect_sync_ticks;

			++m_strafes;
		} else if ( g_ctx.m_cmd->m_moused_x > 0 && !m_strafe_left && m_strafe_right ) {
			m_strafe_left  = true;
			m_strafe_right = false;

			if ( !( g_ctx.m_cmd->m_buttons & ( 1 << 10 ) ) && g_ctx.m_cmd->m_buttons & ( 1 << 9 ) )
				++perfect_sync_ticks;

			++m_strafes;
		}

		if ( g_csgo.m_globals->m_tickcount != m_duration_ticks ) {
			m_duration_ticks_pending = g_csgo.m_globals->m_tickcount - m_on_ground_tick;
			m_duration_ticks         = g_csgo.m_globals->m_tickcount;
		}

		m_max    = std::max( m_max, ( i32 )g_ctx.m_local.get( ).get_velocity( ).length_2d( ) );
		m_height = std::max( m_height, g_ctx.m_local.get( ).get_origin( )[ 2 ] - m_jump_origin[ 2 ] + 4.5999F );

		if ( !m_last_jumping ) {
			m_jump_position[ 0 ] = g_ctx.m_local.get( ).get_origin( );
			m_pre                = g_ctx.m_local.get( ).get_velocity( ).length_2d( );
		}

		m_last_jumping = true;
		goto update;
	}

	if ( g_ctx.m_local.get( ).get_flags( ) & 1 ) {
		m_on_ground_tick = g_csgo.m_globals->m_tickcount;

		if ( m_last_jumping ) {
			landed               = true;
			m_jump_position[ 1 ] = g_ctx.m_local.get( ).get_origin( );
		}

		m_last_jumping = false;
	}

	if ( !landed )
		goto update;

	m_distance = 32.F + ( m_jump_position[ 1 ] - m_jump_position[ 0 ] ).length_2d( );
	if ( perfect_sync_ticks && strafe_ticks )
		m_sync = std::min( 100.F, ( perfect_sync_ticks / strafe_ticks ) * 100.F );
	else
		m_sync = 0.F;

	if ( ( m_distance >= 200.F && m_distance <= 320.F ) && ( m_jump_position[ 1 ][ 2 ] - m_jump_position[ 0 ][ 2 ] ) <= 64.F &&
	     ( m_jump_position[ 1 ][ 2 ] - m_jump_position[ 0 ][ 2 ] ) >= -8.F ) {
		// bhop
		if ( m_hops == 1 && !m_jumpbug ) {
			m_height -= 3.2F;
			m_distance += 2.7F;

			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				io::format( XOR( "<<<NO_TRANSLATE>>> <font color=\"#fbf7f5\">movement</font><font color=\"#B9B9B9\"> | bhop: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font><font color=\"#B9B9B9\"> | strafes: </font><font "
			                     "color=\"#00ff00\">{}</font><font color=\"#B9B9B9\"> | speed: </font><font "
			                     "color=\"#00ff00\">{}/{}</font><font color=\"#B9B9B9\"> | height: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font>" ),
			                m_distance, m_strafes, m_pre, m_max, m_height )
					.c_str( ) );

			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "bhop: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f} " ), m_distance ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| strafes: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{} " ), m_strafes ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| speed: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{}/{} " ), m_pre, m_max ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| height: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f}\n" ), m_height ).c_str( ) );
		} else if ( m_hops > 1 && !m_jumpbug ) {
			m_height -= 3.5F;
			m_distance += 4.2F;
			m_height -= 3.2F;
			m_distance += 2.7F;

			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				io::format( XOR( "<<<NO_TRANSLATE>>> <font color=\"#fbf7f5\">movement</font><font color=\"#B9B9B9\"> | mbhop: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font><font color=\"#B9B9B9\"> | hops: </font><font "
			                     "color=\"#00ff00\">{}</font><font color=\"#B9B9B9\"> | strafes: </font><font "
			                     "color=\"#00ff00\">{}</font><font color=\"#B9B9B9\"> | speed: </font><font "
			                     "color=\"#00ff00\">{}/{}</font><font color=\"#B9B9B9\"> | height: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font>" ),
			                m_distance, m_hops, m_strafes, m_pre, m_max, m_height )
					.c_str( ) );

			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "mbhop: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f} " ), m_distance ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| hops: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{} " ), m_hops ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| strafes: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{} " ), m_strafes ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| speed: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{}/{} " ), m_pre, m_max ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| height: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f}\n" ), m_height ).c_str( ) );
		} else if ( m_jumpbug && m_hops == 0 ) {
			m_height -= 4.6F;
			m_distance += 1.4F;

			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				io::format( XOR( "<<<NO_TRANSLATE>>> <font color=\"#fbf7f5\">movement</font><font color=\"#B9B9B9\"> | jumpbug: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font><font color=\"#B9B9B9\"> | strafes: </font><font "
			                     "color=\"#00ff00\">{}</font><font color=\"#B9B9B9\"> | speed: </font><font "
			                     "color=\"#00ff00\">{}/{}</font><font color=\"#B9B9B9\"> | height: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font>" ),
			                m_distance, m_strafes, m_pre, m_max, m_height )
					.c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "jumpbug: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f} " ), m_distance ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| strafes: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{} " ), m_strafes ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| speed: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{}/{} " ), m_pre, m_max ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| height: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f}\n" ), m_height ).c_str( ) );
		} else if ( !m_jumpbug && m_hops == 0 ) {
			m_height -= 4.6F;
			m_distance += 4.6F;

			g_csgo.m_client_mode_shared->m_chat_element->chat_printf(
				0, 0,
				io::format( XOR( "<<<NO_TRANSLATE>>> <font color=\"#fbf7f5\">movement</font><font color=\"#B9B9B9\"> | longjump: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font><font color=\"#B9B9B9\"> | strafes: </font><font "
			                     "color=\"#00ff00\">{}</font><font color=\"#B9B9B9\"> | speed: </font><font "
			                     "color=\"#00ff00\">{}/{}</font><font color=\"#B9B9B9\"> | height: </font><font "
			                     "color=\"#00ff00\">{:.1f}</font>" ),
			                m_distance, m_strafes, m_pre, m_max, m_height )
					.c_str( ) );

			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "[" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 255, 255 ), XOR( "ecstasy.dev" ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), XOR( "] " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "longjump: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f} " ), m_distance ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| strafes: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{} " ), m_strafes ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| speed: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{}/{} " ), m_pre, m_max ).c_str( ) );
			g_csgo.m_cvars->console_color_printf( render::color( 185, 185, 185, 255 ), XOR( "| height: " ) );
			g_csgo.m_cvars->console_color_printf( render::color( 255, 255, 0, 255 ), io::format( XOR( "{:.1f}\n" ), m_height ).c_str( ) );
		}
	}

	reset( );

update:
	if ( !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
		if ( g_ctx.m_local.get( ).get_velocity( )[ 2 ] < m_jumpbug_z ) {
			m_jumpbug_z      = g_ctx.m_local.get( ).get_velocity( )[ 2 ];
			m_jumpbug_update = true;
		}

		if ( g_ctx.m_local.get( ).get_velocity( )[ 2 ] > m_jumpbug_z && m_jumpbug_update ) {
			m_strafes                = 0;
			m_strafe_left            = false;
			m_strafe_right           = false;
			m_duration_ticks_pending = 0;
			m_duration_ticks         = 0;
			m_pre                    = g_ctx.m_local.get( ).get_velocity( ).length_2d( );
			m_jump_position[ 0 ]     = g_ctx.m_local.get( ).get_abs_origin( );
			m_jumpbug                = true;
			m_jumpbug_update         = false;
		}
	} else if ( !m_jumpbug ) {
		m_jumpbug_update = false;
	}

	if ( ( g_ctx.m_local.get( ).get_flags( ) & 1 ) && ( g_ctx.m_cmd->m_buttons & 2 ) ) {
		m_hops++;
		m_jumpbug_z = 0;
	}

	if ( m_hops >= 0 && ( g_csgo.m_globals->m_tickcount - m_air_tick ) > 1 ) {
		m_hops = 0;
	}
}

void hack::movement::jumpstats::reset( ) {
	m_distance               = 0;
	m_pre                    = 0;
	m_max                    = 0;
	m_strafes                = 0;
	m_jump_origin            = { };
	m_jump_position[ 0 ]     = { };
	m_jump_position[ 1 ]     = { };
	m_height                 = 0;
	m_last_jumping           = false;
	m_strafe_left            = false;
	m_strafe_right           = false;
	m_calculate_height       = false;
	m_duration_ticks_pending = 0;
	m_duration_ticks         = 0;
	m_on_ground_tick         = 0;
	m_jumpbug                = false;
	m_jumpbug_z              = 0;
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
	m_jumpstats.reset( );
	m_in_minijump = false;
}