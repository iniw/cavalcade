#ifndef HANDLE_INL
#define HANDLE_INL

#include "../csgo/csgo.hpp"

template< typename T >
T sdk::handle::get( ) const {
	return g_csgo.m_ent_list->get_handle< T >( *this );
}

#endif // HANDLE_INL