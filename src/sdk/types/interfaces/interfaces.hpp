#pragma once

#include "../types.hpp"

#include "entities/client_entity.hpp"
#include "entities/client_networkable.hpp"
#include "entities/client_collideable.hpp"
#include "entities/client_renderable.hpp"
#include "entities/client_thinkable.hpp"
#include "entities/client_alpha_property.hpp"
#include "entities/client_unknown.hpp"

#include "client_dll.hpp"
#include "cvars.hpp"
#include "engine.hpp"
#include "ent_list.hpp"
#include "prediction.hpp"
#include "mdl_cache.hpp"
#include "game_movement.hpp"
#include "input.hpp"
#include "ivdebugoverlay.hpp"
#include "enginetrace.hpp"
#include "physicssurfaceprops.hpp"
#include "enginesound.hpp"

namespace sdk::interfaces {
	struct registry {
		using instantiate_interface_fn = unk( __cdecl* )( );

		instantiate_interface_fn m_create_fn;
		cstr m_name;
		registry* m_next;
	};
} // namespace sdk::interfaces