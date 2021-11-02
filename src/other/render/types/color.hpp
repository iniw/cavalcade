#ifndef COLOR_HPP
#define COLOR_HPP

namespace render {
	struct color {
		u8 r, g, b, a;

		constexpr color( u8 r = 255, u8 g = 255, u8 b = 255, u8 a = 255 ) : r{ r }, g{ g }, b{ b }, a{ a } { };

		constexpr u32 to_u32( ) const {
			return ( ( r & 0xff ) << 24 ) + ( ( g & 0xff ) << 16 ) + ( ( b & 0xff ) << 8 ) + ( a & 0xff );
		}

		constexpr u32 to_imgui( ) const {
			return ( ( a & 0xff ) << 24 ) + ( ( b & 0xff ) << 16 ) + ( ( g & 0xff ) << 8 ) + ( r & 0xff );
		}

		constexpr color mod_alpha( u8 new_alpha ) {
			return color( r, g, b, new_alpha );
		}

		constexpr static color white( u8 alpha = 255 ) {
			return color( 255, 255, 255, alpha );
		}

		constexpr static color black( u8 alpha = 255 ) {
			return color( 0, 0, 0, alpha );
		}

		constexpr static color gray( u8 alpha = 255 ) {
			return color( 50, 50, 50, alpha );
		}

		constexpr static color red( u8 alpha = 255 ) {
			return color( 255, 0, 0, alpha );
		}
	};
} // namespace render

#endif // COLOR_HPP