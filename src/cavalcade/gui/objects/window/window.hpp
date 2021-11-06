#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct window : public base_parent, public base_traits< window > {
		window( std::string_view name, std::string_view label, const render::size& size );

		// NOTE(wini): this is the only object that doesn't use a init function
		void init( ) override {
			ENFORCE( true, "windows can't be children: parent->m_id = 0x{:X}", m_parent->m_id );
		}

		void render( ) override;

		bool think( ) override;

		static ptr make( std::string_view name, std::string_view label, const render::size& size ) {
			return std::make_shared< window >( name, label, size );
		}
	};

} // namespace gui::objects