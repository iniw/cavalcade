#pragma once

#include "../csgo/csgo.hpp"

template< typename T >
T sdk::handle::get( ) const {
	return g_csgo.m_ent_list->get_handle< T >( *this );
}

#pragma once