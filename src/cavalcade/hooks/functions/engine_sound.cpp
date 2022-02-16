#include "../hooks.hpp"

void cavalcade::hooks::engine_sound::emit_sound( unk ecx, unk edx, unk filter, i32 entity_index, i32 channel, cstr sound_entry, u32 sound_entry_hash,
                                                 cstr sample, f32 volume, f32 attenuation, i32 seed, i32 flags, i32 pitch, const math::v3f* origin,
                                                 const math::v3f* direction, math::v3f* vec_origins, bool update_positions, f32 sound_time,
                                                 i32 speaker_entity, unk params ) {
	static auto og = g_mem[ ENGINE_DLL ].get_og< emit_sound_fn >( HASH_CT( "EmitSound" ) );

	if ( !g_csgo.m_engine->is_in_game( ) )
		return og( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin,
		           direction, vec_origins, update_positions, sound_time, speaker_entity, params );

	if ( !g_ctx.m_local || !g_ctx.m_local.valid( ) )
		return og( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin,
		           direction, vec_origins, update_positions, sound_time, speaker_entity, params );

	auto se = std::string_view{ sound_entry };
	if ( se.find( "Deathcam.Review_Start" ) != std::string_view::npos || se.find( "Deathcam.Review_Victory" ) != std::string_view::npos )
		g_ctx.m_in_deathcam = true;
	else if ( se.find( "Deathcam.Review_End" ) != std::string_view::npos )
		g_ctx.m_in_deathcam = false;

	if ( !g_ctx.m_local.get( ).is_alive( ) )
		return og( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin,
		           direction, vec_origins, update_positions, sound_time, speaker_entity, params );

	if ( g_hack.m_prediction.m_prevent_sounds )
		volume = 0.F;

	og( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin, direction,
	    vec_origins, update_positions, sound_time, speaker_entity, params );
}