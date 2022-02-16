#include "../render.hpp"

u32 render::color::convert( ) const {
	auto ret = *this;

	// color override
	if ( !g_render.alphas( ).empty( ) )
		ret = ret.mod_alpha( g_render.alphas( ).top( ) );

	return ret.to_imgui( );
}

f32 render::color::hue( ) const {
	if ( m_r == m_g && m_g == m_b )
		return 0.f;

	const f32 red   = m_r / 255.f;
	const f32 green = m_g / 255.f;
	const f32 blue  = m_b / 255.f;

	const f32 hue_max = std::max( red, std::max( green, blue ) ), hue_min = std::min( red, std::min( green, blue ) );

	if ( hue_max == hue_min )
		return 0.f;

	const f32 hue_delta = hue_max - hue_min;
	f32 hue             = 0.f;

	if ( hue_max == red )
		hue = ( green - blue ) / hue_delta;
	else if ( hue_max == green )
		hue = 2.f + ( blue - red ) / hue_delta;
	else if ( hue_max == blue )
		hue = 4.f + ( red - green ) / hue_delta;

	hue *= 60.f;

	if ( hue < 0.f )
		hue += 360.f;

	return hue / 360.f;
}

f32 render::color::saturation( ) const {
	const f32 red   = m_r / 255.f;
	const f32 green = m_g / 255.f;
	const f32 blue  = m_b / 255.f;

	const f32 sat_max = std::max( red, std::max( green, blue ) ), sat_min = std::min( red, std::min( green, blue ) );
	const f32 sat_delta = sat_max - sat_min;

	if ( sat_max == 0.f )
		return sat_delta;

	return sat_delta / sat_max;
}

f32 render::color::brightness( ) const {
	return std::max( m_r, std::max( m_g, m_b ) ) / 255.f;
}

render::color render::color::from_hsb( f32 hue, f32 sat, f32 brightness ) {
	const f32 h = fmodf( hue, 1.f ) / ( 60.f / 360.f );
	const i32 i = static_cast< i32 >( h );
	const f32 f = h - static_cast< f32 >( i );
	const f32 p = brightness * ( 1.f - sat );
	const f32 q = brightness * ( 1.f - sat * f );
	const f32 t = brightness * ( 1.f - sat * ( 1.f - f ) );

	f32 r = 0.f, g = 0.f, b = 0.f;

	switch ( i ) {
	case 0:
		r = brightness, g = t, b = p;
		break;
	case 1:
		r = q, g = brightness, b = p;
		break;
	case 2:
		r = p, g = brightness, b = t;
		break;
	case 3:
		r = p, g = q, b = brightness;
		break;
	case 4:
		r = t, g = p, b = brightness;
		break;
	case 5:
	default:
		r = brightness, g = p, b = q;
		break;
	}

	return color( r * 255, g * 255, b * 255 );
}

std::tuple< f32, f32, f32 > render::color::to_hsb( ) const {
	return { hue( ), saturation( ), brightness( ) };
}