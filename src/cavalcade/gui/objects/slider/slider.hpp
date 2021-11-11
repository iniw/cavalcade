#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	template< math::Number T >
	struct slider : public base_child< T >, public base_traits< slider< T > > {
	protected:

		bool m_dragging;

		T m_min, m_max;

		i32 m_precision;

		render::point m_var_text_pos;
		std::string m_var_text;

	public:

		slider( std::string_view name, std::string_view label, T min, T max, i32 precision = 2 );

		void init( ) override;

		void render( ) const override;

		bool think( ) override;

		void reposition( const render::point& delta ) override;

		void resize( const render::point& delta ) override;
	};
} // namespace gui::objects

#include "slider.inl"