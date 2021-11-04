#pragma once

namespace sdk {
	struct globals {
		f32 m_realtime;
		i32 m_framecount;
		f32 m_absoluteframetime;
		f32 m_absoluteframestarttime;
		f32 m_curtime;
		f32 m_frametime;
		i32 m_max_clients;
		i32 m_tickcount;
		f32 m_interval_per_tick;
		f32 m_interpolation_amount;
		i32 m_sim_ticks_this_frame;
		i32 m_network_protocol;
		unk m_save_data;
		bool m_client;
		bool m_remote_client;
		i32 m_time_stamp_networking_base;
		i32 m_time_stamp_randomize_window;
	};
} // namespace sdk