#pragma once

#include "../base/base.hpp"
#include "../listbox/listbox.hpp"
#include "../button/button.hpp"

namespace gui::objects {
	struct selectable : listbox, button, helpers::popup {
		selectable( ) = default;

		selectable( std::string_view label, const options& list, uint32_t max_vis_elements )
			: listbox( label, list, max_vis_elements ), base_object( label ) { }

		selectable( std::string_view label, const options& list, preset preset = preset::FULL )
			: listbox( label, list, preset ), base_object( label ) { }

		virtual ~selectable( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void animate( ) override;

		virtual void update_flags( ) override;

		virtual void render_debug_bounds( ) const override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override;

		void render_children( ) const;
	};
} // namespace gui::objects