#ifndef RENDER_TYPES_HPP
#define RENDER_TYPES_HPP

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>

#include "color.hpp"

#include "../../math/math.hpp"

namespace render {
	using point = math::v2i;
	using size  = math::v2i;
	using rect  = math::v4i;
} // namespace render

#endif // RENDER_TYPES_HPP