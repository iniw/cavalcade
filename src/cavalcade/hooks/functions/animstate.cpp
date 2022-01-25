#include "../hooks.hpp"

// NOTE(para): fixes camera height if u duck after pred on high ping
void cavalcade::hooks::csgo_player_anim_state::modify_eye_position( unk, unk, math::v3f& ) {
	return;
}