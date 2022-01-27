#include "mem.hpp"

#include "../../sdk/csgo/csgo.hpp"

bool mem::impl::init( ) {
	const auto PEB = reinterpret_cast< win32::PEB* >( __readfsdword( 0x30 ) );

	MOCKING_TRY;

	MOCK PEB;
	MOCK PEB->Ldr->InMemoryOrderModuleList.Flink;

	MOCKING_CATCH( return false );

	auto list = &PEB->Ldr->InMemoryOrderModuleList;

	for ( auto it = list->Flink; it != list; it = it->Flink ) {
		auto ldr_entry = CONTAINING_RECORD( it, win32::LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );
		if ( !ldr_entry )
			continue;

		auto name = utils::str::to_lower( utils::str::wide_to_multi_byte( ldr_entry->BaseDllName.Buffer ) );

		m_modules[ HASH_RT( name ) ] = module_info( address( ldr_entry->DllBase ) );
	}

	g_io.log( XOR( "initialized modules" ) );

	return !m_modules.empty( );
}

bool mem::impl::setup( ) {
	add_addresses( );

	if ( !validate_addresses( ) )
		return false;

	g_io.log( XOR( "initialized addresses" ) );

	return true;
}

void mem::impl::unload( ) {
	for ( auto& [ k, v ] : m_modules )
		v.restore( );
}

