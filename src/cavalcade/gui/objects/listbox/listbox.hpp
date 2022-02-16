#pragma once

#include "../base/base.hpp"
namespace gui::objects {
	struct listbox : base_parent, base_child< uint32_t > {
		enum class preset
		{
			// will show all elements
			FULL,
			// show half of the elements
			HALF
		};

		using options = std::vector< std::string >;

	protected:

		// cool alias!!
		using options_info = helpers::children_info< helpers::list_option >;

		uint32_t m_max_vis_objs;

		int m_height;

		options m_options;

	public:

		listbox( ) = default;

		listbox( std::string_view label, const options& list, uint32_t max_vis_elements );

		listbox( std::string_view label, const options& list, preset preset = preset::FULL );

		virtual ~listbox( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void resize( const render::size& delta ) override;

	protected:

		// hide the scope of this function to avoid being able to write silly stuff like
		// listbox->add<checkbox>();
		// in gui::init
		using base_parent::add;
	};
} // namespace gui::objects