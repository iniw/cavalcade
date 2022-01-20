#ifndef IVDEBUGOVERLAY_HPP
#define IVDEBUGOVERLAY_HPP

#pragma once

namespace sdk::interfaces {
	struct debug_overlay {
		int screen_position( const math::v3f& in, math::v3f& out ) {
			return mem::call_v_func< int, 13 >( this, std::cref( in ), std::ref( out ) );
		}
	};
} // namespace sdk::interfaces

#endif /* IVDEBUGOVERLAY_HPP */
