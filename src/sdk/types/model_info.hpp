

#pragma once

#include "../../other.hpp"

namespace sdk {
	struct studio_bbox {
		i32 m_bone;
		i32 m_group;
		math::v3f m_bb_min;
		math::v3f m_bb_max;
		i32 m_hitbox_name_index;
		math::v3f m_offset_orientation;
		f32 capsule_radius;
	};

	enum e_hitbox
	{
		HEAD,
		NECK,
		PELVIS,
		BELLY,
		THORAX,
		LOWER_CHEST,
		UPPER_CHEST,
		RIGHT_THIGH,
		LEFT_THIGH,
		RIGHT_CALF,
		LEFT_CALF,
		RIGHT_FOOT,
		LEFT_FOOT,
		RIGHT_HAND,
		LEFT_HAND,
		RIGHT_UPPER_ARM,
		RIGHT_FOREARM,
		LEFT_UPPER_ARM,
		LEFT_FOREARM,
		MAX
	};

	struct studio_hitbox_set {
		i32 m_name_index;
		i32 m_num_hitboxes;
		i32 m_hitbox_index;

		cstr get_name( ) {
			return m_name_index ? ( cstr )( ( uintptr_t )this + m_name_index ) : nullptr;
		}

		studio_bbox* get_hitbox( i32 i ) {
			return i >= 0 && i < m_num_hitboxes ? ( studio_bbox* )( ( uintptr_t )this + m_hitbox_index ) + i : nullptr;
		}
	};

	struct studio_bone {
		i32 m_name_index;
		i32 m_parent;
		PAD( 152 );
		i32 m_flags;
		PAD( 52 );

		cstr get_name( ) {
			return m_name_index ? ( cstr )( ( uintptr_t )this + m_name_index ) : nullptr;
		}
	};

	struct studio_hdr {
		i32 m_id;
		i32 m_version;
		i32 m_checksum;
		char m_name[ 64 ];
		i32 m_length;
		math::v3f m_eye_position;
		math::v3f m_illum_position;
		math::v3f m_hull_min;
		math::v3f m_hull_max;
		math::v3f m_bb_min;
		math::v3f m_bb_max;
		i32 m_flags;
		i32 m_num_bones;
		i32 m_bone_index;
		i32 m_num_bone_controllers;
		i32 m_bone_controller_index;
		i32 m_num_hitbox_sets;
		i32 m_hitbox_set_index;

		const studio_bone* get_bone( i32 i ) {
			return i >= 0 && i <= m_num_bones ? ( studio_bone* )( ( uintptr_t )this + m_bone_index ) + i : nullptr;
		}

		studio_hitbox_set* get_hitbox_set( i32 i ) {
			return i >= 0 && i <= m_num_hitbox_sets ? ( studio_hitbox_set* )( ( uintptr_t )this + m_hitbox_set_index ) + i : nullptr;
		}
	};

	struct model {
		unk m_handle;
		char m_name[ 260 ];
		i32 m_load_flags;
		i32 m_server_count;
		i32 m_type;
		i32 m_flags;
		math::v3f m_mins;
		math::v3f m_maxs;
	};

	namespace interfaces {

		struct model_info {
			enum idx
			{
				GET_MODEL_INDEX  = 2,
				GET_STUDIO_MODEL = 32
			};

			VFUNC( i32, get_model_index, idx::GET_MODEL_INDEX, ( cstr name ), name );
			VFUNC( studio_hdr*, get_studio_model, idx::GET_STUDIO_MODEL, ( const model* mod ), mod );
		};
	} // namespace interfaces
} // namespace sdk