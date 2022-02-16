#pragma once

namespace gui::objects {
	template<typename T>
	struct base_traits {
		using ptr = std::shared_ptr<T>;
	};
}