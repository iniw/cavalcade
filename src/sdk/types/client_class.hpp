#ifndef CLIENT_CLASS_HPP
#define CLIENT_CLASS_HPP

#include "dt_recv.hpp"

namespace sdk {
	struct client_class {
		using create_client_class_fn = unk( __cdecl* )( i32, i32 );
		using create_event_fn        = unk( __cdecl* )( );

		create_client_class_fn m_create;
		create_event_fn m_create_event;
		str m_network_name;
		recv_table* m_recv_table;
		client_class* m_next;
		class_id m_class_id;
	};
} // namespace sdk

#endif // CLIENT_CLASS_HPP
