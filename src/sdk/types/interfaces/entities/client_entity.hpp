#ifndef CLIENT_ENTITY_HPP
#define CLIENT_ENTITY_HPP

#include "client_collideable.hpp"
#include "client_networkable.hpp"
#include "client_renderable.hpp"
#include "client_thinkable.hpp"
#include "client_unknown.hpp"

namespace sdk::interfaces {
	struct client_entity : client_unknown, client_renderable, client_networkable, client_thinkable {
		virtual void release( ) override                 = 0;
		virtual const math::v3f& get_abs_origin( ) const = 0;
		virtual const math::ang& get_abs_angles( ) const = 0;

		data_map* get_data_map( ) { return mem::call_v_func< data_map*, 15 >( this ); }

		data_map* get_pred_data_map( ) { return mem::call_v_func< data_map*, 17 >( this ); }
	};
} // namespace sdk::interfaces

#endif // CLIENT_ENTITY_HPP
