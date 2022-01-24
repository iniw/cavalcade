#pragma once

#include "../ctx/ctx.hpp"
#include "../entity_cacher/entity_cacher.hpp"
#include "../hack/hack.hpp"
#include "../gui/gui.hpp"

namespace cavalcade {
	struct hooks {
		bool init( );

	private:

		struct base_animating {
			using should_skip_animation_frame_fn = bool( __thiscall* )( unk );
			static bool __fastcall should_skip_animation_frame( unk ecx, unk edx );

			using build_transformations_fn = void( __thiscall* )( sdk::cs_player*, unk, unk, unk, unk, i32, unk );
			static void __fastcall build_transformations( sdk::cs_player* ecx, unk, unk hdr, unk pos, unk q, unk camera_transform, i32 bone_mask,
			                                              unk bone_computed );
		};

		struct csgo_player_anim_state {
			// static void __fastcall modify_eye_position( sdk::csgo_player_anim_state* ecx,
			// unk, math::v3& input_eye_pos );
		};

		struct c_cs_player {
			using do_extra_bone_processing_fn = void( __thiscall* )( unk, unk, unk, unk, unk, unk, unk );
			static void __fastcall do_extra_bone_processing( unk ecx, unk, unk studio_hdr, unk pos, unk q, unk bone_to_world, unk bone_computed,
			                                                 unk ik_context );
		};

		struct chlc_client {
			using frame_stage_notify_fn = void( __thiscall* )( unk, sdk::frame_stage );
			static void __fastcall frame_stage_notify( unk ecx, unk, sdk::frame_stage stage );

			using level_init_pre_entity_fn = void( __stdcall* )( const char* );
			static void __stdcall level_init_pre_entity( const char* name );

			using level_init_post_entity_fn = void( __cdecl* )( );
			static void __cdecl level_init_post_entity( );

			using on_override_mouse_input_fn = void( __fastcall* )( unk, unk, i32, f32&, f32& );
			static void __fastcall on_override_mouse_input( unk, unk, i32, f32&, f32& );
		};

		struct push_notice_ {
			using push_notice_fn = void( __fastcall* )( unk, unk, const char*, int, const char* );
			static void __fastcall push_notice( unk, unk, const char*, int, const char* );
		};

		struct client_cmd_ {
			using client_cmd_fn = void( __fastcall* )( unk, unk, const char* );
			static void __fastcall client_cmd( unk, unk, const char* );
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

		struct prediction {
			using setup_move_fn = void( __fastcall* )( unk, unk, sdk::cs_player*, sdk::user_cmd*, unk, sdk::move_data* );
			static void __fastcall setup_move( unk, unk, sdk::cs_player*, sdk::user_cmd*, unk, sdk::move_data* );
		};

		struct sequence_transitioner {
			using check_for_sequence_change_fn = void( __thiscall* )( unk, unk, i32, bool, bool );
			static void __fastcall check_for_sequence_change( unk ecx, unk, unk hdr, i32 cur_sequence, bool force_new_sequence, bool interpolate );
		};

		struct weapon_cs_base {
			using draw_crosshair_fn = void( __thiscall* )( sdk::weapon_cs_base* );
			static void __fastcall draw_crosshair( sdk::weapon_cs_base* ecx, unk edx );
		};

		struct entity_listener {
			using fn = void( __fastcall* )( unk, unk, unk, sdk::handle );
			static void __fastcall on_add_entity( unk, unk, unk, sdk::handle handle );
			static void __fastcall on_remove_entity( unk, unk, unk, sdk::handle handle );
		};

		struct sfhud {
			using fn = bool( __fastcall* )( unk, unk );
			static bool __fastcall weapon_reticle_knife_show( unk, unk );
		};

		struct crosshair {
			using should_draw_fn = bool( __fastcall* )( unk, unk );
			static bool __fastcall should_draw( unk, unk );
		};

		struct steam {
			// hook GameConnectedFriendChatMsg_t
			STEAM_CALLBACK( steam, on_friend_message, GameConnectedFriendChatMsg_t );
			// STEAM_CALLBACK( steam, on_join_server, GSClientApprove_t );
		};

		inline static steam whatever;
	};
} // namespace cavalcade

inline cavalcade::hooks g_hooks;