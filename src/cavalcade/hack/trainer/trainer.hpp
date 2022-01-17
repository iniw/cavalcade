#ifndef TRAINER_HPP
#define TRAINER_HPP

#pragma once

#include "../../../other/other.hpp"

namespace hack {
	struct trainer {
		std::optional< std::pair< math::v3f, math::ang > > m_checkpoint;

		void go( );
		void run( );
	};
} // namespace hack

#endif /* TRAINER_HPP */
