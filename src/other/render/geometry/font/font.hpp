#ifndef FONT_HPP
#define FONT_HPP

#include "../shape.hpp"

struct ImFont;

namespace render::geometry {
	struct font {
	private:

		ImFont* m_font;
		f32 m_size;

	public:

		font( ) = default;

		bool init( std::string_view path, f32 size );

		void draw( const point& pos, std::string_view text, color col );

		size calc_size( std::string_view text );
	};
} // namespace render::geometry

#endif // FONT_HPP