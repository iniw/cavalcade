#pragma once

namespace gui::detail {
	namespace sizing {
		constexpr int width = 70;
		constexpr int min_popup_width = 100;
		int max_popup_height() {
			return helpers::list_option::height() * 10;
		}
	}

	namespace padding {
		constexpr int popup_gap = 3;
	}
}