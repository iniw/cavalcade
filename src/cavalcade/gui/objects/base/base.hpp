#pragma once

namespace gui::objects {

	struct base_object;

	struct base_parent;

	template< typename T >
	concept Object = std::is_base_of_v< base_object, T > && !std::is_same_v< T, base_object >;

	constexpr auto CFG_SEPARATOR = ":";

	// implemented by base_object::m_flags
	enum flags
	{
		// hovered state
		HOVERED = 0,
		// set to true when we are being interacted with
		// if this is true think() returned true
		ACTIVE,
		// disables rendering and thinking
		DISABLED,
		// TODO
		INACTIVE,
		// draws rectangles around the object's bounds
		DEBUG_BOUNDS,
		// :)
		MAX
	};

	using base_ptr = std::unique_ptr< base_object >;

	using parent_ptr = std::unique_ptr< base_parent >;

	using flags_set = std::bitset< flags::MAX >;

	using callback_fn = std::function< void( base_ptr& ) >;
} // namespace gui::objects

#include "../../../../other/other.hpp"
#include "../../cfg/cfg.hpp"
#include "../../managers/managers.hpp"
#include "../../style/style.hpp"
#include "../../utilities/utilities.hpp"

#include "traits/base_traits.hpp"
#include "object/base_object.hpp"
#include "child/base_child.hpp"
#include "parent/base_parent.hpp"

#include "../../helpers/helpers.hpp"