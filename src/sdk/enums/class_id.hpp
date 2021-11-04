#pragma once

namespace sdk {
	enum class class_id : i16
	{
		AI_BASE_NPC = 0,
		AK47,
		BASE_ANIMATING,
		BASE_ANIMATING_OVERLAY,
		BASE_ATTRIBUTABLE_ITEM,
		BASE_BUTTON,
		BASE_COMBAT_CHARACTER,
		BASE_COMBAT_WEAPON,
		BASE_CS_GRENADE,
		BASE_CS_GRENADE_PROJECTILE,
		BASE_DOOR,
		BASE_ENTITY,
		BASE_FLEX,
		BASE_GRENADE,
		BASE_PARTICLE_ENTITY,
		BASE_PLAYER,
		BASE_PROP_DOOR,
		BASE_TEAM_OBJECTIVE_RESOURCE,
		BASE_TEMP_ENTITY,
		BASE_TOGGLE,
		BASE_TRIGGER,
		BASE_VIEW_MODEL,
		BASE_V_PHYSICS_TRIGGER,
		BASE_WEAPON_WORLD_MODEL,
		BEAM,
		BEAM_SPOTLIGHT,
		BONE_FOLLOWER,
		BRC4_TARGET,
		BREACH_CHARGE,
		BREACH_CHARGE_PROJECTILE,
		BREAKABLE_PROP,
		BREAKABLE_SURFACE,
		BUMP_MINE,
		BUMP_MINE_PROJECTILE,
		C4,
		CASCADE_LIGHT,
		CHICKEN,
		COLOR_CORRECTION,
		COLOR_CORRECTION_VOLUME,
		CS_GAME_RULES_PROXY,
		CS_PLAYER,
		CS_PLAYER_RESOURCE,
		CS_RAGDOLL,
		CS_TEAM,
		DANGER_ZONE,
		DANGER_ZONE_CONTROLLER,
		DEAGLE,
		DECOY_GRENADE,
		DECOY_PROJECTILE,
		DRONE,
		DRONEGUN,
		DYNAMIC_LIGHT,
		DYNAMIC_PROP,
		ECON_ENTITY,
		ECON_WEARABLE,
		EMBERS,
		ENTITY_DISSOLVE,
		ENTITY_FLAME,
		ENTITY_FREEZING,
		ENTITY_PARTICLE_TRAIL,
		ENV_AMBIENT_LIGHT,
		ENV_DETAIL_CONTROLLER,
		ENV_DOF_CONTROLLER,
		ENV_GAS_CANISTER,
		ENV_PARTICLE_SCRIPT,
		ENV_PROJECTED_TEXTURE,
		ENV_QUADRATIC_BEAM,
		ENV_SCREEN_EFFECT,
		ENV_SCREEN_OVERLAY,
		ENV_TONEMAP_CONTROLLER,
		ENV_WIND,
		FE_PLAYER_DECAL,
		FIRE_CRACKER_BLAST,
		FIRE_SMOKE,
		FIRE_TRAIL,
		FISH,
		FISTS,
		FLASHBANG,
		FOG_CONTROLLER,
		FOOTSTEP_CONTROL,
		FUNC_DUST,
		FUNC_LOD,
		FUNC_AREA_PORTAL_WINDOW,
		FUNC_BRUSH,
		FUNC_CONVEYOR,
		FUNC_LADDER,
		FUNC_MONITOR,
		FUNC_MOVE_LINEAR,
		FUNC_OCCLUDER,
		FUNC_REFLECTIVE_GLASS,
		FUNC_ROTATING,
		FUNC_SMOKE_VOLUME,
		FUNC_TRACK_TRAIN,
		GAME_RULES_PROXY,
		GRASS_BURN,
		HANDLE_TEST,
		HE_GRENADE,
		HOSTAGE,
		HOSTAGE_CARRIABLE_PROP,
		INCENDIARY_GRENADE,
		INFERNO,
		INFO_LADDER_DISMOUNT,
		INFO_MAP_REGION,
		INFO_OVERLAY_ACCESSOR,
		ITEM_HEALTHSHOT,
		ITEM_CASH,
		ITEM_DOGTAGS,
		KNIFE,
		KNIFE_GG,
		LIGHT_GLOW,
		MAP_VETO_PICK_CONTROLLER,
		MATERIAL_MODIFY_CONTROL,
		MELEE,
		MOLOTOV_GRENADE,
		MOLOTOV_PROJECTILE,
		MOVIE_DISPLAY,
		PARADROP_CHOPPER,
		PARTICLE_FIRE,
		PARTICLE_PERFORMANCE_MONITOR,
		PARTICLE_SYSTEM,
		PHYS_BOX,
		PHYS_BOX_MULTIPLAYER,
		PHYSICS_PROP,
		PHYSICS_PROP_MULTIPLAYER,
		PHYS_MAGNET,
		PHYS_PROP_AMMO_BOX,
		PHYS_PROP_LOOT_CRATE,
		PHYS_PROP_RADAR_JAMMER,
		PHYS_PROP_WEAPON_UPGRADE,
		PLANTED_C4,
		PLASMA,
		PLAYER_PING,
		PLAYER_RESOURCE,
		POINT_CAMERA,
		POINT_COMMENTARY_NODE,
		POINT_WORLD_TEXT,
		POSE_CONTROLLER,
		POST_PROCESS_CONTROLLER,
		PRECIPITATION,
		PRECIPITATION_BLOCKER,
		PREDICTED_VIEW_MODEL,
		PROP_HALLUCINATION,
		PROP_COUNTER,
		PROP_DOOR_ROTATING,
		PROP_JEEP,
		PROP_VEHICLE_DRIVEABLE,
		RAGDOLL_MANAGER,
		RAGDOLL_PROP,
		RAGDOLL_PROP_ATTACHED,
		ROPE_KEYFRAME,
		SCAR17,
		SCENE_ENTITY,
		SENSOR_GRENADE,
		SENSOR_GRENADE_PROJECTILE,
		SHADOW_CONTROL,
		SLIDESHOW_DISPLAY,
		SMOKE_GRENADE,
		SMOKE_GRENADE_PROJECTILE,
		SMOKE_STACK,
		SNOWBALL,
		SNOWBALL_PILE,
		SNOWBALL_PROJECTILE,
		SPATIAL_ENTITY,
		SPOTLIGHT_END,
		SPRITE,
		SPRITE_ORIENTED,
		SPRITE_TRAIL,
		STATUE_PROP,
		STEAM_JET,
		SUN,
		SUNLIGHT_SHADOW_CONTROL,
		SURVIVAL_SPAWN_CHOPPER,
		TABLET,
		TEAM,
		TEAMPLAY_ROUND_BASED_RULES_PROXY,
		TE_ARMOR_RICOCHET,
		TE_BASE_BEAM,
		TE_BEAM_ENT_POINT,
		TE_BEAM_ENTS,
		TE_BEAM_FOLLOW,
		TE_BEAM_LASER,
		TE_BEAM_POINTS,
		TE_BEAM_RING,
		TE_BEAM_RING_POINT,
		TE_BEAM_SPLINE,
		TE_BLOOD_SPRITE,
		TE_BLOOD_STREAM,
		TE_BREAK_MODEL,
		TEBSP_DECAL,
		TE_BUBBLES,
		TE_BUBBLE_TRAIL,
		TE_CLIENT_PROJECTILE,
		TE_DECAL,
		TE_DUST,
		TE_DYNAMIC_LIGHT,
		TE_EFFECT_DISPATCH,
		TE_ENERGY_SPLASH,
		TE_EXPLOSION,
		TE_FIRE_BULLETS,
		TE_FIZZ,
		TE_FOOTPRINT_DECAL,
		TE_FOUNDRY_HELPERS,
		TE_GAUSS_EXPLOSION,
		TE_GLOW_SPRITE,
		TE_IMPACT,
		TE_KILL_PLAYER_ATTACHMENTS,
		TE_LARGE_FUNNEL,
		TE_METAL_SPARKS,
		TE_MUZZLE_FLASH,
		TE_PARTICLE_SYSTEM,
		TE_PHYSICS_PROP,
		TE_PLANT_BOMB,
		TE_PLAYER_ANIM_EVENT,
		TE_PLAYER_DECAL,
		TE_PROJECTED_DECAL,
		TE_RADIO_ICON,
		TE_SHATTER_SURFACE,
		TE_SHOW_LINE,
		TESLA,
		TE_SMOKE,
		TE_SPARKS,
		TE_SPRITE,
		TE_SPRITE_SPRAY,
		TEST_PROXY_TOGGLE_NETWORKABLE,
		TEST_TRACELINE,
		TE_WORLD_DECAL,
		TRIGGER_PLAYER_MOVEMENT,
		TRIGGER_SOUND_OPERATOR,
		V_GUI_SCREEN,
		VOTE_CONTROLLER,
		WATER_BULLET,
		WATER_LOD_CONTROL,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_BASE_ITEM,
		WEAPON_BIZON,
		WEAPON_CS_BASE,
		WEAPON_CS_BASE_GUN,
		WEAPON_CYCLER,
		WEAPON_ELITE,
		WEAPON_FAMAS,
		WEAPON_FIVE_SEVEN,
		WEAPON_G3SG1,
		WEAPON_GALIL,
		WEAPON_GALIL_AR,
		WEAPON_GLOCK,
		WEAPON_HKP2000,
		WEAPON_M249,
		WEAPON_M3,
		WEAPON_M4A1,
		WEAPON_MAC10,
		WEAPON_MAG7,
		WEAPON_MP5_NAVY,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NEGEV,
		WEAPON_NOVA,
		WEAPON_P228,
		WEAPON_P250,
		WEAPON_P90,
		WEAPON_SAWEDOFF,
		WEAPON_SCAR20,
		WEAPON_SCOUT,
		WEAPON_SG550,
		WEAPON_SG552,
		WEAPON_SG556,
		WEAPON_SHIELD,
		WEAPON_SSG08,
		WEAPON_TASER,
		WEAPON_TEC9,
		WEAPON_TMP,
		WEAPON_UMP45,
		WEAPON_USP,
		WEAPON_XM1014,
		WEAPON_ZONE_REPULSOR,
		WORLD,
		WORLD_VGUI_TEXT,
		DUST_TRAIL,
		MOVIE_EXPLOSION,
		PARTICLE_SMOKE_GRENADE,
		ROCKET_TRAIL,
		SMOKE_TRAIL,
		SPORE_EXPLOSION,
		SPORE_TRAIL
	};
} // namespace sdk