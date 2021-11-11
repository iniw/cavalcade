#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	// a special object used only by parents
	// doesn't have identification
	struct scrollbar : base_object, base_traits< scrollbar > {
	private:

		bool m_dragging;

		render::point m_previous_mouse_pos;

	public:

		scrollbar( ) = default;

		void identify( const parent_ptr& parent ) override;

		void init( ) override;

		void render( ) override;

		bool think( ) override;

		void update_height( );
	};
} // namespace gui::objects