#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../base/base.hpp"

namespace gui::objects {
	struct window : base_parent {
		window( std::string_view label, const render::size& size ) {
			m_label = label.data( );

			// TODO(wini): center it to the middle of the screen
			m_static_rect = render::rect( 0, 0, size[ X ], size[ Y ] );

			m_type = type::WINDOW;
		}

		// NOTE(wini): this is the only object that doesn't require a init function
		void init( parent_ptr parent ) override { }

		void render( ) override;

		bool think( ) override;

		using ptr = std::shared_ptr< window >;

		static ptr make( std::string_view label, const render::size& size ) {
			return std::make_shared< window >( label, size );
		}
	};

} // namespace gui::objects

#endif // WINDOW_HPP