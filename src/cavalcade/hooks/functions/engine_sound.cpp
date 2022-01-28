#include "../hooks.hpp"

void cavalcade::hooks::engine_sound::emit_sound( unk ecx, unk edx, unk filter, i32 entity_index, i32 channel, const char* sound_entry,
                                                 u32 sound_entry_hash, const char* sample, f32 volume, f32 attenuation, i32 seed, i32 flags,
                                                 i32 pitch, const math::v3f* origin, const math::v3f* direction, math::v3f* vec_origins,
                                                 bool update_positions, f32 sound_time, i32 speaker_entity, unk params ) {
	static auto og = g_mem[ ENGINE_DLL ].get_og< emit_sound_fn >( HASH_CT( "EmitSound" ) );

	if ( !g_csgo.m_engine->is_in_game( ) )
		return og( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin,
		           direction, vec_origins, update_positions, sound_time, speaker_entity, params );

	if ( !g_ctx.m_local || !g_ctx.m_local.get( ).is_alive( ) )
		return og( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin,
		           direction, vec_origins, update_positions, sound_time, speaker_entity, params );

	auto hash = HASH_RT( sound_entry );
	if ( hash == HASH_CT( "Deathcam.Review_Start" ) || hash == HASH_CT( "Deathcam.Review_Victory" ) )
		g_ctx.m_in_deathcam = true;
	else if ( hash == HASH_CT( "Deathcam.Review_End" ) )
		g_ctx.m_in_deathcam = false;

	if ( entity_index == g_ctx.m_local.get( ).get_networkable_index( ) ) {
		if ( std::string_view{ sound_entry }.find( "land" ) != std::string_view::npos ) {
			if ( g_hack.m_movement.m_in_jumpbug || g_hack.m_movement.m_edgebug.m_in_edgebug ) {
				volume = 0.F;
			}
		}
	}

	og( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin, direction,
	    vec_origins, update_positions, sound_time, speaker_entity, params );
}