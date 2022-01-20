#pragma once

#include "../shape.hpp"
#include "../../types/types.hpp"

struct ImFont;

namespace render::geometry {
	struct font {
	private:

		ImFont* m_font;
		f32 m_size;

	public:

		font( ) = default;

		bool init( std::string_view path, f32 size );

		void draw( const point& pos, std::string_view text, color col ) const;

		size calc_size( std::string_view text ) const;
	};
} // namespace render::geometry