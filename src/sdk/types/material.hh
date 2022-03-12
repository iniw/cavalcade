#ifndef MATERIAL_HH
#define MATERIAL_HH

#pragma once

namespace sdk {
	struct material {
		const char* get_name( ) {
			return mem::call_v_func< const char*, 0 >( this );
		}

		const char* get_texture_group_name( ) {
			return mem::call_v_func< const char*, 1 >( this );
		}

		void set_material_var_flag( i32 flag, bool on ) {
			return mem::call_v_func< void, 29 >( this, flag, on );
		}
	};
} // namespace sdk

#endif /* MATERIAL_HH */
