#ifndef GRAPH_HPP
#define GRAPH_HPP

#pragma once

namespace hack {
	struct graph {
		struct data {
			i32 m_speed;
			bool m_on_ground;
		};

		std::deque< data > m_data;

		void gather( );

		void draw( );

		void clear( );

		i32 m_old_gs{ -1 };
		f32 m_old_gsc{ -1.F };

		i32 get_upmost_y_scenario( );
		i32 get_bottommost_y_scenario( );
	};
} // namespace hack

#endif /* GRAPH_HPP */
