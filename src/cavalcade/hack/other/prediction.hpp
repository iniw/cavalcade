#ifndef PREDICTION_HPP
#define PREDICTION_HPP

#include "../../ctx/ctx.hpp"

namespace hack::other {
	struct prediction {
	private:

		f32 m_backup_curtime;
		f32 m_backup_frametime;

		bool m_backup_is_first_time_predicted;
		bool m_backup_in_prediction;

		sdk::move_data m_move_data;

	public:

		void start( );

		void end( );

		void update( );
	};

} // namespace hack::other

#endif // PREDICTION_HPP
