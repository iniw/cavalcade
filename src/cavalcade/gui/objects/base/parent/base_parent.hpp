#pragma once

#include "../../scrollbar/scrollbar.hpp"

namespace gui::objects {
	struct scrollbar;
	struct base_parent : virtual base_object {
	protected:

		// the position used to place new children
		render::point m_cursor;

		// our children manager
		// handles all the boilerplate iteration code for us
		managers::children m_children_mgr;

		// NOTE(wini): i could look into hiding this under another base layer,
		// since maybe not all parents will use it. but i think its fine like this
		scrollbar* m_scrollbar = nullptr;

	public:

		base_parent( ) : base_object( ) {
			m_children_mgr.set_parent( this );
		}

		virtual ~base_parent( ) = default;

		virtual void animate( ) override;

		virtual void reposition( const render::point& delta ) override;

		virtual void resize( const render::size& delta ) override;

		// NOTE(wini): this a bit of hack/workaround about the fact that you can't virtualize templated functions
		// it might be of interest to most objects that inherent from base_parent to automatically
		// initialize their scrollbar as soon as a child's area gets past it's own, BUT, that might not apply to
		// every object, e.g: tabs. so until i figure out something smarter this is going to stay here
		virtual void on_add_child( base_object* child );

		virtual void render_debug_bounds( ) const override;

		void push_cursor( int offset );

		void add_to_cursor( const render::size& offset );

		void add_to_cursor( int offset );

		void create_scrollbar( );

		// the function used to add new children
		template< typename T, typename... VA >
		T* add( VA&&... args );

	public:

		const auto& scrollbar( ) const {
			return m_scrollbar;
		}

		auto& scrollbar( ) {
			return m_scrollbar;
		}

		const auto& children( ) const {
			return m_children_mgr;
		}

		auto& children( ) {
			return m_children_mgr;
		}

		const auto& cursor( ) const {
			return m_cursor;
		}

		void set_cursor( const render::point& val ) {
			m_cursor = val;
		}
	};
} // namespace gui::objects

#include "base_parent.inl"