#pragma once

namespace sdk::interfaces {
	struct client_unknown;

	struct client_thinkable {
		virtual client_unknown* get_client_unknown( ) = 0;
		virtual void client_think( )                  = 0;
		virtual unk get_think_handle( )               = 0;
		virtual void set_think_handle( unk think )    = 0;
		virtual void release( )                       = 0;
	};
} // namespace sdk::interfaces