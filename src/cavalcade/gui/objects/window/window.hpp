#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct window : base_parent {
	protected:

		render::size m_size;
		bool m_dragging;

		render::rect m_reposition_area;
		render::rect m_resize_area;

		render::point m_previous_mouse_pos;

	public:

		window( ) = default;

		window( std::string_view label, const render::size& size );

		virtual ~window( ) = default;

		// NOTE(wini): this is the only object that doesn't use a init function
		virtual void init( ) override { }

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override;

		virtual void update_flags( ) override;

		virtual void render_debug_bounds( ) const override;

		static std::unique_ptr< window > make( std::string_view label, const render::size& size ) {
			return std::make_unique< window >( label, size );
		}
	};

} // namespace gui::objects