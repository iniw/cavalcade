#pragma once

#include "client_collideable.hpp"
#include "client_networkable.hpp"
#include "client_renderable.hpp"
#include "client_thinkable.hpp"
#include "client_unknown.hpp"

namespace sdk::interfaces {
	struct client_entity : client_unknown, client_renderable, client_networkable, client_thinkable {
		virtual void release( ) override                  = 0;
		virtual math::v3f& get_abs_origin( ) const        = 0;
		virtual const math::ang& get_abs_angles( ) const  = 0;
		virtual unk get_mouth( )                          = 0;
		virtual bool get_sound_spatialization( unk info ) = 0;
		virtual bool is_blurred( )                        = 0;

		VFUNC( data_map*, get_data_map, 15, ( ) );

		VFUNC( data_map*, get_pred_data_map, 17, ( ) );
	};
} // namespace sdk::interfaces