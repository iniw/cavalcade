#pragma once

namespace render {
	struct rect {
		i32 x, y, w, h;

		constexpr rect( ) = default;

		constexpr rect( i32 x, i32 y, i32 w, i32 h ) : x( x ), y( y ), w( w ), h( h ) { }

		constexpr rect( const point& pos, const size& size ) : x( pos.x ), y( pos.y ), w( size.w ), h( size.h ) { }

		rect( const point& pos1, const point& pos2 ) {
			x = std::min( pos1.x, pos2.x );
			y = std::min( pos1.y, pos2.y );

			w = abs( pos2.x - pos1.x );
			h = abs( pos2.y - pos1.y );
		}

		constexpr point pos( ) const {
			return { x, y };
		}

		constexpr void set_pos( const point& p ) {
			x = p.x;
			y = p.y;
		}

		constexpr void add_pos( const point& p ) {
			x += p.x;
			y += p.y;
		}

		constexpr size size( ) const {
			return { w, h };
		}

		constexpr void set_size( const render::size& s ) {
			w = s.w;
			h = s.h;
		}

		constexpr void add_size( const render::size& s ) {
			w += s.w;
			h += s.h;
		}

		constexpr i32 x2( ) const {
			return x + w;
		}

		constexpr i32 y2( ) const {
			return y + h;
		}

		constexpr point max( ) const {
			return { x2( ), y2( ) };
		}

		constexpr void vertical_cut( i32 v, bool centered = false ) {
			y += v;
			if ( centered )
				h -= v * 2;
			else
				h -= v;
		}

		constexpr void horizontal_cut( i32 v, bool centered = false ) {
			x += v;
			if ( centered )
				w -= v * 2;
			else
				w -= v;
		}

		constexpr rect expand( const render::size& size, bool centered = true ) const {
			if ( centered ) {
				return { x - size.w, y - size.h, w + size.w * 2, h + size.h * 2 };
			} else {
				return { x, y, w + size.w, h + size.h };
			}
		}

		constexpr rect shrink( const render::size& size, bool centered = true ) const {
			return expand( { -size.w, -size.h }, centered );
		}

		constexpr rect expand( i32 amount, bool centered = true ) const {
			if ( centered ) {
				return { x - amount, y - amount, w + amount * 2, h + amount * 2 };
			} else {
				return { x, y, w + amount, h + amount };
			}
		}

		constexpr rect shrink( i32 amount, bool centered = true ) const {
			return expand( -amount, centered );
		}

		constexpr rect offset( i32 amt ) {
			rect tmp = *this;
			tmp.x += amt;
			tmp.y += amt;
			return tmp;
		}

		bool hovered( ) const;

		f32 mouse_percent_x( bool uncapped = false ) const;
		f32 mouse_percent_y( bool uncapped = false ) const;

		void clamp_to( const rect& rect );
	};
} // namespace render