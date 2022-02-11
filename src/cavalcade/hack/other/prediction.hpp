#pragma once

#include "../../../other/other.hpp"
#include "../../../sdk/types/move_data.hpp"

namespace hack::other {
	struct prediction {
	private:

		f32 m_backup_curtime;
		f32 m_backup_frametime;

		bool m_backup_is_first_time_predicted;
		bool m_backup_in_prediction;

	public:

		bool m_prevent_sounds{ false };

		sdk::move_data m_move_data;

		void start( );

		void apply( );

		void restore( );

		void update( );
	};
} // namespace hack::other