void mem::impl::add_addresses( ) {
#define ADD_PATTERN_REL( module_name, addr_name, pattern ) m_modules[ module_name ].add_pattern( HASH_CT( addr_name ), STB( pattern ), true )
#define ADD_PATTERN( module_name, addr_name, pattern )     m_modules[ module_name ].add_pattern( HASH_CT( addr_name ), STB( pattern ) )
#define ADD_ADDRESS( module_name, addr_name, address )     m_modules[ module_name ].add_address( HASH_CT( addr_name ), address )

	{ // client.dll

		ADD_ADDRESS( CLIENT_DLL, "CHLClient::FrameStageNotify", get_v_func( g_csgo.m_client, 37 ) );
		ADD_PATTERN( CLIENT_DLL, "CHLClient::LevelInitPreEntity", "55 8B EC 83 E4 F8 83 EC 30 80 3D" );
		ADD_PATTERN( CLIENT_DLL, "CHLClient::LevelInitPostEntity", "55 8B EC 83 EC 10 53 56 8B 35 ? ? ? ? 57 FF D6 0F" );
		ADD_PATTERN( CLIENT_DLL, "PushNotice", "55 8B EC 83 E4 F8 B8 ? ? ? ? E8 ? ? ? ? 53 8B D9 8B 0D ? ? ? ? 56 57 81 F9 ? ? ? ? 75 0C A1" );
		ADD_ADDRESS( CLIENT_DLL, "CEngineClient::IsHLTV", get_v_func( g_csgo.m_engine, 93 ) );
		ADD_ADDRESS( CLIENT_DLL, "CEngineClient::ClientCmd", get_v_func( g_csgo.m_engine, 7 ) );
		ADD_PATTERN( CLIENT_DLL, "CEntityListener::OnAddEntity", "55 8B EC 51 8B 45 0C 53 56 8B F1 57" );
		ADD_PATTERN( CLIENT_DLL, "CEntityListener::OnRemoveEntity", "55 8B EC 51 8B 45 0C 53 8B D9 56 57 83 F8 FF 75 07" );
		ADD_PATTERN( CLIENT_DLL, "OnOverrideMouseInput", "55 8B EC 83 E4 F8 83 EC 18 56 8B 75 0C 57 8B 7D 10" );
		ADD_ADDRESS( CLIENT_DLL, "CPrediction::SetupMove", get_v_func( g_csgo.m_prediction, 20 ) );
		ADD_PATTERN( CLIENT_DLL, "TraceToExit", "55 8B EC 83 EC 4C F3 0F 10 75" );
		ADD_PATTERN_REL( CLIENT_DLL, "IsOtherEnemy", "E8 ? ? ? ? 02 C0 32 87 ? ? ? ? 24 02 30 87" );
		ADD_PATTERN( CLIENT_DLL, "IsBreakableEntity", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE" );
		ADD_PATTERN_REL( CLIENT_DLL, "ClipTraceToPlayers", "E8 ? ? ? ? 83 C4 18 8A 56 37" );
		ADD_PATTERN( CLIENT_DLL, "CrosshairShouldDraw", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 8B 0D" );
		ADD_PATTERN( CLIENT_DLL, "ReportHit", "55 8B EC 8B 55 08 83 EC 1C F6 42 1C 01" );
		ADD_ADDRESS( ENGINE_DLL, "EmitSound", get_v_func( g_csgo.m_engine_sound, 5 ) );
		ADD_PATTERN(
			ENGINE_DLL, "MaterialSystem",
			"8B 0D ? ? ? ? 6A 00 6A 01 68 ? ? ? ? 8B 01 68 ? ? ? ? FF 90 ? ? ? ? C3 85 C9 78 35 3B 0D ? ? ? ? 7D 2D A1 ? ? ? ? 8B 0C 88 85 C9" );
		auto addr            = m_modules[ ENGINE_DLL ].get_address< uintptr_t >( HASH_CT( "MaterialSystem" ) ) + 2;
		auto material_system = **( void*** )addr;
		ADD_ADDRESS( MATERIALSYSTEM_DLL, "FindMaterial", get_v_func( material_system, 84 ) );
		ADD_PATTERN_REL( ENGINE_DLL, "StaticPropMgrPrecacheLighting", "E8 ? ? ? ? FF 76 60" );

		ADD_PATTERN( MATERIALSYSTEM_DLL, "GetColorModulation", "55 8B EC 83 EC ? 56 8B F1 8A 46" );

		{ // C_BaseEntity

			ADD_PATTERN( CLIENT_DLL, "C_BaseEntity->m_vecAbsVelocity", "F3 0F 10 A6 ? ? ? ? F3 0F 11 64 24 ?" );
			ADD_PATTERN( CLIENT_DLL, "C_BaseEntity->m_nAnimOverlay", "8B 80 ? ? ? ? 03 C1 74 15" );
			ADD_PATTERN( CLIENT_DLL, "C_BaseEntity->m_nPredictionRandomSeed", "8B 47 40 A3 ? ? ? ?" );
			ADD_PATTERN( CLIENT_DLL, "C_BaseEntity->m_pPredictionPlayer", "0F 5B C0 89 35 ? ? ? ?" );

			ADD_PATTERN( CLIENT_DLL, "C_BaseEntity::SetAbsOrigin", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" );

			ADD_PATTERN_REL( CLIENT_DLL, "WeaponReticleKnifeShow", "E8 ? ? ? ? 84 C0 74 1C A1 ? ? ? ? " );

			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseEntity::SetAbsAngles", "E8 ? ? ? ? 53 8D 45 F0" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseEntity::CalcAbsoluteVelocity", "E8 ? ? ? ? 83 7B 30 00" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseEntity::GetGroundEntity", "E8 ? ? ? ? 8B 4C 24 14 85 C0" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseEntity::InvalidatePhysicsRecursive", "E8 ? ? ? ? 89 5E 18" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseEntity::PhysicsRunThink", "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? F3 0F 10 86 ? ? ? ?" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseEntity::CheckHasThinkFunction", "E8 ? ? ? ? EB 11 8B 86 ? ? ? ?" );
		}

		{ // C_BasePlayer

			ADD_PATTERN( CLIENT_DLL, "C_BasePlayer->m_LastCmd", "8D 8E ? ? ? ? 89 5C 24 3C" );
			ADD_PATTERN( CLIENT_DLL, "C_BasePlayer->m_afButtonForced", "8B 86 ? ? ? ? 09 47 30" );
			ADD_PATTERN( CLIENT_DLL, "C_BasePlayer->m_afButtonLast", "33 CA 89 86 ? ? ? ?" );

			ADD_PATTERN( CLIENT_DLL, "C_BasePlayer::IsLocalPlayer", "56 8B F1 85 F6 74 16" );
			ADD_PATTERN( CLIENT_DLL, "C_BasePlayer::SimulatePlayerSimulatedEntities", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" );
			ADD_PATTERN( CLIENT_DLL, "C_BasePlayer::InvalidateBoneCache", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" );

			ADD_PATTERN_REL( CLIENT_DLL, "C_BasePlayer::PostThinkVPhysics", "E8 ? ? ? ? 8B CF E8 ? ? ? ? 8B 06" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BasePlayer::CreateMove", "E8 ? ? ? ? FF 15 ? ? ? ? 84 C0 74 29" );
		}

		{ // C_BaseAnimating

			ADD_PATTERN( CLIENT_DLL, "C_BaseAnimating::ShouldSkipAnimationFrame", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02" );
			ADD_PATTERN( CLIENT_DLL, "C_BaseAnimating::BuildTransformations", "55 8B EC 53 56 57 FF 75 1C" );

			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseAnimating::GetSequenceActivity", "E8 ? ? ? ? 33 C9 3B F0" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseAnimating::LookupBone", "E8 ? ? ? ? 85 C0 78 4E" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseAnimating::GetBonePosition", "E8 ? ? ? ? 8D 14 24" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseAnimating::GetModelPtr", "E8 ? ? ? ? 8B 55 38" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseAnimating::LookupPoseParameter", "E8 ? ? ? ? 85 C0 79 08" );
			ADD_PATTERN_REL( CLIENT_DLL, "C_BaseAnimating::SetPoseParameter", "E8 ? ? ? ? 0F 28 D8 83 C4 04" );
		}

		{ // CCSGOPlayerAnimState

			ADD_PATTERN( CLIENT_DLL, "CCSGOPlayerAnimState::SetUpAimMatrix", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ?" );
			ADD_PATTERN( CLIENT_DLL, "CCSGOPlayerAnimState::SetUpWeaponAction", "55 8B EC 51 53 56 57 8B F9 8B 77 60" );

			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::Update", "E8 ? ? ? ? E9 ? ? ? ? 83 BE ? ? ? ? ?" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::ModifyEyePosition", "E8 ? ? ? ? 8B 06 8B CE FF 90 ? ? ? ? 85 C0 74 50" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::DoProceduralFootPlant", "E8 ? ? ? ? F6 86 ? ? ? ? ? 0F 84 ? ? ? ?" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::SetUpMovement", "E8 ? ? ? ? 8B CF E8 ? ? ? ? 8B CF E8 ? ? ? ? 8B 47 60" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::SetUpAliveloop", "E8 ? ? ? ? 8B CF E8 ? ? ? ? 8B 47 60" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::SetUpWholeBodyAction", "E8 ? ? ? ? 8B 47 60 83 B8 ? ? ? ? ?" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::SetUpFlinch", "E8 ? ? ? ? 8B CF E8 ? ? ? ? 33 C0" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::SetUpLean", "E8 ? ? ? ? 33 C0 89 44 24 0C" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::CacheSequences", "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 77 60" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::IncrementLayerCycle", "E8 ? ? ? ? FF 76 60 F3 0F 10 54 24 ?" );
			ADD_PATTERN_REL( CLIENT_DLL, "CCSGOPlayerAnimState::IncrementLayerWeight", "E8 ? ? ? ? 8B 5F 60 83 BB ? ? ? ? ?" );
		}

		ADD_PATTERN( CLIENT_DLL, "C_CSPlayer::DoExtraBoneProcessing", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C 80" );
		ADD_PATTERN( CLIENT_DLL, "C_WeaponCSBase::DrawCrosshair", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 8B 0D ? ? ? ?" );
		ADD_PATTERN( CLIENT_DLL, "CSequenceTransitioner::CheckForSequenceChange", "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85" );
	}

	{ // gameoverlayrenderer.dll

		ADD_PATTERN( GAMEOVERLAYRENDERER_DLL, "HookFunc", "55 8B EC 51 8B 45 10 C7" );
		ADD_PATTERN_REL( GAMEOVERLAYRENDERER_DLL, "UnhookFunc", "E8 ? ? ? ? 83 C4 08 FF 15 ? ? ? ?" );
	}

	{ // shaderapidx9.dll

		ADD_ADDRESS( SHADERAPIDX9_DLL, "IDirect3DDevice9::Reset", get_v_func( g_csgo.m_d3d9_device, 16 ) );
		ADD_ADDRESS( SHADERAPIDX9_DLL, "IDirect3DDevice9::EndScene", get_v_func( g_csgo.m_d3d9_device, 42 ) );
	}

#undef ADD_PATTERN
#undef ADD_PATTERN_REL
#undef ADD_ADDRESS
}

bool mem::impl::validate_addresses( ) {
	for ( auto& [ k, v ] : m_modules )
		if ( !v.validate( ) )
			return false;

	return true;
}