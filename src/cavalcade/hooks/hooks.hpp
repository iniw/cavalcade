#ifndef HOOKS_HPP
#define HOOKS_HPP

#include "../ctx/ctx.hpp"
#include "../hack/hack.hpp"

namespace cavalcade {
	struct hooks {
		bool init( );

	private:

		struct base_animating {
			using should_skip_animation_frame_fn = bool( __thiscall* )( unk );
			static bool __fastcall should_skip_animation_frame( unk ecx, unk edx );

			using build_transformations_fn = void( __thiscall* )( sdk::cs_player*, unk, unk, unk, unk, i32, unk );
			static void __fastcall build_transformations( sdk::cs_player* ecx, unk, unk hdr, unk pos, unk q, unk camera_transform,
			                                              i32 bone_mask, unk bone_computed );
		};

		struct csgo_player_anim_state {
			// static void __fastcall modify_eye_position( sdk::csgo_player_anim_state* ecx,
			// unk, math::v3& input_eye_pos );
		};

		struct c_cs_player {
			using do_extra_bone_processing_fn = void( __thiscall* )( unk, unk, unk, unk, unk, unk, unk );
			static void __fastcall do_extra_bone_processing( unk ecx, unk, unk studio_hdr, unk pos, unk q, unk bone_to_world,
			                                                 unk bone_computed, unk ik_context );
		};

		struct chlc_client {
			using frame_stage_notify_fn = void( __thiscall* )( unk, sdk::frame_stage );
			static void __fastcall frame_stage_notify( unk ecx, unk, sdk::frame_stage stage );
		};

		struct leaf_system {
			using is_renderable_in_pvs_fn = bool( __thiscall* )( unk, sdk::interfaces::client_renderable* );
			static bool __fastcall is_renderable_in_pvs( unk ecx, unk, sdk::interfaces::client_renderable* ent );
		};

		struct base_player {
			using create_move_fn = bool( __thiscall* )( sdk::cs_player*, f32, sdk::user_cmd* );
			static bool __fastcall create_move( sdk::cs_player* ecx, unk, f32 input_sample_time, sdk::user_cmd* cmd );
		};

		struct d3d9_device {
			using end_scene_fn = HRESULT( __stdcall* )( IDirect3DDevice9* );
			static HRESULT D3DAPI end_scene( IDirect3DDevice9* device );

			using reset_fn = HRESULT( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
			static HRESULT D3DAPI reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_params );
		};

		struct engine_client {
			using is_hltv_fn = bool( __thiscall* )( unk );
			static bool __fastcall is_hltv( unk ecx, unk );
		};

		struct sequence_transitioner {
			using check_for_sequence_change_fn = void( __thiscall* )( unk, unk, i32, bool, bool );
			static void __fastcall check_for_sequence_change( unk ecx, unk, unk hdr, i32 cur_sequence, bool force_new_sequence,
			                                                  bool interpolate );
		};

		struct weapon_cs_base {
			using draw_crosshair_fn = void( __thiscall* )( sdk::weapon_cs_base* );
			static void __fastcall draw_crosshair( sdk::weapon_cs_base* ecx, unk edx );
		};
	};
} // namespace cavalcade

inline cavalcade::hooks g_hooks;

#endif // HOOKS_HPP