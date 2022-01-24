#ifndef NIGHTMODE_HPP
#define NIGHTMODE_HPP

#pragma once

#include "../../../other/other.hpp"

namespace sdk {
	struct static_prop;
	struct material;
} // namespace sdk

namespace hack {
	struct static_prop {
		sdk::static_prop* m_ptr;
		math::v4f m_base_color;
	};

	struct nightmode {
		void add( sdk::static_prop* prop );
		void clear( );
		void run_props( );
		void run( sdk::material* mat, f32& r, f32& g, f32& b );

		// NOTE(para): currently we're only doing darkness
		// std::optional< math::v4f > m_old_color;
		std::optional< i32 > m_old_factor;

		bool m_set{false};
		bool m_reset{false};
		std::vector< static_prop > m_static_props;
	};
} // namespace hack

#endif /* NIGHTMODE_HPP */
