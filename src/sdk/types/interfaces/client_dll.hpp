#pragma once

#include "../client_class.hpp"

namespace sdk::interfaces {
	struct client {
		enum idx
		{
			GET_ALL_CLASSES      = 8,
			GET_STD_RECV_PROXIES = 48
		};

		VFUNC( client_class*, get_all_classes, idx::GET_ALL_CLASSES, ( ) );

		VFUNC( std_recv_proxies*, get_std_recv_proxies, idx::GET_STD_RECV_PROXIES, ( ) );
	};
} // namespace sdk::interfaces