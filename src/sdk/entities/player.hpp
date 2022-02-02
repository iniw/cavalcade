#ifndef PLAYER_HPP
#define PLAYER_HPP

#pragma once

#include "../types/interfaces/auxiliary/player_info.hpp"

namespace sdk {
	struct cs_player;
	struct player {
		player( ) = default;
		player( sdk::cs_player* p );
		player( const player& ) = default;

		sdk::cs_player& get( ) const;
		const sdk::auxiliary::player_info_t& get_player_info( ) const;
		std::string_view get_name( ) const;

		operator sdk::cs_player*( ) const {
			return m_player;
		}

		constexpr auto valid( ) const {
			return m_player && ( uintptr_t )m_player > 0x10000;
		}

	private:

		sdk::cs_player* m_player;
		sdk::auxiliary::player_info_t m_player_info;
	};
} // namespace sdk

#endif /* PLAYER_HPP */
