#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	// a special object used only by parents
	// doesn't get added to the children list; doesn't have identification
	struct scrollbar : base_object, base_traits< scrollbar >, std::enable_shared_from_this< scrollbar > {
	private:

		bool m_dragging;

		render::point m_previous_mouse_pos;

	public:

		scrollbar( ) = default;

		void identify( const parent_ptr& parent ) override;

		// no init 4 u
		void init( ) override;

		void render( ) override;

		bool think( ) override;

		ptr get( ) {
			return shared_from_this( );
		}

		void update_height( );
	};
} // namespace gui::objects