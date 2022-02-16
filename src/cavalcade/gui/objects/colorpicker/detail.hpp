#pragma once

namespace gui::detail {
	namespace padding {
		constexpr int inner_margin = style::padding::margin * 2 + style::padding::obj_spacing;
	} // namespace padding

	namespace sizing {
		// size of the picker itself
		constexpr render::size main = { style::sizing::button.w, style::sizing::button.h / 2 };
		// size of the entire popup
		constexpr render::size popup = { 150, 150 };
		// size of the hue/alpha sliders
		constexpr int slider_size = style::sizing::button.w / 2;

		constexpr render::size sat    = popup.shrink( padding::inner_margin ).shrink( slider_size );
		constexpr render::size hue    = { slider_size, popup.h - padding::inner_margin - slider_size };
		constexpr render::size alpha  = { popup.h - padding::inner_margin - slider_size, slider_size };
		constexpr render::size og_col = { slider_size, slider_size };

		constexpr int indicator_size = 3;
	} // namespace sizing

	namespace anim {
		constexpr float cursor_speed{ 25.f };
		constexpr float open_speed{ 15.f };
		constexpr float close_speed{ 25.f };
	} // namespace anim

	namespace palette {
		constexpr render::color border{ 0x323232FF };
		constexpr render::color cursor_shadow = render::color::black( 50 );
	} // namespace palette
} // namespace gui::detail