#pragma once

#include "../../objects/base/base.hpp"

namespace gui::helpers {
	struct popup : virtual objects::base_object {
	protected:

		bool m_open        = false;
		float m_open_alpha = 0.f;
		render::rect m_popup_area;

	public:

		virtual bool think( ) override;

		virtual void animate( ) override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override;
	};
} // namespace gui::helpers