#pragma once

#include "../../objects/base/base.hpp"

namespace gui::helpers {
	// meant to be used by listboxes/selectables/their derivatives
	struct list_option : virtual objects::base_object, children_info< list_option > {
	public:

		list_option( std::string_view label ) : base_object( label ){ };

		virtual ~list_option( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		static int height( ) {
			return g_render.get_font( render::font::MENU ).height( ) + 2;
		}
	};
} // namespace gui::helpers