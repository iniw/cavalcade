#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct colorpicker : base_child< render::color >, helpers::popup {
	private:

		enum class clicked_item
		{
			NONE,
			SATURATION,
			HUE,
			ALPHA
		};

		float m_hue        = 0.f;
		float m_saturation = 0.f;
		float m_brightness = 0.f;

		float m_visual_hue        = 0.f;
		float m_visual_saturation = 0.f;
		float m_visual_brightness = 0.f;
		float m_visual_alpha      = 1.f;

		clicked_item m_clicking;
		bool m_init;
		bool m_attach;

	protected:

		render::rect m_hue_area;
		render::rect m_sat_area;
		render::rect m_alpha_area;
		render::rect m_og_col_area;

	public:

		colorpicker( ) = default;

		colorpicker( std::string_view label, bool attach_to_last = true ) : base_object( label ), m_attach{ attach_to_last } { }

		virtual ~colorpicker( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void animate( ) override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override { }
	};
} // namespace gui::objects