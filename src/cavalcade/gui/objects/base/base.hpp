#pragma once

// gives access to the rendering and utility functions to every object
#include "../../../other/other.hpp"

namespace gui::objects {
	struct base_object;
	using base_ptr = std::shared_ptr< base_object >;

	struct base_parent;
	using parent_ptr = std::shared_ptr< base_parent >;

	template< typename T >
	concept Object = std::is_base_of_v< base_object, T > && !std::is_same_v< T, base_object >;

	// NOTE(wini): not sure how useful this is...
	enum class type
	{
		NONE = 0,
		CHECKBOX,
		GROUPBOX,
		WINDOW
	};

	// list of flags that every object has
	// implemented by base_object::m_flags
	enum flags
	{
		// set to true when hovered
		HOVERED = 0,
		// set to true when we are being interacted with
		// if this is active think() returned true
		ACTIVE,
		// mostly a debugging flag
		// prevents think() from being called
		DISABLED,
		// :)
		MAX
	};

	using flags_set = std::bitset< flags::MAX >;

	// used in base_object::identify() to separate the names of objects
	constexpr cstr NAME_SEPARATOR = ":";

} // namespace gui::objects

#include "base_object.hpp"
#include "base_child.hpp"
#include "base_parent.hpp"
#include "base_styling.hpp"