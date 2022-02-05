#ifndef DLIGHT_HPP
#define DLIGHT_HPP

#pragma once

#include "../../../../other/other.hpp"

namespace sdk::auxiliary {
	struct dlight {
		i32 m_flags;
		math::v3f m_origin;
		f32 m_radius;
		render::color m_color;
		f32 m_die;
		f32 m_decay;
		f32 m_minlight;
		i32 m_key;
		i32 m_style;

		math::v3f m_direction;
		f32 m_inner_angle;
		f32 m_outer_angle;
	};
} // namespace sdk::auxiliary

#endif /* DLIGHT_HPP */
