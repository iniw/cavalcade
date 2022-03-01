#include "backtrack.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"
#include "../../gui/cfg/cfg.hpp"
#include "../aimbot/autowall/autowall.hpp"

hack::backtrack::tick::tick( sdk::cs_player* p ) {
	m_simulation_time = p->get_sim_time( );

	for ( auto i = 0; i < 256; ++i ) {
		m_matrix[ i ] = p->get_cached_bone_matrix( i );
	}
}
#include "../aimbot/aimbot.hpp"

static f32 get_interpolation( ) {
	auto updaterate = std::clamp( g_ctx.m_cvars.cl_updaterate->get_float( ), g_ctx.m_cvars.sv_minupdaterate->get_float( ),
	                              g_ctx.m_cvars.sv_maxupdaterate->get_float( ) );
	auto lerp_ratio = std::clamp( g_ctx.m_cvars.cl_interp_ratio->get_float( ), g_ctx.m_cvars.sv_client_min_interp_ratio->get_float( ),
	                              g_ctx.m_cvars.sv_client_max_interp_ratio->get_float( ) );

	return std::clamp( lerp_ratio / updaterate, g_ctx.m_cvars.cl_interp->get_float( ), 1.0f );
}

bool hack::backtrack::is_tick_valid( f32 sim_time, f32 bt_time ) {
	auto info = g_csgo.m_engine->get_net_channel_info( );
	if ( !info )
		return false;

	auto correct = std::clamp( info->get_latency( 1 ) + info->get_latency( 0 ) + get_interpolation( ), 0.F, g_ctx.m_cvars.sv_maxunlag->get_float( ) );

	auto delta_time = correct - ( g_csgo.m_globals->m_curtime - sim_time );

	static auto& btt = gui::cfg::get< f32 >( HASH_CT( "main:group1:backtrack time" ) );

	if ( fabs( delta_time ) > btt )
		return false;

	return true;
}

void hack::backtrack::store_records( ) {
	if ( !g_ctx.m_local.get( ).is_alive( ) ) {
		clear( );

		return;
	}

	static auto& bt  = gui::cfg::get< bool >( HASH_CT( "main:group1:backtrack" ) );
	static auto& btt = gui::cfg::get< f32 >( HASH_CT( "main:group1:backtrack time" ) );

	m_best_player = nullptr;
	m_best_tick   = std::numeric_limits< i32 >::min( );

	if ( !bt || btt == 0.F )
		return;

	g_entity_cacher.for_each( [ & ]( auto& p ) {
		if ( !p )
			return;

		bool bother = false;
		if ( p.get( ).is_alive( ) && !p.get( ).is_dormant( ) && p.get( ).is_enemy( g_ctx.m_local ) ) {
			auto& rec = m_records[ p.get( ).get_networkable_index( ) ];

			// ensures simtime and at least 1 entry
			rec.emplace_front( p );

			// update records
			if ( !is_tick_valid( rec.back( ).m_simulation_time.value( ), btt ) )
				rec.pop_back( );
		} else {
			bother = true;
		}

		if ( bother && ( !p.get( ).is_alive( ) || p.get( ).is_dormant( ) || !p.get( ).is_enemy( g_ctx.m_local ) ) ) {
			if ( m_records.contains( p.get( ).get_networkable_index( ) ) ) {
				m_records[ p.get( ).get_networkable_index( ) ].clear( );
			}
		}
	} );
}

