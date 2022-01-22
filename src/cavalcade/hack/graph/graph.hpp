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

		i32 get_upmost_y_scenario( );
	};
} // namespace hack

#endif /* GRAPH_HPP */
