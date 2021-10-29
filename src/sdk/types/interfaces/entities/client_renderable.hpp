#ifndef CLIENT_RENDERABLE_HPP
#define CLIENT_RENDERABLE_HPP

namespace sdk::interfaces {
	struct client_unknown;

	struct client_renderable {
		virtual client_unknown* get_client_unknown( ) = 0;
		virtual math::v3f& get_render_origin( )       = 0;
		virtual math::v3f& get_render_angles( )       = 0;
		virtual bool should_draw( )                   = 0;
		virtual i32 get_render_flags( )               = 0;
		virtual void unused( ) const { }
		virtual u16 get_shadow_handle( ) const                                                                          = 0;
		virtual u16& render_handle( )                                                                                   = 0;
		virtual unk get_model( ) const                                                                                  = 0;
		virtual i32 draw_model( i32 flags, const u8& instance )                                                         = 0;
		virtual i32 get_body( )                                                                                         = 0;
		virtual void get_color_modulation( f32* color )                                                                 = 0;
		virtual bool lod_test( )                                                                                        = 0;
		virtual bool setup_bones( math::matrix_3x4* bone_to_world_out, i32 max_bones, i32 bone_mask, f32 current_time ) = 0;
		virtual void setup_weights( const math::matrix_3x4* bone_to_world_out, i32 arg, f32* arg1, f32* arg2 )          = 0;
		virtual void do_animation_events( )                                                                             = 0;
		virtual unk get_pvs_notify_interface( )                                                                         = 0;
		virtual void get_render_bounds( math::v3f& mins, math::v3f& maxs )                                              = 0;
		virtual void get_render_bounds_worldspace( math::v3f& mins, math::v3f& maxs )                                   = 0;
		virtual void get_shadow_render_bounds( math::v3f& mins, math::v3f& maxs, i32 shadow_type )                      = 0;
		virtual bool should_receive_projected_textures( i32 flags )                                                     = 0;
		virtual bool get_shadow_cast_distance( f32* dist, i32 shadow_type ) const                                       = 0;
		virtual bool get_shadow_cast_direction( math::v3f* direction, i32 shadow_type ) const                           = 0;
		virtual bool is_shadow_dirty( )                                                                                 = 0;
		virtual void mark_shadow_dirty( bool dirty )                                                                    = 0;
		virtual client_renderable* get_shadow_parent( )                                                                 = 0;
		virtual client_renderable* first_shadow_child( )                                                                = 0;
		virtual client_renderable* next_shadow_peer( )                                                                  = 0;
		virtual i32 shadow_cast_type( )                                                                                 = 0;
		virtual void create_model_instance( )                                                                           = 0;
		virtual u16 get_model_instance( )                                                                               = 0;
		virtual const math::matrix_3x4& renderable_to_world_transform( )                                                = 0;
		virtual i32 lookup_attachment( cstr attachment_name )                                                           = 0;
		virtual bool get_attachment( i32 number, math::v3f& origin, math::v3f& angles )                                 = 0;
		virtual bool get_attachment( i32 number, math::matrix_3x4& matrix )                                             = 0;
		virtual f32* get_render_clip_plane( )                                                                           = 0;
		virtual i32 get_skin( )                                                                                         = 0;
		virtual void on_threaded_draw_setup( )                                                                          = 0;
		virtual bool uses_flex_delayed_weights( )                                                                       = 0;
		virtual void record_tool_message( )                                                                             = 0;
		virtual bool should_draw_for_split_screen_user( i32 slot )                                                      = 0;
		virtual u8 override_alpha_modulation( u8 alpha )                                                                = 0;
		virtual u8 override_shadow_alpha_modulation( u8 alpha )                                                         = 0;
		virtual unk get_client_model_renderable( )                                                                      = 0;
	};
} // namespace sdk::interfaces

#endif // CLIENT_RENDERABLE_HPP
