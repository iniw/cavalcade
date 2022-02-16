

#pragma once

namespace sdk {
	struct material {
		cstr get_name( ) {
			return mem::call_v_func< cstr, 0 >( this );
		}

		cstr get_texture_group_name( ) {
			return mem::call_v_func< cstr, 1 >( this );
		}
	};
} // namespace sdk