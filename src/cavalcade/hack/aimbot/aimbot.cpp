#include "aimbot.hpp"
#include "../../ctx/ctx.hpp"

math::ang hack::aimbot::pixels_to_angle( float x, float y ) {
	float px = x * g_ctx.m_cvars.m_pitch->get_float( );
	float py = y * g_ctx.m_cvars.m_yaw->get_float( );

	return math::ang( -x, -y, 0.F ).clamp_angle( );
}

math::ang hack::aimbot::angle_to_pixels( const math::ang& angle ) {
	float x = angle.pitch / g_ctx.m_cvars.m_pitch->get_float( );
	float y = angle.yaw / g_ctx.m_cvars.m_yaw->get_float( );

	return math::ang( -y, x, 0.F );
}

// aimbot
// run players
// get hitbox pos
// aim angle = calc angle eye pos (shoot pos) -> hitbox pos
// get view angles
// view delta = (aim angle - view angle).clamp_angle();
// move angle = pixels to angle(x, y)
// move angle *= strength
// move angle /= smoothing
// delta_x = abs(move angle.x)
// delta_y = abs(move angle.y)
// pixels = angle_to_pixels(std::clamp(view delta x, -delta_x, delta_x), std::clamp(view delta y, -delta_y, delta_y))
// if (view_delta.length2d() <= fov)
//  x += pixels.pitch
//  y += pixels.yaw
void hack::aimbot::run( float& x, float& y ) { }