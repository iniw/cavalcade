#ifndef CLIENT_ALPHA_PROPERTY_HPP
#define CLIENT_ALPHA_PROPERTY_HPP

namespace sdk::interfaces {
	struct client_unknown;

	struct client_alpha_property {
		virtual client_unknown* get_i_client_unknown( )                                                  = 0;
		virtual void set_alpha_modulation( u8 alpha )                                                    = 0;
		virtual void set_render_fx( i32 render_fx, i32 render_mode, f32 start_time, f32 duration = 0.f ) = 0;
		virtual void set_fade( f32 global_fade_scale, f32 dist_fade_start, f32 dist_fade_end )           = 0;
		virtual void set_desync_offset( i32 offset )                                                     = 0;
		virtual void enable_alpha_modulation_override( bool enable )                                     = 0;
		virtual void enable_shadow_alpha_modulation_override( bool enable )                              = 0;
		virtual void set_distance_fade_mode( i32 fade_mode )                                             = 0;
	};
} // namespace sdk::interfaces

#endif // CLIENT_ALPHA_PROPERTY_HPP