#pragma once

#include "handle_entity.hpp"

namespace sdk::interfaces {
	struct client_collideable;
	struct client_networkable;
	struct client_renderable;
	struct client_entity;
	struct base_entity;
	struct client_thinkable;
	struct client_alpha_property;

	struct client_unknown : handle_entity {
		virtual client_collideable* get_collideable( )              = 0;
		virtual client_networkable* get_client_networkable( )       = 0;
		virtual client_renderable* get_client_renderable( )         = 0;
		virtual client_entity* get_client_entity( )                 = 0;
		virtual base_entity* get_base_entity( )                     = 0;
		virtual client_thinkable* get_client_thinkable( )           = 0;
		virtual client_alpha_property* get_client_alpha_property( ) = 0;
	};
} // namespace sdk::interfaces