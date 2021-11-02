#ifndef BASE_HPP
#define BASE_HPP

// gives access to the rendering and utility functions to every object
#include "../../../other/other.hpp"

namespace gui::objects {
	struct base_object;
	using base_ptr = std::shared_ptr< base_object >;

	struct base_parent;
	using parent_ptr = std::shared_ptr< base_parent >;

	template< typename T > concept object = std::is_base_of_v< base_object, T > && !std::is_same_v< T, base_object >;

	// NOTE(wini): not sure how useful this is...
	enum class type
	{
		NONE = 0,
		WINDOW
	};
} // namespace gui::objects

#include "base_object.hpp"
#include "base_parent.hpp"

#endif // BASE_HPP