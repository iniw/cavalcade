#include "../hooks.hpp"

bool __fastcall cavalcade::hooks::crosshair::should_draw( unk ecx, unk edx ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< should_draw_fn >( HASH_CT( "CrosshairShouldDraw" ) );

	bool restore            = false;
	sdk::cs_weapon_info* wi = nullptr;
	if ( g_ctx.m_local.valid( ) ) {
		// crosshairstyle > 1 on AWP
		auto handle = g_ctx.m_local.get( ).get_active_weapon( );
		if ( auto ent = g_csgo.m_ent_list->get_handle< sdk::weapon_cs_base* >( handle ); !g_ctx.m_local.get( ).is_scoped( ) && ent ) {
			wi = ent->get_cs_weapon_info( );
			if ( auto ty = wi->m_type; wi && ty == sdk::weapon_type::SNIPER ) {
				restore    = true;
				wi->m_type = sdk::weapon_type::PISTOL;
			} else
				return og( ecx, edx );
		} else
			return og( ecx, edx );
	} else
		return og( ecx, edx );

	auto ret = og( ecx, edx );
	if ( wi && restore )
		wi->m_type = sdk::weapon_type::SNIPER;

	return ret;
}