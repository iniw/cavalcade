#pragma once

#include "base_entity.hpp"

namespace sdk {
	struct base_attributable_item : base_entity {
		NETVAR( def_idx, get_item_definition_index, "DT_BaseAttributableItem->m_iItemDefinitionIndex" );

		NETVAR( i32, get_item_id_high, "DT_BaseAttributableItem->m_iItemIDHigh" );

		NETVAR( i32, get_item_id_low, "DT_BaseAttributableItem->m_iItemIDLow" );

		NETVAR( i32, get_entity_quality, "DT_BaseAttributableItem->m_iEntityQuality" );

		NETVAR( i32, get_fallback_paint_kit, "DT_BaseAttributableItem->m_nFallbackPaintKit" );

		NETVAR( i32, get_fallback_seed, "DT_BaseAttributableItem->m_nFallbackSeed" );

		NETVAR( f32, get_fallback_wear, "DT_BaseAttributableItem->m_flFallbackWear" );

		NETVAR( i32, get_fallback_stat_trak, "DT_BaseAttributableItem->m_nFallbackStatTrak" );

		PNETVAR( char, get_custom_name, "DT_BaseAttributableItem->m_szCustomName" );
	};
} // namespace sdk