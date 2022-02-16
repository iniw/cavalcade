#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct groupbox : base_parent {
		enum class preset
		{
			// we'll take up the entire area of our parent
			FULL,
			// we'll take up half of the area of our parent
			HALF,
			// we'll update our height on every child added
			DYNAMIC
		};

	public:

		int m_height;
		preset m_preset;

	public:

		groupbox( ) = default;

		groupbox( std::string_view label, uint32_t height );

		groupbox( std::string_view label, preset preset );

		virtual ~groupbox( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void on_add_child( base_object* child ) override;

		virtual void resize( const render::size& delta ) override;
	};
} // namespace gui::objects