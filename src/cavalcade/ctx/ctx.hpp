#ifndef CTX_HPP
#define CTX_HPP

#include "../../other/other.hpp"
#include "../../sdk/sdk.hpp"

namespace cavalcade {
	enum flags
	{
		LOCAL    = ( 1 << 0 ),
		DEAD     = ( 1 << 1 ),
		DORMANT  = ( 1 << 2 ),
		TEAMMATE = ( 1 << 3 ),
		IMMUNE   = ( 1 << 4 )
	};

	struct ctx {
		sdk::cs_player* m_local;
		sdk::user_cmd* m_cmd;

		struct {
			sdk::cvar* cl_updaterate;
			sdk::cvar* sv_minupdaterate;
			sdk::cvar* sv_maxupdaterate;

			sdk::cvar* cl_interp;
			sdk::cvar* cl_interp_ratio;
			sdk::cvar* sv_client_min_interp_ratio;
			sdk::cvar* sv_client_max_interp_ratio;

			sdk::cvar* sv_maxunlag;

			sdk::cvar* sv_showlagcompensation_duration;

			sdk::cvar* sv_autobunnyhopping;

			sdk::cvar* mp_teammates_are_enemies;

			sdk::cvar* sensitivity;

			sdk::cvar* m_pitch;
			sdk::cvar* m_yaw;

		} m_cvars;

		bool init( );

		void iter_players( const std::function< void( sdk::cs_player* ) >& fn, i32 flags = 0 );
	};
} // namespace cavalcade

inline cavalcade::ctx g_ctx;

#endif // CTX_HPP
