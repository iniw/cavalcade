#ifndef SUNSET_HPP
#define SUNSET_HPP

#pragma once

namespace hack {
	struct sunset {
		void run( );
		void reset( );

		bool m_update{ true };
		math::v3f m_light_shadow_direction;
		f32 m_max_shadow_dist{ };
	};
} // namespace hack

#endif /* SUNSET_HPP */
