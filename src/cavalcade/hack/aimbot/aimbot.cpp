#include "aimbot.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"

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

		auto local_pos = g_ctx.m_local.get( ).get_eye_position( );
		// TODO(para): target selection
		g_entity_cacher.for_each( [ & ]( auto& p ) {
			if ( !p )
				return;

			if ( !p.get( ).is_alive( ) || p.get( ).is_dormant( ) || !p.get( ).is_enemy( g_ctx.m_local ) || p.get( ).is_immune( ) )
				return;

			auto hitbox_pos = p.get( ).get_hitbox_position( sdk::e_hitbox::HEAD );
			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

			if ( m_rcs )
				aim_angle -= g_ctx.m_local.get( ).get_aim_punch_angle( ) * 2.F;

			auto move_ang = pixels_to_angle( x, y );

			auto view_angles = g_csgo.m_engine->get_view_angles( );

			// TODO(anyone): unfuck ang/vec mess.
			auto _view_delta = ( aim_angle - view_angles ).clamp_angle( );
			auto view_delta  = *( math::v3f* )&_view_delta;

			move_ang /= m_smoothing;

			float delta_x = abs( move_ang.pitch );
			float delta_y = abs( move_ang.yaw );

			math::ang delta{ 0.f, 0.f, 0.f };
			delta.pitch = std::clamp( view_delta[ 0 ], -delta_x, delta_x );
			delta.yaw   = std::clamp( view_delta[ 1 ], -delta_y, delta_y );

			// TODO: scale delta to distance

			auto pixels = angle_to_pixels( delta );
			if ( view_delta.length_2d( ) <= m_fov ) {
				x += pixels.pitch;
				y += pixels.yaw;
			}
		} );
	}
}