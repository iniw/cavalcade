#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct checkbox : public base_child< bool >, public base_traits< checkbox > {
		checkbox( std::string_view name, std::string_view label );

		void init( ) override;

		void render( ) const override;

		bool think( ) override;

		// we don't wanna resize
		void resize( const render::point& delta ) override { }
	};
} // namespace gui::objects