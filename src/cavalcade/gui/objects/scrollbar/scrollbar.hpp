#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	// a special object used only by parents
	// doesn't have identification
	struct scrollbar : base_object, base_traits< scrollbar > {
	private:

		bool m_dragging;

		i32 m_previous_mouse_pos_y;

		f32 m_ratio;

		math::v2i m_max_bounds;

	public:

		scrollbar( ) = default;

		void identify( const parent_raw_ptr parent ) override;

		void init( ) override;

		void render( ) const override;

		bool think( ) override;

		void scroll( i32 delta );

		void update_height( );
	};
} // namespace gui::objects