#pragma once

#include "utl_vector.hpp"

namespace sdk {
	struct varmapentry {
		u16 m_type;
		u16 m_needs_interp;

		unk m_data;
		unk m_watcher;
	};

	struct varmapping {
		utl_vector< varmapentry > m_entries;
		i32 m_interp_entries;
		f32 m_last_interp_time;
	};
} // namespace sdk