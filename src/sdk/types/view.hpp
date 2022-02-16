

#pragma once

namespace sdk {
	// I HAVE TO UPDATE THIS STRUCT TO MATCH STYLE I KNOW ITS FROM AN OLD HACK
	class view {
	public:

		int x, x_old;
		int y, y_old;
		int w, w_old;
		int h, h_old;

		bool ortho;
		float ortho_left;
		float ortho_top;
		float ortho_right;
		float ortho_bottom;

	private:

		char pad1[ 0x7C ];

	public:

		float fov;
		float fov_viewmodel;
		math::v3f origin;
		math::v3f angles;

		float z_near;
		float z_far;
		float z_near_viewmodel;
		float z_far_viewmodel;

		float aspect_ratio;
		float near_blur_depth;
		float near_focus_depth;
		float far_focus_Depth;
		float far_blur_depth;
		float near_blur_radius;
		float far_blur_radius;
		int do_f_quality;
		int motion_blur_mode;

		float shutter_time;
		math::v3f shutter_open_position;
		math::v3f shutter_open_angles;
		math::v3f shutter_close_position;
		math::v3f shutter_close_angles;

		float off_center_top;
		float off_center_bottom;
		float off_center_left;
		float off_center_right;

		bool off_center                           : 1;
		bool render_to_subrect_of_larger_screen   : 1;
		bool do_bloom_and_tone_mapping            : 1;
		bool do_depth_of_field                    : 1;
		bool hdr_target                           : 1;
		bool draw_world_normal                    : 1;
		bool cull_front_faces                     : 1;
		bool cache_full_scene_state               : 1;
		bool render_flashlight_depth_translucents : 1;

	private:

		char pad2[ 0x40 ];
	}; // Size=0x014C
} // namespace sdk