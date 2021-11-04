#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct checkbox : public base_child< bool > {
		checkbox( std::string_view name, std::string_view label );

		void init( ) override;

		void render( ) override;

		bool think( ) override;
	};
} // namespace gui::objects