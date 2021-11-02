#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "../base/base.hpp"

namespace gui::objects {
	// TODO(wini and para): a good, per-object config system that doesn't rely
	// on external variables and is also easily accessible from outside the object
	// ideal syntax:
	// (from outside the object) - g_gui.get_var< type >( an easy to type and unique identifier )
	// (from inside the object) - set_var() / get_var()
	struct checkbox : public base_child< bool > {
		using ptr = std::shared_ptr< checkbox >;

		checkbox( std::string_view name, std::string_view label );

		void init( ) override;

		void render( ) override;

		bool think( ) override;
	};
} // namespace gui::objects

#endif // CHECKBOX_HPP