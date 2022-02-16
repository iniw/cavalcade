#pragma once

#include "../base/base.hpp"
#include "../checkbox/checkbox.hpp"

namespace gui::objects {
	struct master_checkbox : checkbox, base_parent {
	protected:

		struct {
			render::rect dyn;
			render::rect stt;
		} m_checkbox_area;

	public:

		master_checkbox( ) = default;

		master_checkbox( std::string_view label ) : checkbox( label ), base_object( label ) { }

		virtual ~master_checkbox( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void animate( ) override;

		virtual void on_add_child( base_object* child ) override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override;

		void on_interaction( );
	};
} // namespace gui::objects