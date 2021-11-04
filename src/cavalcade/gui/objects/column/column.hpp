#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	// an almost proxy-like object that serves only as a positioning tool
	struct column : public base_parent {
		column( ) = default;

		// don't allow construction with any arguments
		column( auto... args ) = delete;

		void init( ) override;

		void render( ) override;

		bool think( ) override;

		void identify( parent_ptr parent ) override;
	};
} // namespace gui::objects