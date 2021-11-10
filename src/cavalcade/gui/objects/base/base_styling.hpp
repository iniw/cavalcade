#pragma once

// general styling, used by every object
namespace gui::general {
	// NOTE(wini): these are namespaces so that they can be extended by each object
	namespace pallete {
		constexpr inline render::color primary{ 0x1A1A1AFF };
		constexpr inline render::color secondary{ 0x666666FF };
		constexpr inline render::color highlight{ 0x6AF2F0FF };
		constexpr inline render::color text{ 0xFFFFFFFF };
	}; // namespace pallete

	namespace padding {
		constexpr inline i32 margin{ 5 };
		constexpr inline i32 obj_margin{ 4 };
	}; // namespace padding
} // namespace gui::general