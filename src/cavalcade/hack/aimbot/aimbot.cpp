#include "aimbot.hpp"
#include "../../ctx/ctx.hpp"
#include "../../entity_cacher/entity_cacher.hpp"
#include "../../gui/cfg/cfg.hpp"

const static std::map< hack::aimbot::e_hitboxes, std::vector< sdk::e_hitbox > > g_aim_hitboxes = {
	{ hack::aimbot::e_hitboxes::HEAD, { sdk::e_hitbox::HEAD } },

	{ hack::aimbot::e_hitboxes::NECK, { sdk::e_hitbox::NECK } },

	{ hack::aimbot::e_hitboxes::BODY, { sdk::e_hitbox::BELLY, sdk::e_hitbox::THORAX, sdk::e_hitbox::LOWER_CHEST, sdk::e_hitbox::UPPER_CHEST } },

	{ hack::aimbot::e_hitboxes::ARMS,
	  { sdk::e_hitbox::LEFT_HAND, sdk::e_hitbox::RIGHT_HAND, sdk::e_hitbox::RIGHT_UPPER_ARM, sdk::e_hitbox::RIGHT_FOREARM,
	    sdk::e_hitbox::LEFT_UPPER_ARM, sdk::e_hitbox::LEFT_FOREARM } },

	{ hack::aimbot::e_hitboxes::LEGS,
	  { sdk::e_hitbox::LEFT_THIGH, sdk::e_hitbox::RIGHT_THIGH, sdk::e_hitbox::LEFT_CALF, sdk::e_hitbox::RIGHT_CALF, sdk::e_hitbox::LEFT_FOOT,
	    sdk::e_hitbox::RIGHT_FOOT } }
};

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

math::v3f angle_vectors( const math::ang& angles ) {
	constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };

	f32 sp{ }, sy{ }, cp{ }, cy{ };

	sy = sin( DEG2RAD( angles.yaw ) );
	cy = cos( DEG2RAD( angles.yaw ) );

	sp = sin( DEG2RAD( angles.pitch ) );
	cp = cos( DEG2RAD( angles.pitch ) );

	return { cp * cy, cp * sy, -sp };
}

static auto get_fov( const math::ang& view, const math::ang& aim ) {
	constexpr auto RAD2DEG = []( const f32 x ) -> f32 { return x * ( 180.f / M_PI ); };

	math::v3f _aim = angle_vectors( view );
	math::v3f _ang = angle_vectors( aim );

	return RAD2DEG( acos( _aim.dot_product( _ang ) / _aim.length_sqr( ) ) );
}

void hack::aimbot::run( f32& x, f32& y ) {
	m_aiming = false;
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

		static auto& fov       = gui::cfg::get< i32 >( HASH_CT( "main:group1:fov" ) );
		static auto& smooth    = gui::cfg::get< i32 >( HASH_CT( "main:group1:smoothing" ) );
		static auto& on_attack = gui::cfg::get< bool >( HASH_CT( "main:group1:on attack" ) );

		auto local_pos   = g_ctx.m_local.get( ).get_eye_position( );
		auto rcs_angle   = g_ctx.m_local.get( ).get_aim_punch_angle( ) * 2;
		auto view_angles = g_csgo.m_engine->get_view_angles( );

		m_best_player = nullptr;
		m_best_fov    = fov;

		g_entity_cacher.for_each( [ & ]( auto& p ) {
			if ( !p )
				return;

			if ( p == g_ctx.m_local || !p.get( ).is_alive( ) || p.get( ).is_dormant( ) || !p.get( ).is_enemy( g_ctx.m_local ) ||
			     p.get( ).is_immune( ) )
				return;

			auto hitbox_pos = p.get( ).get_hitbox_position( sdk::e_hitbox::HEAD );
			if ( hitbox_pos ==
			         math::v3f{ std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) } ||
			     hitbox_pos == math::v3f{ 0, 0, 0 } )
				return;
			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );
			if ( m_rcs )
				aim_angle -= rcs_angle;

			auto dis = get_fov( view_angles, aim_angle );
			if ( m_best_fov > dis ) {
				m_best_player = p;
				m_best_fov    = dis;
			}
		} );

		if ( on_attack && !g_io.key_state< io::key_state::DOWN >( VK_LBUTTON ) )
			return;

		if ( m_best_player ) {
			auto best_hitbox = [ & ]( ) -> math::v3f {
				auto best_fov    = fov;
				auto best_hitbox = sdk::e_hitbox::HEAD;

				for ( const auto& [ k, e ] : g_aim_hitboxes ) {
					// TODO(para, wini (menu)): verify if k selected, if not, continue

					for ( auto h : e ) {
						auto hitbox_pos = m_best_player->get_hitbox_position( h );
						if ( hitbox_pos == math::v3f{ std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ),
						                              std::numeric_limits< f32 >::max( ) } ||
						     hitbox_pos == math::v3f{ 0, 0, 0 } )
							continue;
						auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
						auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

						if ( m_rcs )
							aim_angle -= rcs_angle;

						auto dis = get_fov( view_angles, aim_angle );
						if ( m_best_fov > dis ) {
							best_hitbox = h;
							best_fov    = dis;
						}
					}
				}

				return m_best_player->get_hitbox_position( best_hitbox );
			};

			auto hitbox_pos = best_hitbox( );
			auto _aim_angle = local_pos.calculate_angle( hitbox_pos );
			auto aim_angle  = ( *( math::ang* )&_aim_angle ).clamp_angle( );

			if ( m_rcs )
				aim_angle -= rcs_angle;

			auto _view_delta = ( aim_angle - view_angles ).clamp_angle( );
			auto view_delta  = *( math::v3f* )&_view_delta;
			m_aiming         = true;

			if ( smooth > 1 ) {
				auto move_ang = pixels_to_angle( x, y );

				move_ang /= smooth * 2;

				f32 delta_x = abs( move_ang.pitch );
				f32 delta_y = abs( move_ang.yaw );

				math::ang delta{ 0.f, 0.f, 0.f };
				delta.pitch = std::clamp( view_delta[ 0 ], -delta_x, delta_x );
				delta.yaw   = std::clamp( view_delta[ 1 ], -delta_y, delta_y );

				auto pixels = angle_to_pixels( delta );

				x += pixels.pitch;
				y += pixels.yaw;

			} else {
				auto position = angle_to_pixels( _view_delta );

				x = position.pitch;
				y = position.yaw;
			}
		}
	}
}