void hack::backtrack::run( ) {
	if ( !g_ctx.m_cmd )
		return;

	static auto& bt  = gui::cfg::get< bool >( HASH_CT( "main:group1:backtrack" ) );
	static auto& btt = gui::cfg::get< f32 >( HASH_CT( "main:group1:backtrack time" ) );

	if ( !bt || btt == 0.F )
		return;

	static auto& rcs = gui::cfg::get< bool >( HASH_CT( "main:group1:rcs" ) );

	sdk::weapon_cs_base* weap = nullptr;
	sdk::cs_weapon_info* info = nullptr;
	if ( g_ctx.m_local.valid( ) && g_ctx.m_local.get( ).is_alive( ) ) {
		if ( weap = g_csgo.m_ent_list->get_handle< sdk::weapon_cs_base* >( g_ctx.m_local.get( ).get_active_weapon( ) ); weap )
			if ( info = weap->get_cs_weapon_info( ); info ) {
				if ( info->m_max_clip1 <= 0 || info->m_type == sdk::weapon_type::KNIFE ||
				     ( info->m_type >= sdk::weapon_type::C4 && info->m_type < sdk::weapon_type::MELEE ) )
					return;
			} else
				return;
		else
			return;
	}

	auto rcs_angle   = g_ctx.m_local.get( ).get_aim_punch_angle( ) * 2;
	auto view_angles = g_csgo.m_engine->get_view_angles( );

	f32 best_fov   = std::numeric_limits< f32 >::max( );
	auto local_pos = g_ctx.m_local.get( ).get_eye_position( );

	g_entity_cacher.for_each( [ & ]( auto& p ) {
		if ( !p.valid( ) )
			return;

		if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || p.get( ).is_dormant( ) || !p.get( ).is_enemy( g_ctx.m_local ) || p.get( ).is_immune( ) )
			return;

		auto dis = std::numeric_limits< f32 >::max( );

		auto set_dis = [ & ]( sdk::e_hitbox h ) {
			auto hitbox_pos = p.get( ).get_hitbox_position( h, m_records[ p.get( ).get_networkable_index( ) ].front( ).m_matrix );
			if ( hitbox_pos ==
			         math::v3f{ std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) } ||
			     hitbox_pos == math::v3f{ 0, 0, 0 } )
				return;

			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

			if ( rcs )
				aim_angle -= rcs_angle;

			auto fov = hack::aimbot::get_fov( view_angles, aim_angle );
			if ( fov < dis )
				dis = fov;
		};

		set_dis( sdk::e_hitbox::HEAD );
		set_dis( sdk::e_hitbox::BELLY );
		set_dis( sdk::e_hitbox::THORAX );
		set_dis( sdk::e_hitbox::LOWER_CHEST );
		set_dis( sdk::e_hitbox::UPPER_CHEST );

		if ( best_fov > dis ) {
			m_best_player = p;
			best_fov      = dis;
		}
	} );

	auto best_tick_pov = std::numeric_limits< f32 >::max( );
	if ( m_best_player ) {
		const auto& rec = m_records[ m_best_player->get_networkable_index( ) ];
		for ( auto t = 0; t < rec.size( ); ++t ) {
			auto dis = std::numeric_limits< f32 >::max( );

			auto set_dis = [ & ]( sdk::e_hitbox h ) {
				auto hitbox_pos = m_best_player->get_hitbox_position( h, rec[ t ].m_matrix );
				if ( hitbox_pos ==
				         math::v3f{ std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) } ||
				     hitbox_pos == math::v3f{ 0, 0, 0 } )
					return;

				auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
				auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

				if ( rcs )
					aim_angle -= rcs_angle;

				auto fov = hack::aimbot::get_fov( view_angles, aim_angle );
				if ( dis > fov ) {
					// f32 dmg   = 0;
					// auto scan = autowall::can_hit( m_best_player, weap, info, hitbox_pos, dmg );
					// if ( !scan ) {
					//	// TODO(para): verify other hitboxes in accordance to config too
					//	return;
					// }

					dis = fov;
				}
			};

			set_dis( sdk::e_hitbox::HEAD );
			set_dis( sdk::e_hitbox::BELLY );
			set_dis( sdk::e_hitbox::THORAX );
			set_dis( sdk::e_hitbox::LOWER_CHEST );
			set_dis( sdk::e_hitbox::UPPER_CHEST );

			if ( best_tick_pov > dis ) {
				best_tick_pov = dis;
				m_best_tick   = t;
			}
		}
		if ( m_best_tick != std::numeric_limits< i32 >::min( ) && rec[ m_best_tick ].m_simulation_time.has_value( ) &&
		     ( g_ctx.m_cmd->m_buttons & ( 1 << 0 ) ) )
			g_ctx.m_cmd->m_tick_count = sdk::time_to_ticks( rec[ m_best_tick ].m_simulation_time.value( ) );
	}
}

void hack::backtrack::clear( ) {
	m_records.clear( );
}