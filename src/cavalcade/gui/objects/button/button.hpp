#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct button : virtual base_object {
	public:

		button( ) = default;

		button( std::string_view label, callback_fn fn );

		virtual ~button( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void resize( const render::size& delta ) override;
	};
} // namespace gui::objects