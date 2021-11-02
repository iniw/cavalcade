#ifndef BASE_PARENT_HPP
#define BASE_PARENT_HPP

#include "base_object.hpp"

namespace gui::objects {
	struct scrollbar;

	struct base_parent : public base_object, std::enable_shared_from_this< base_parent > {
	protected:

		// the position used to place new children
		render::point m_cursor;

		// a list of our children
		std::vector< base_ptr > m_children;

		// our current scrollbar object, if it exists
		std::shared_ptr< scrollbar > m_scrollbar;

	public:

		virtual ~base_parent( ) = default;

		// the function used to add new children
		template< typename T, typename... VA >
		std::shared_ptr< T > add( VA&&... args );

		// returns m_cursor
		const render::point& get_cursor( );

		// offsets m_cursor[ Y ] by the offset + the style's object padding
		void push_cursor( i32 offset );

		// returns a shared_ptr instance of ourselves
		parent_ptr get( );

	protected:

		// NOTE(wini): this a bit of hack/workaround about the fact that you can't virtualize templated functions
		// it might be of interest to most objects that inherent from base_parent to automatically
		// initialize their scrollbar as soon as a child's area gets past it's own, BUT, that might not apply to
		// every object, e.g: tabs. so until i figure out something smarter this is going to stay here
		virtual void on_add_child( base_ptr child );

		// just some wrappers for easier maintainability
		// TOOD(wini): focus system by sorting the children

		// renders all of our children, if we have any
		void render_children( );

		// calls think() for all of our children, if we have any
		// returns true as soon as one of them returns true
		bool think_children( );
	};
} // namespace gui::objects

#include "base_parent.inl"

#endif // BASE_PARENT_HPP