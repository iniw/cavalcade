#include "../hack.hpp"
#include "movement.hpp"
#include "../../ctx/ctx.hpp"

static bool local_on_ladder_or_noclip( ) {
	return g_ctx.m_local &&
	       ( g_ctx.m_local.get( ).get_move_type( ) == sdk::move_type::LADDER || g_ctx.m_local.get( ).get_move_type( ) == sdk::move_type::NOCLIP );
}

void hack::movement::pre( ) {
	m_jumpbugged     = false;
	m_longjumped     = false;
	m_old_velocity_z = g_ctx.m_local.get( ).get_velocity( )[ 2 ];
	m_base_flags     = g_ctx.m_local.get( ).get_flags( );

	no_duck_delay( );
	bunnyhop( );
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
	if ( !( true && g_io.key_state< io::key_state::DOWN >( VK_XBUTTON2 ) ) || local_on_ladder_or_noclip( ) ) {
		m_lj_ducked_ticks = 0;
		return;
	}

	if ( !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) {
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
	}
}

void hack::movement::post( ) {
	jumpbug( );
	longjump( );

	// NOTE(para) : I know at least second last check is pedantic
	m_jumpbugged = ( g_ctx.m_local.get( ).get_velocity( )[ 2 ] > m_old_velocity_z ) &&
	               ( !g_ctx.m_local.get( ).get_ground_entity( ).get( ) && !( m_base_flags & 1 ) && !( g_ctx.m_local.get( ).get_flags( ) & 1 ) ) &&
	               !local_on_ladder_or_noclip( );
}