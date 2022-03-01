#include "../../cavalcade/ctx/ctx.hpp"

bool sdk::cs_player::is_enemy( sdk::base_player* player ) {
	if ( g_ctx.m_cvars.mp_teammates_are_enemies->get_int( ) )
		return this != player;

	return get_team( ) != player->get_team( );
}

bool sdk::cs_player::is_other_enemy( base_player* other ) {
	static auto is_other_enemy_fn = g_mem[ CLIENT_DLL ].get_address< bool( __thiscall* )( cs_player*, base_player* ) >( HASH_CT( "IsOtherEnemy" ) );
	return is_other_enemy_fn( this, other );
}

void sdk::cs_player::post_think( ) {
	g_csgo.m_mdl_cache->begin_lock( );

	if ( is_alive( ) || is_player_ghost( ) ) { // cmp [edi+3A95h], al

		mem::call_v_func< void, 340 >( this ); // UpdateCollisionBounds

		if ( get_flags( ) & flags::ONGROUND )
			get_fall_velocity( ) = 0.f;

		if ( get_sequence( ) == -1 )
			mem::call_v_func< void, 219 >( this, 0 ); // SetSequence

		mem::call_v_func< void, 220 >( this ); // StudioFrameAdvance

		post_think_v_physics( );
	}

	simulate_player_simulated_entities( );

	g_csgo.m_mdl_cache->end_lock( );
}

math::v3f sdk::cs_player::get_hitbox_position( i32 hitbox_id, const math::matrix_3x4* m ) {
	auto model = g_csgo.m_model_info->get_studio_model( get_model( ) );

	if ( model && m ) {
		auto hitbox = model->get_hitbox_set( 0 )->get_hitbox( hitbox_id );

		if ( hitbox ) {
			auto vector_transform = [ & ]( const math::v3f& vec ) -> math::v3f {
				math::v3f r{ };
				for ( auto i = 0; i < 3; ++i )
					r[ i ] = vec.dot_product( m[ hitbox->m_bone ].data[ i ] ) + m[ hitbox->m_bone ].data[ i ][ 3 ];

				return r;
			};

			if ( hitbox->m_bone < 0 || hitbox->m_bone > 256 )
				return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };

			if ( hitbox->m_bb_min == math::v3f{ 0, 0, 0 } )
				return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };

			if ( hitbox->m_bb_max == math::v3f{ 0, 0, 0 } )
				return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };

			auto min = vector_transform( hitbox->m_bb_min );
			auto max = vector_transform( hitbox->m_bb_max );

			return ( min + max ) / 2.F;
		}
	}

	return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };
}

math::v3f sdk::cs_player::get_hitbox_position( i32 hitbox_id ) {
	auto model = g_csgo.m_model_info->get_studio_model( get_model( ) );

	if ( model ) {
		auto hitbox = model->get_hitbox_set( 0 )->get_hitbox( hitbox_id );

		if ( hitbox ) {
			auto vector_transform = [ & ]( const math::v3f& vec ) -> math::v3f {
				math::v3f r{ };
				for ( auto i = 0; i < 3; ++i )
					r[ i ] = vec.dot_product( get_cached_bone_matrix( hitbox->m_bone ).data[ i ] ) +
					         get_cached_bone_matrix( hitbox->m_bone ).data[ i ][ 3 ];

				return r;
			};

			if ( hitbox->m_bone < 0 || hitbox->m_bone > 256 )
				return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };

			if ( hitbox->m_bb_min == math::v3f{ 0, 0, 0 } )
				return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };

			if ( hitbox->m_bb_max == math::v3f{ 0, 0, 0 } )
				return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };

			auto min = vector_transform( hitbox->m_bb_min );
			auto max = vector_transform( hitbox->m_bb_max );

			return ( min + max ) / 2.F;
		}
	}

	return { std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ), std::numeric_limits< f32 >::max( ) };
}

math::v3f sdk::cs_player::get_eye_position( ) {
	math::v3f v;
	mem::call_v_func< void, 285 >( this, std::ref( v ) );
	return v;
}

bool sdk::cs_player::can_fire_shot( ) {
	if ( !this )
		return false;

	if ( !g_ctx.m_cmd || g_ctx.m_cmd->m_weapon_select )
		return false;

	auto server_time = sdk::ticks_to_time( g_ctx.m_local.get( ).get_tickbase( ) );

	auto weap = get_active_weapon( ).get< sdk::weapon_cs_base* >( );

	if ( !weap || !weap->get_ammo( ) )
		return false;

	if ( get_next_attack( ) > server_time )
		return false;

	if ( weap->get_next_primary_attack( ) > server_time )
		return false;

	return true;
}