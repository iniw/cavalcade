#include "../hooks.hpp"

void cavalcade::hooks::studio_render::draw_model( sdk::studio_render_context* ecx, unk edx, unk results, const sdk::draw_model_info& info,
                                                  math::matrix_3x4* matrix, f32* flex_weights, f32* flex_delayed_rates, const math::v3f& origin,
                                                  i32 flags ) {
	static auto og = g_mem[ STUDIORENDER_DLL ].get_og< draw_model_fn >( HASH_CT( "DrawModel" ) );

	if ( !g_ctx.m_local || !info.m_client_entity || ecx->is_forced_material_override( ) || ecx->is_weapon( ) )
		return og( ecx, edx, results, info, matrix, flex_weights, flex_delayed_rates, origin, flags );
	static auto& ch   = gui::cfg::get< bool >( HASH_CT( "main:group1:chams" ) );
	static auto& cbat = gui::cfg::get< bool >( HASH_CT( "main:group1:chams bt alltick" ) );
	if ( !ch )
		return og( ecx, edx, results, info, matrix, flex_weights, flex_delayed_rates, origin, flags );

	sdk::base_entity* ent = ( sdk::base_entity* )( info.m_client_entity - 4 );
	if ( ent && ent->is_player( ) && ( ( sdk::cs_player* )( ent ) )->is_enemy( g_ctx.m_local ) ) {
		if ( g_hack.m_backtrack.m_records.contains( ent->get_networkable_index( ) ) ) {
			const auto& rec = g_hack.m_backtrack.m_records[ ent->get_networkable_index( ) ];
			if ( !rec.empty( ) ) {
				if ( cbat ) {
					for ( const auto& entry : rec ) {
						ecx->set_color( render::color( 255, 255, 255 ) );
						ecx->set_alpha( 0.3F );

						static auto mat = g_csgo.m_material_system->find_material( XOR( "debug/debugdrawflat" ), XOR( "Model textures" ) );
						ecx->set_material( mat );

						og( ecx, edx, results, info, ( math::matrix_3x4* )entry.m_matrix, flex_weights, flex_delayed_rates, entry.m_origin, flags );

						/*
						if no vis material
						ecx->set_color( render::color( 255, 255, 255 ) );
						ecx->set_alpha( 1.F );

						ecx->set_material( nullptr, false );
						*/

						ecx->set_color( render::color( 122, 178, 200 ) );
						ecx->set_alpha( 0.3F );
						ecx->set_material( mat, false );
						og( ecx, edx, results, info, ( math::matrix_3x4* )entry.m_matrix, flex_weights, flex_delayed_rates, entry.m_origin, flags );
						ecx->set_material( nullptr );
					}
				} else {
					const auto& entry = rec.back( );
					ecx->set_color( render::color( 255, 255, 255 ) );
					ecx->set_alpha( 0.3F );

					static auto mat = g_csgo.m_material_system->find_material( XOR( "debug/debugdrawflat" ), XOR( "Model textures" ) );
					ecx->set_material( mat );

					og( ecx, edx, results, info, ( math::matrix_3x4* )entry.m_matrix, flex_weights, flex_delayed_rates, entry.m_origin, flags );

					/*
					if no vis material
					ecx->set_color( render::color( 255, 255, 255 ) );
					ecx->set_alpha( 1.F );

					ecx->set_material( nullptr, false );
					*/

					ecx->set_color( render::color( 122, 178, 200 ) );
					ecx->set_alpha( 0.3F );
					ecx->set_material( mat, false );
					og( ecx, edx, results, info, ( math::matrix_3x4* )entry.m_matrix, flex_weights, flex_delayed_rates, entry.m_origin, flags );
					ecx->set_material( nullptr );
				}
			}
		}

		ecx->set_color( render::color( 255, 255, 255 ) );
		ecx->set_alpha( 0.3F );

		static auto mat = g_csgo.m_material_system->find_material( XOR( "debug/debugdrawflat" ), XOR( "Model textures" ) );
		ecx->set_material( mat );

		og( ecx, edx, results, info, matrix, flex_weights, flex_delayed_rates, origin, flags );

		/*
		if no vis material
		ecx->set_color( render::color( 255, 255, 255 ) );
		ecx->set_alpha( 1.F );

		ecx->set_material( nullptr, false );
		*/

		ecx->set_color( render::color( 122, 178, 200 ) );
		ecx->set_alpha( 0.3F );
		ecx->set_material( mat, false );
		og( ecx, edx, results, info, matrix, flex_weights, flex_delayed_rates, origin, flags );
		ecx->set_material( nullptr );
		return;
	}

	og( ecx, edx, results, info, matrix, flex_weights, flex_delayed_rates, origin, flags );
}