

#pragma once

#include "imatrendercontext.hpp"

namespace sdk {
	struct pix_event {
		pix_event( mat_render_context* ctx, cstr name, unsigned long color = 0xFFF5940F ) {
			m_render_context = ctx;
			ctx->begin_pix_event( color, name );
		}

		~pix_event( ) {
			m_render_context->end_pix_event( );
		}

		mat_render_context* m_render_context;
	};
} // namespace sdk

#define PIXEVENT sdk::pix_event _pixEvent