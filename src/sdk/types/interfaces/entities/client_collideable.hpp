#ifndef CLIENT_COLLIDEABLE_HPP
#define CLIENT_COLLIDEABLE_HPP

namespace sdk::interfaces {
	struct client_unknown;
	struct handle_entity;

	struct client_collideable {
		virtual handle_entity* get_entity_handle( )                                                   = 0;
		virtual math::v3f& obb_mins( ) const                                                          = 0;
		virtual math::v3f& obb_maxs( ) const                                                          = 0;
		virtual void world_space_trigger_bounds( math::v3f* world_mins, math::v3f* world_maxs ) const = 0;
		virtual bool test_collision( unk ray, u32 contents_mask, unk tr )                             = 0;
		virtual bool test_hitboxes( unk ray, u32 contents_mask, unk tr )                              = 0;
		virtual i32 get_collision_model_index( )                                                      = 0;
		virtual unk get_collision_model( )                                                            = 0;
		virtual math::v3f& get_collision_origin( ) const                                              = 0;
		virtual math::ang& get_collision_angles( ) const                                              = 0;
		virtual const math::matrix_3x4& collision_to_world_transform( ) const                         = 0;
		virtual i32 get_solid( ) const                                                                = 0;
		virtual i32 get_solid_flags( ) const                                                          = 0;
		virtual client_unknown* get_client_unknown( )                                                 = 0;
		virtual i32 get_collision_group( ) const                                                      = 0;
		virtual void world_space_surrounding_bounds( math::v3f* mins, math::v3f* maxs )               = 0;
		virtual u32 get_required_trigger_flags( ) const                                               = 0;
		virtual const math::matrix_3x4* get_root_parent_to_world_transform( ) const                   = 0;
		virtual unk get_v_physics_object( ) const                                                     = 0;
	};
} // namespace sdk::interfaces

#endif // CLIENT_COLLIDEABLE_HPP
