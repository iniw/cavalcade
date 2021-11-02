#ifndef BASE_HPP
#define BASE_HPP

// gives access to the rendering and utility functions to every object
#include "../../../other/other.hpp"

namespace gui::objects {
	struct base_object;
	using base_ptr = std::shared_ptr< base_object >;

	struct base_parent;
	using parent_ptr = std::shared_ptr< base_parent >;

	template< typename T >
	concept object = std::is_base_of_v< base_object, T > && !std::is_same_v< T, base_object >;

	// NOTE(wini): not sure how useful this is...
	enum class type
	{
		NONE = 0,
		WINDOW,
		CHECKBOX
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

	// NOTE(wini): unused rn, can maybe go back to it later
	// used by every object that inherits from base_child
	// bool: used by - checkbox
	// i32: used by - slider
	// f32: used by - slider
	// using child_var = std::variant< bool, i32, f32 >;

	// used in base_object::identify() to separate the names of objects
	constexpr cstr NAME_SEPARATOR = ":";

} // namespace gui::objects

#include "base_object.hpp"
#include "base_child.hpp"
#include "base_parent.hpp"

#endif // BASE_HPP