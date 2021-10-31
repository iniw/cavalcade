#ifndef CLIENT_STATE_HPP
#define CLIENT_STATE_HPP

namespace sdk {
	struct client_state {
		PAD( 0x9c );
		unk m_net_channel;
		i32 m_challenge_nr;
		PAD( 0x64 );
		i32 m_signon_state;
		PAD( 0x8 );
		f32 m_next_cmd_time;
		i32 m_server_count;
		i32 m_current_sequence;
		PAD( 0x54 );
		i32 m_delta_tick;
		bool m_paused;
		PAD( 0x7 );
		i32 m_view_entity;
		i32 m_player_slot;
		char m_level_name[ max_path ];
		char m_level_name_short[ 80 ];
		char m_map_group_name[ 80 ];
		char m_last_level_name_short[ 80 ];
		PAD( 0xC );
		i32 m_max_clients;
		PAD( 0x498c );
		f32 m_last_server_tick_time;
		bool m_in_simulation;
		PAD( 0x3 );
		i32 m_old_tickcount;
		f32 m_tick_remainder;
		f32 m_frame_time;
		i32 m_last_outgoing_command;
		i32 m_choked_commands;
		i32 m_last_command_ack;
		i32 m_command_ack;
		i32 m_sound_sequence;
		PAD( 0x50 );
		math::ang ang_view_point;
		PAD( 0xD0 );
		unk m_events;
	};
} // namespace sdk

#endif // CLIENT_STATE_HPP
