

#pragma once

#include "../imatrendercontext.hpp"

namespace sdk::interfaces {
	struct material_system {
		VFUNC( sdk::material*, find_material, 84, ( cstr name, cstr texture, bool complain = true, cstr prefix = nullptr ), name, texture, complain,
		       prefix );
		VFUNC( sdk::texture*, find_texture, 91, ( cstr texture, cstr texture_group, bool complain = true, int additional_creation_flags = 0 ),
		       texture, texture_group, complain, additional_creation_flags );
		VFUNC( sdk::mat_render_context*, get_render_context, 115, ( ) );
	};
} // namespace sdk::interfaces