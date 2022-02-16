#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	// an almost proxy-like object that serves only as a positioning tool
	struct column : base_parent {
		column( ) = default;

		virtual ~column( ) = default;

		virtual void init( ) override;

		virtual void render( ) const override;

		virtual bool think( ) override;

		virtual void on_add_child( base_object* child ) override;
	};
} // namespace gui::objects