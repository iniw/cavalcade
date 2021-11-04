#pragma once

struct ImDrawList;

namespace render::geometry {
	struct base_shape {
	protected:

		u32 m_color;

	public:

		virtual ~base_shape( ) = default;

		virtual void draw( ) = 0;

		static inline ImDrawList* s_draw_list = nullptr;
	};

	struct safe_shape {
	protected:

		// "Ah! Yes!" Yelled the gentlman, as he rushed into the computer;
		// "A queue of queues!" - his excitement was immeasurable.
		std::vector< std::unique_ptr< base_shape > > m_drawlist;

	public:

		virtual ~safe_shape( ) = default;

		virtual void draw( ) = 0;
	};

	template< typename T >
	concept Shape = std::is_base_of_v< base_shape, T > && !std::is_same_v< T, base_shape >;
} // namespace render::geometry