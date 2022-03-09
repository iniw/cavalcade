#include "backtrack.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"
#include "../../gui/cfg/cfg.hpp"
#include "../aimbot/autowall/autowall.hpp"

#define BACKTRACK_DEBUG 0

static void invalidate_bone_cache( sdk::cs_player* p ) {
	/*
	 * 55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56 83 E0 04 C6 44 24 ? ? 57 C1 E0 0A 8B F9
	 * v16 = g_ModelBoneCounter - 1;
	 * (_DWORD *)(result + 0x2928) = -8388609; -- FLT_MAX
	 * (_DWORD *)(result + 0x2690) = v16;
	 */

	static auto addy = g_mem[ CLIENT_DLL ].get_address< uintptr_t >( HASH_CT( "ModelBoneCounter" ) );

	*( float* )( ( uintptr_t )p + 0x2928 ) = -FLT_MAX;
	*( DWORD* )( ( uintptr_t )p + 0x2690 ) = ( **( ( u32** )( addy + 10 ) ) ) - 1u;
}

static void create_bones( sdk::cs_player* p, math::matrix_3x4* matrix, i32 maxbones, i32 flags, f32 time ) {
	/*p->occlusion_flags( )      = 0;
	p->occlusion_framecount( ) = 0;*/

	invalidate_bone_cache( p );

	p->setup_bones( matrix, maxbones, flags, time );
}

hack::backtrack::tick::tick( sdk::cs_player* p ) {
	if ( !p )
		return;

	auto model = g_csgo.m_model_info->get_studio_model( p->get_model( ) );

	if ( !model )
		return;

	m_old_simulation_time = p->get_old_sim_time( );
	m_simulation_time     = p->get_sim_time( );
	m_origin              = p->get_origin( );
	m_abs_angles          = p->get_abs_angles( );
	m_velocity            = p->get_velocity( );
	m_ang_eye_pos         = p->get_ang_eye_pos( );
	m_eflags              = p->get_eflags( );
	m_flags               = p->get_flags( );
	m_mins                = p->get_collideable( )->obb_mins( );
	m_maxs                = p->get_collideable( )->obb_maxs( );
	m_pose_parameters     = p->get_pose_parameter( );
	m_duck_amount         = p->get_duck_amount( );
	m_bone_count          = p->get_bone_count( );

	create_bones( p, m_matrix, 256, 0x7FF00, 0.F );
}
#include "../aimbot/aimbot.hpp"

f32 hack::backtrack::get_interpolation( ) {
	auto ratio = std::clamp( g_ctx.m_cvars.cl_interp_ratio->get_float( ), g_ctx.m_cvars.sv_client_min_interp_ratio->get_float( ),
	                         g_ctx.m_cvars.sv_client_max_interp_ratio->get_float( ) );
	return std::max( g_ctx.m_cvars.cl_interp->get_float( ), ( ratio / g_ctx.m_cvars.cl_updaterate->get_float( ) ) );
}

bool hack::backtrack::is_tick_valid( const tick& rec, f32 bt_time ) {
	auto info = g_csgo.m_engine->get_net_channel_info( );
	if ( !info )
		return false;

	// lost track
	if ( rec.m_simulation_time < rec.m_old_simulation_time )
		return false;

	auto correct = std::clamp( info->get_latency( 1 ) + get_interpolation( ), 0.F, g_ctx.m_cvars.sv_maxunlag->get_float( ) );

	auto delta_time = correct - ( g_csgo.m_globals->m_curtime - rec.m_simulation_time );

	return delta_time < bt_time;
}

void hack::backtrack::store_records( ) {
	static auto& bt  = gui::cfg::get< bool >( HASH_CT( "main:group1:backtrack" ) );
	static auto& btt = gui::cfg::get< f32 >( HASH_CT( "main:group1:backtrack time" ) );

	if ( !bt || btt == 0.F ) {
		clear( );
		return;
	}

	m_best_player = nullptr;
	m_best_tick   = std::numeric_limits< i32 >::min( );

	// wait for new tick
	static i32 old_tick = -1;
	if ( old_tick != g_csgo.m_globals->m_tickcount ) {
		old_tick = g_csgo.m_globals->m_tickcount;
	} else {
		return;
	}

	g_entity_cacher.for_each( [ & ]( auto& p ) {
		if ( !p )
			return;

		bool bother = false;
		if ( p.get( ).is_alive( ) && !p.get( ).is_dormant( ) && p.get( ).is_enemy( g_ctx.m_local ) ) {
			auto& rec = m_records[ p.get( ).get_networkable_index( ) ];

			// ensures simtime and at least 1 entry
			rec.emplace_front( p );

			// NOTE(para): we dont include current tick in the size check, and I think this is somewhat
			// arbitrary
			auto ticks_await = static_cast< i32 >( std::ceil( ( 1.f / g_csgo.m_globals->m_interval_per_tick ) / 5 ) ) - 1;

			// update records
			if ( !is_tick_valid( rec.back( ), btt ) || rec.size( ) > ticks_await )
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

void hack::backtrack::apply_record( sdk::cs_player* p, i32 record, bool set_tickcount ) {
	const auto& rec = m_records[ p->get_networkable_index( ) ];
	if ( record != std::numeric_limits< i32 >::min( ) && rec[ record ].m_simulation_time && ( g_ctx.m_cmd->m_buttons & ( 1 << 0 ) ) &&
	     g_ctx.m_local.get( ).can_fire_shot( ) ) {
		const auto& frec = rec[ record ];
		auto& bones      = p->get_cached_bones( );

		p->get_origin( ) = frec.m_origin;
		p->set_abs_origin( frec.m_origin );
		p->set_abs_angles( frec.m_abs_angles );
		p->get_collideable( )->obb_mins( ) = frec.m_mins;
		p->get_collideable( )->obb_maxs( ) = frec.m_maxs;
		p->get_ang_eye_pos( )              = frec.m_ang_eye_pos;
		p->get_pose_parameter( )           = frec.m_pose_parameters;
		memcpy( ( void* )bones.begin( ), frec.m_matrix, 48 * frec.m_bone_count );
		p->get_bone_count( )  = frec.m_bone_count;
		p->get_velocity( )    = frec.m_velocity;
		p->get_eflags( )      = frec.m_eflags;
		p->get_flags( )       = frec.m_flags;
		p->get_duck_amount( ) = frec.m_duck_amount;
		if ( set_tickcount ) {
			#if BACKTRACK_DEBUG == 1
			g_csgo.m_cvars->console_color_printf( render::color( 0, 255, 0, 255 ), io::format( "backtracked {} ticks\n", record ).c_str( ) );
			#endif
			g_ctx.m_cmd->m_tick_count = sdk::time_to_ticks( frec.m_simulation_time + get_interpolation( ) );
		}
	}
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

		auto& rec = m_records[ p.get( ).get_networkable_index( ) ];
		if ( rec.empty( ) )
			return;

		auto dis = std::numeric_limits< f32 >::max( );

		auto set_dis = [ & ]( sdk::e_hitbox h ) {
			auto hitbox_pos = p.get( ).get_hitbox_position( h, rec.front( ).m_matrix );
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

		apply_record( m_best_player, m_best_tick, true );
	}
}

void hack::backtrack::clear( ) {
	m_records.clear( );
}