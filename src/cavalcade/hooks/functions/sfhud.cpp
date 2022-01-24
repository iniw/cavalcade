#include "../hooks.hpp"

bool cavalcade::hooks::sfhud::weapon_reticle_knife_show( unk ecx, unk edx ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< fn >( HASH_CT( "WeaponReticleKnifeShow" ) );

	auto run = og( ecx, edx );

	if ( g_ctx.m_local.valid( ) ) {
		// crosshairstyle <= 1 on AWP
		auto handle = g_ctx.m_local.get( ).get_active_weapon( );
		if ( auto ent = g_csgo.m_ent_list->get_handle< sdk::weapon_cs_base* >( handle ); !g_ctx.m_local.get( ).is_scoped( ) && ent ) {
			auto wi = ent->get_cs_weapon_info( );
			if ( auto ty = wi->m_type; wi && ty == sdk::weapon_type::SNIPER ) {
				run = !run;
			}
		}
	}

	return run;
}