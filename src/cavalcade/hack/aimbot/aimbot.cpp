#include "aimbot.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"
#include "../../gui/cfg/cfg.hpp"

math::ang hack::aimbot::pixels_to_angle( float x, float y ) {
	float px = x * g_ctx.m_cvars.m_pitch->get_float( );
	float py = y * g_ctx.m_cvars.m_yaw->get_float( );

	return math::ang( -x, -y, 0.F ).clamp_angle( );
}

math::ang hack::aimbot::angle_to_pixels( const math::ang& angle ) {
	float x = angle.pitch / g_ctx.m_cvars.m_pitch->get_float( );
	float y = angle.yaw / g_ctx.m_cvars.m_yaw->get_float( );

	return math::ang( -y, x, 0.F );
}

static auto distance_point_to_line( const math::v3f& point, const math::v3f& line_origin, const math::v3f& direction ) {
	auto point_direction = point - line_origin;
	const auto temp      = point_direction.dot_product( direction ) / direction.length_sqr( );
	if ( temp < 0.000001f )
		return FLT_MAX;

	const auto perpendicular_point = line_origin + direction * temp;
	return ( point - perpendicular_point ).length( );
}

void hack::aimbot::run( float& x, float& y ) {
	if ( g_ctx.m_local && g_ctx.m_local.get( ).is_alive( ) ) {
		if ( auto weap = g_csgo.m_ent_list->get_handle< sdk::weapon_cs_base* >( g_ctx.m_local.get( ).get_active_weapon( ) ); weap )
			if ( auto info = weap->get_cs_weapon_info( ); info ) {
				if ( info->m_max_clip1 <= 0 || info->m_type == sdk::weapon_type::KNIFE ||
				     ( info->m_type >= sdk::weapon_type::C4 && info->m_type < sdk::weapon_type::MELEE ) )
					return;
			} else
				return;
		else
			return;

		sdk::cs_player* best_player = nullptr;
		auto best_fov               = std::numeric_limits< float >::max( );

		static int& smooth = gui::cfg::get< i32 >( HASH_CT( "main:group1:smoothing" ) );

		auto local_pos = g_ctx.m_local.get( ).get_eye_position( );
		auto rcs_angle = g_ctx.m_local.get( ).get_aim_punch_angle( ) * 2;

		// TODO(para): target selection
		g_entity_cacher.for_each( [ & ]( auto& p ) {
			if ( !p )
				return;

			if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || p.get( ).is_dormant( ) || !p.get( ).is_enemy( g_ctx.m_local ) ||
			     p.get( ).is_immune( ) )
				return;

			auto hitbox_pos = p.get( ).get_hitbox_position( sdk::e_hitbox::HEAD );
			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

			if ( m_rcs )
				aim_angle -= rcs_angle;

			auto dis = distance_point_to_line( hitbox_pos, local_pos, *( math::v3f* )&aim_angle );
			if ( best_fov > dis ) {
				best_fov    = dis;
				best_player = p;
			}
		} );

		if ( best_player ) {
			auto hitbox_pos = best_player->get_hitbox_position( sdk::e_hitbox::HEAD );
			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

			if ( m_rcs )
				aim_angle -= rcs_angle;

			auto view_angles = g_csgo.m_engine->get_view_angles( );
			auto _view_delta = ( aim_angle - view_angles ).clamp_angle( );
			auto view_delta  = *( math::v3f* )&_view_delta;
			if ( smooth > 1 ) {
				auto move_ang = pixels_to_angle( x, y );

				move_ang /= smooth * 4;

				float delta_x = abs( move_ang.pitch );
				float delta_y = abs( move_ang.yaw );

				math::ang delta{ 0.f, 0.f, 0.f };
				delta.pitch = std::clamp( view_delta[ 0 ], -delta_x, delta_x );
				delta.yaw   = std::clamp( view_delta[ 1 ], -delta_y, delta_y );

				auto pixels = angle_to_pixels( delta );

				if ( view_delta.length_2d( ) <= m_fov ) {
					x += pixels.pitch;
					y += pixels.yaw;
				}

			} else {
				auto position = angle_to_pixels( _view_delta );

				x = position.pitch;
				y = position.yaw;
			}
		}
	}
}