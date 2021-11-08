#pragma once

namespace gui::objects {
	struct base_parent : public base_object, std::enable_shared_from_this< base_parent > {
	protected:

		// the position used to place new children
		render::point m_cursor;

		// our children manager
		managers::children m_children;

	public:

		virtual ~base_parent( ) = default;

		// returns a shared_ptr instance of ourselves
		template< typename T = base_parent >
		std::shared_ptr< T > get( );

		// the function used to add new children
		template< typename T, typename... VA >
		std::shared_ptr< T > add( VA&&... args );

		// returns m_cursor
		const render::point& get_cursor( );

		// offsets m_cursor[ Y ] by the offset + the style's object padding
		void push_cursor( i32 offset );

		// sets m_cursor directly
		void set_cursor( const render::point& val );

	protected:

		// NOTE(wini): this a bit of hack/workaround about the fact that you can't virtualize templated functions
		// it might be of interest to most objects that inherent from base_parent to automatically
		// initialize their scrollbar as soon as a child's area gets past it's own, BUT, that might not apply to
		// every object, e.g: tabs. so until i figure out something smarter this is going to stay here
		virtual void on_add_child( base_ptr child );
	};
} // namespace gui::objects

#include "base_parent.inl"