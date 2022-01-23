#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#pragma once

struct animation {
	f32 m_speed;
	std::function< f32( f32 ) > m_easing;
};

struct animator {
	void bake( bool increment, const animation& in, const animation& out ) {
		m_animation_state += increment ? in.m_speed * 0.001f : -out.m_speed * 0.001f;
		m_animation_state  = std::clamp( m_animation_state, 0.f, 1.f );
		m_animation_factor = increment ? in.m_easing( m_animation_state ) : out.m_easing( m_animation_state );
		m_animation_factor = std::clamp( m_animation_factor, 0.f, 1.f );
	}

	f32 m_animation_factor;
	f32 m_animation_state;
};

namespace easing {
	static f32 out_expo( f32 delta ) {
		return 1 - pow( 2, -8 * delta );
	}
} // namespace easing

#endif /* ANIMATOR_HPP */
