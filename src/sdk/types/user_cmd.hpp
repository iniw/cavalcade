#pragma once

#include "../../other/lib/include/crc32/crc32.hpp"

namespace sdk {
	struct user_cmd {
		PAD( 0x4 );
		i32 m_command_number;
		i32 m_tick_count;
		math::ang m_view_angles;
		math::v3f m_aim_direction;
		f32 m_forward_move;
		f32 m_side_move;
		f32 m_up_move;
		i32 m_buttons;
		byte m_impulse;
		i32 m_weapon_select;
		i32 m_weapon_sub_type;
		i32 m_random_seed;
		i16 m_moused_x;
		i16 m_moused_y;
		bool m_has_been_predicted;

		CRC32_t get_checksum( ) const {
			CRC32_t crc;
			CRC32_Init( &crc );

			CRC32_ProcessBuffer( &crc, &m_command_number, sizeof( m_command_number ) );
			CRC32_ProcessBuffer( &crc, &m_tick_count, sizeof( m_tick_count ) );
			CRC32_ProcessBuffer( &crc, &m_view_angles, sizeof( m_view_angles ) );
			CRC32_ProcessBuffer( &crc, &m_aim_direction, sizeof( m_aim_direction ) );
			CRC32_ProcessBuffer( &crc, &m_forward_move, sizeof( m_forward_move ) );
			CRC32_ProcessBuffer( &crc, &m_side_move, sizeof( m_side_move ) );
			CRC32_ProcessBuffer( &crc, &m_up_move, sizeof( m_up_move ) );
			CRC32_ProcessBuffer( &crc, &m_buttons, sizeof( m_buttons ) );
			CRC32_ProcessBuffer( &crc, &m_impulse, sizeof( m_impulse ) );
			CRC32_ProcessBuffer( &crc, &m_weapon_select, sizeof( m_weapon_select ) );
			CRC32_ProcessBuffer( &crc, &m_weapon_sub_type, sizeof( m_weapon_sub_type ) );
			CRC32_ProcessBuffer( &crc, &m_random_seed, sizeof( m_random_seed ) );
			CRC32_ProcessBuffer( &crc, &m_moused_x, sizeof( m_moused_x ) );
			CRC32_ProcessBuffer( &crc, &m_moused_y, sizeof( m_moused_y ) );

			CRC32_Final( &crc );
			return crc;
		}
	};
} // namespace sdk