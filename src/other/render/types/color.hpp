#pragma once

namespace render {
	struct color {
		u8 m_r, m_g, m_b, m_a;

		constexpr color( ) = default;

		constexpr color( u8 r, u8 g, u8 b, u8 a = 255 ) : m_r{ r }, m_g{ g }, m_b{ b }, m_a{ a } { };

		constexpr color( u32 hex ) : m_r( ( hex >> 24 ) & 0xFF ), m_g( ( hex >> 16 ) & 0xFF ), m_b( ( hex >> 8 ) & 0xFF ), m_a( hex & 0xFF ){ };

		static color from_hsb( f32 hue, f32 sat, f32 brightness );

		u32 convert( ) const;

		f32 hue( ) const;

		f32 saturation( ) const;

		f32 brightness( ) const;

		std::tuple< f32, f32, f32 > to_hsb( ) const;

		constexpr u32 to_imgui( ) const {
			return ( ( m_a & 0xFF ) << 24 ) | ( ( m_b & 0xFF ) << 16 ) | ( ( m_g & 0xFF ) << 8 ) | ( m_r & 0xFF );
		}

		constexpr u32 to_u32( ) const {
			return ( ( m_r & 0xff ) << 24 ) | ( ( m_g & 0xff ) << 16 ) | ( ( m_b & 0xff ) << 8 ) | ( m_a & 0xff );
		}

		constexpr color mod_alpha( u8 alpha ) {
			return color( m_r, m_g, m_b, alpha );
		}

		constexpr color mod_alpha( f32 alpha ) {
			return color( m_r, m_g, m_b, m_a * std::clamp( alpha, 0.f, 1.f ) );
		}

		constexpr color frac_alpha( f32 mod ) {
			return color( m_r, m_g, m_b, m_a * std::min( mod, 1.F ) );
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