#ifndef BASE_PARENT_HPP
#define BASE_PARENT_HPP

#include "base_object.hpp"

namespace gui::objects {
	struct scrollbar;

	struct base_parent : base_object, std::enable_shared_from_this< base_parent > {
		// a list of our children
		std::vector< base_ptr > m_children;

		// the position used to place new children
		render::point m_cursor;

		// our current scrollbar object, if it exists;
		std::shared_ptr< scrollbar > m_scrollbar;

	public:

		parent_ptr get( ) {
			return shared_from_this( );
		}

		// the function used to add new children
		template< object T, typename... VA >
		std::shared_ptr< T > add( VA&&... args ) {
			std::shared_ptr< T > child = std::make_shared< T >( std::forward< VA >( args )... );

			child->init( get( ) );

			m_children.push_back( child );

			on_add_child( );

			return child;
		}

	protected:

		// NOTE(wini): this a bit of hack/workaround about the fact that you can't virtualize templated functions;
		// it might be of interest to most objects that inherent from base_parent to automatically;
		// initialize their scrollbar as soon as a child's area gets past it's own, BUT, that might not apply to;
		// every object, e.g: tabs. so until i figure out something smarter this is going to stay here
		virtual void on_add_child( ) {
			// TODO(wini): initialize our scrollbar if the child's area extends beyond ours
		}

		// just some wrappers for easier maintainability
		// TOOD(wini): focus system by sorting the children

		void render_children( ) {
			for ( auto& child : m_children )
				child->render( );
		}

		bool think_children( ) {
			for ( auto& child : m_children )
				if ( child->think( ) )
					return child->m_time = m_time = GetTickCount64( );

			return false;
		}
	};
} // namespace gui::objects

#endif // BASE_PARENT_HPP