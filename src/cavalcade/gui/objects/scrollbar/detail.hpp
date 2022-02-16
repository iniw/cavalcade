#pragma once

namespace gui::detail {
	namespace options {
		constexpr float amount{ 250.f };
		constexpr float overscroll_multiplier{ 0.4f };
	}

	namespace sizing {
		constexpr int main = 1;
		constexpr int main_hovered = 5;
	}

	namespace anim {
		constexpr float slowdown{ 15.f };
		constexpr float overscroll_reset_speed{ 15.f };
		constexpr float drag_speed{ 15.f };
		constexpr float bar_width{ 30.f };
	}
}