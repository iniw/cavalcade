#pragma once

namespace gui::style {
	namespace palette {
		inline render::color primary{ 0, 0, 0, 255 };
		inline render::color secondary{ 25, 25, 25, 115 };
		inline render::color background{ 28, 28, 28, 235 };
		inline render::color stroke{ 35, 35, 35, 255 };
		inline render::color highlight{ 0x6F42F5FF };
		inline render::color text{ 0xFFFFFFFF };
		inline render::color debug_bounds_stt{ 0xFF0000FF };
		inline render::color debug_bounds_dyn{ 0x00FF00FF };
		inline render::color debug_bounds_other{ 0xFFFF00FF };
	} // namespace palette
} // namespace gui::style