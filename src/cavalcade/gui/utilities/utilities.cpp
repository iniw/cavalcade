#include "utilities.hpp"
#include "../objects/base/base.hpp"
#include "../../ctx/ctx.hpp"

render::point gui::utils::center_label( std::string_view label, const render::rect& area ) {
	auto label_size = g_render.text_size< render::font::MENU >( label );

	return { ( area.x + area.w / 2 ) - label_size.w + label_size.w / 2, ( area.y + area.h / 2 ) - label_size.h + label_size.h / 2 };
}

void gui::utils::animate( float& val, float goal, float speed ) {
	val = std::lerp( val, goal, speed * g_csgo.m_globals->m_frametime );
}