#pragma once

#include "../../../other/other.hpp"
#include "../../../sdk/types/move_data.hpp"

namespace sdk {
	struct user_cmd;
} // namespace sdk

namespace hack::other {
	struct prediction {
	private:

		i32 m_backup_tickbase{ };
		f32 m_backup_curtime{ };
		f32 m_backup_frametime{ };

		i32 m_correct_tickbase{ };

		sdk::user_cmd* m_last_cmd{ };

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