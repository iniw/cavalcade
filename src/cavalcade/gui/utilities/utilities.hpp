#pragma once

namespace gui::utils {
	render::point center_label( std::string_view label, const render::rect& area );

	void animate( float& val, float goal, float speed );
} // namespace gui::utils