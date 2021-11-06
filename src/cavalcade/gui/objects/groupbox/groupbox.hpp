#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct groupbox : public base_parent, public base_traits< groupbox > {
	protected:

		i32 m_height;

	public:

		// NOTE(wini): the default param of "-1" means "dynamic",
		// in the sense that the height will be updated on each add() call
		groupbox( std::string_view name, std::string_view label, i32 height = -1 );

		void init( ) override;

		void render( ) override;

		bool think( ) override;

		void on_add_child( base_ptr child ) override;
	};
} // namespace gui::objects