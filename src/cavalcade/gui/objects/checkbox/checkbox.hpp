#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct checkbox : base_child< bool > {
	protected:

		float m_alpha = 0.f;

	public:

		checkbox( ) = default;

		checkbox( std::string_view label ) : base_object( label ) { }

		virtual ~checkbox( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void animate( ) override;

		// we don't wanna resize
		virtual void resize( const render::size& delta ) override { }
	};
} // namespace gui::objects