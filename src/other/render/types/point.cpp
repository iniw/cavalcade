#include "point.hpp"
#include "rect.hpp"

bool render::point::in_rect( const rect& r ) const {
	return x >= r.x && y >= r.y && x <= r.x + r.w && y <= r.y + r.h;
}