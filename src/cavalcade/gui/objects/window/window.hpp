#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../base/base.hpp"

namespace gui::objects {
	struct window : base_parent {
		window( std::string_view label, const render::size& size );

		// NOTE(wini): this is the only object that doesn't use a init function
		void init( parent_ptr parent ) override {
			ENFORCE( true, "windows can't be children: parent->m_id = {}", parent->m_id );
		}

		void render( ) override;

		bool think( ) override;

		using ptr = std::shared_ptr< window >;

		static ptr make( std::string_view label, const render::size& size ) {
			return std::make_shared< window >( label, size );
		}
	};

} // namespace gui::objects

#endif // WINDOW_HPP