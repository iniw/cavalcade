#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct tab : base_parent, helpers::children_info< tab > {
	protected:

		// the area of our clickable button
		render::rect m_button_area;

		void center_tabs( );

	public:

		tab( ) = default;

		tab( std::string_view label ) : base_object( label ) { }

		virtual ~tab( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void update_flags( ) override;

		virtual void on_add_child( base_object* child ) override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override;

		virtual void render_debug_bounds( ) const override;

	public:

		const auto& button_area( ) const {
			return m_button_area;
		}
	};
} // namespace gui::objects