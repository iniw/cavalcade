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

		// wtf lol literally dose the same thing as @139
		bool has_class_2( const char* name ) {
			return mem::call_v_func< bool, 41 >( this, name );
		}

		bool load_layout_from_string( const char* str, bool a1 = false, bool a2 = false ) {
			return mem::call_v_func< bool, 19 >( this, str, a1, a2 );
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

		void set_has_class( const char* name, bool has ) {
			return mem::call_v_func< void, 145 >( this, name, has );
		}

		float get_attribute_float( const char* name, f32 defalt_value ) {
			return mem::call_v_func< f32, 278 >( this, name, defalt_value );
		}

		void set_attribute_float( const char* name, f32 value ) {
			return mem::call_v_func< void, 288 >( this, name, value );
		}

		i32 get_classes_count( ) {
			return *( i32* )( ( uintptr_t )( this ) + 296 );
		}

		char* get_class( i32 i ) {
			return *( char** )( ( uintptr_t )( this ) + 284 + ( i * 2 ) );
		}
	};

	struct panel_2d {
		PAD( 4 );
		ui_panel* m_panel;
	};

	struct ui_engine {
		// NOTE(para): dispatchevent - 16
		bool is_valid_panel_pointer( ui_panel const* p ) {
			return mem::call_v_func< bool, 36 >( this, p );
		}

		// NOTE(para): 'already_compiled' is never used. a8 is probably v8::Script::Run error handling shit.
		void run_script( ui_panel* panel, const char* entire_js, const char* path_to_xml, int a5, int a6, bool already_compiled = false,
		                 bool a8 = false ) {
			return mem::call_v_func< void, 113 >( this, panel, entire_js, path_to_xml, a5, a6, already_compiled, a8 );
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