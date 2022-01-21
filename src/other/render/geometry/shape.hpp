#pragma once

#include "../types/color.hpp"

struct ImDrawList;

namespace render::geometry {
	struct base_shape {
	protected:

		color m_color;

	public:

		virtual ~base_shape( ) = default;

		virtual void draw( ) = 0;

		static inline ImDrawList* s_draw_list = nullptr;
	};

	template< typename T >
	concept Shape = std::is_base_of_v< base_shape, T > && !std::is_same_v< T, base_shape >;
} // namespace render::geometry