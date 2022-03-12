#include "hooks.hpp"

#define PANORAMA_DEBUG 1

bool cavalcade::hooks::init( ) {
	MOCKING_TRY;

	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::FrameStageNotify" ), &chlc_client::frame_stage_notify );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::LevelInitPreEntity" ), &chlc_client::level_init_pre_entity );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CHLClient::LevelInitPostEntity" ), &chlc_client::level_init_post_entity );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "OnOverrideMouseInput" ), &chlc_client::on_override_mouse_input );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "PushNotice" ), &push_notice_::push_notice );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "ReportHit" ), &protobuf::report_hit );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CEngineClient::ClientCmd" ), &client_cmd_::client_cmd );
	// MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CPrediction::SetupMove" ), &prediction::setup_move );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "SetViewAngles" ), &engine_client::set_view_angles );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CreateMove" ), &base_player::create_move );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CEntityListener::OnAddEntity" ), &entity_listener::on_add_entity );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CBaseAnimating::SetupBones" ), &base_animating::setup_bones );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CEntityListener::OnRemoveEntity" ), &entity_listener::on_remove_entity );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "WeaponReticleKnifeShow" ), &sfhud::weapon_reticle_knife_show );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CrosshairShouldDraw" ), &crosshair::should_draw );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CCSGOPlayerAnimState::ModifyEyePosition" ), &csgo_player_anim_state::modify_eye_position );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "PostScreenFx" ), &fx::post_screen_fx );
	MOCK g_mem[ ENGINE_DLL ].hook( HASH_CT( "StaticPropMgrPrecacheLighting" ), &static_prop_mgr::precache_lighting );
	MOCK g_mem[ ENGINE_DLL ].hook( HASH_CT( "EmitSound" ), &engine_sound::emit_sound );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "RenderView" ), &view_render::render_view );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "GayFunction" ), &view_render::gay_function );
	// MOCK g_mem[ MATERIALSYSTEM_DLL ].hook( HASH_CT( "FindMaterial" ), &material_system::find_material );
	MOCK g_mem[ MATERIALSYSTEM_DLL ].hook( HASH_CT( "GetColorModulation" ), &material_system::get_color_modulation );
	MOCK g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::EndScene" ), &d3d9_device::end_scene );
	MOCK g_mem[ SHADERAPIDX9_DLL ].hook( HASH_CT( "IDirect3DDevice9::Reset" ), &d3d9_device::reset );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "CRadar::Initialize" ), &panorama::radar_init );
	MOCK g_mem[ CLIENT_DLL ].hook( HASH_CT( "SomeFn" ), &base_player::some_fn );
	MOCK g_mem[ PHYSICS_DLL ].hook( HASH_CT( "GetVCollide" ), &vphysics::get_collide );
	MOCK g_mem[ STUDIORENDER_DLL ].hook( HASH_CT( "DrawModel" ), &studio_render::draw_model );

	MOCKING_CATCH( return false );

	return true;
}