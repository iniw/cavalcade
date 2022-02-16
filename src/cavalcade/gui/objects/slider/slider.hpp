#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	template< math::Number T >
	struct slider : base_child< T > {
	protected:

		bool m_dragging = false;

		T m_min, m_max;

		uint8_t m_precision;

		std::string m_var_text;
		render::point m_var_text_pos;

		float m_val_width = 0.f;

	public:

		slider( ) = default;

		slider( std::string_view label, T min, T max, uint8_t precision = 2 );

		virtual ~slider( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void animate( ) override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override;
	};
} // namespace gui::objects

#include "slider.inl"