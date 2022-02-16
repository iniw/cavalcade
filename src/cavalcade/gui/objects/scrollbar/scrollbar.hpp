#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	// a special object used only by parents
	// doesn't have identification
	struct scrollbar : virtual base_object {
	private:

		bool m_clicking;
		bool m_dragging;

		int m_start_drag_y;
		int m_start_drag_scrollbar_pos;

		bool m_drag_scrolling;
		float m_drag_scroll_goal;

		float m_scroll       = 0.f;
		float m_scroll_speed = 0.f;

		int m_scroll_pos    = 0;
		int m_scrollbar_pos = 0;

		int m_total_height;
		int m_visible_height;
		int m_max_scroll;
		float m_scrollbar_ratio;

		float m_bar_width;

	public:

		scrollbar( ) = default;

		virtual ~scrollbar( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void animate( ) override;

		virtual void update_flags( ) override;

		virtual void resize( const render::size& delta ) override;

		bool update_scroll( );

		void update_height( );

		void set_scroll( float amt ) {
			m_scroll = amt;
		}
	};
} // namespace gui::objects