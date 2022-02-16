#ifndef PANORAMA_HPP
#define PANORAMA_HPP

#include <xmemory>
#pragma once

namespace sdk::interfaces {
	struct ui_panel {
		const char* get_id( ) {
			return mem::call_v_func< const char*, 9 >( this );
		}

		ui_panel* get_parent( ) {
			return mem::call_v_func< ui_panel*, 25 >( this );
		}

		i32 get_child_count( ) {
			return mem::call_v_func< i32, 48 >( this );
		}

		ui_panel* get_child( i32 n ) {
			return mem::call_v_func< ui_panel*, 49 >( this, n );
		}

		bool has_class( const char* name ) {
			return mem::call_v_func< bool, 139 >( this, name );
		}

		float get_attribute_float( const char* name, f32 defalt_value ) {
			return mem::call_v_func< f32, 278 >( this, name, defalt_value );
		}

		void set_attribute_float( const char* name, f32 value ) {
			return mem::call_v_func< void, 288 >( this, name, value );
		}
	};

	struct ui_engine {
		// NOTE(para): dispatchevent - 16
		bool is_valid_panel_pointer( ui_panel const* p ) {
			return mem::call_v_func< bool, 36 >( this, p );
		}

		// NOTE(para): 'already_compiled' is never used. a8 is probably v8::Script::Run error handling shit.
		void run_script( ui_panel* panel, const char* path_to_js, const char* path_to_xml, int a5, int a6, bool already_compiled = false,
		                 bool a8 = false ) {
			return mem::call_v_func< void, 113 >( this, panel, path_to_js, path_to_xml, a5, a6, already_compiled, a8 );
		}

		// NOTE(para): dispatchevent + 4
		ui_panel* get_last_dispatched_event_target_panel( ) {
			return mem::call_v_func< ui_panel*, 56 >( this );
		}
	};

	struct panorama_ui_engine {
		ui_engine* access_ui_engine( ) {
			return mem::call_v_func< ui_engine*, 11 >( this );
		}
	};
} // namespace sdk::interfaces

#endif /* PANORAMA_HPP */
