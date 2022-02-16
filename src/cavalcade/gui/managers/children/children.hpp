#pragma once

namespace gui::managers {

	enum find_filter
	{
		SCROLLBARS = 1 << 0,
		COLUMNS    = 1 << 1,
	};

	struct children {
	private:

		// the list of children
		// std::list is used to here to avoid invalidation of iterators when adding new elements
		std::list< objects::base_ptr > m_list;

		// the object we belong to
		objects::base_parent* m_parent;

	public:

		children( ) : m_list{ }, m_parent{ nullptr } { }

		void render( const render::rect& area, bool intersect_clip_rect = true ) const;

		bool think( );

		void animate( );

		void reposition( const render::point& delta );

		void resize( const render::size& delta );

		// these two functions are necessary because the
		// "first" and "last" objects on m_list aren't necessarily
		// the oldest/latest, since the list is constantly being sorted on think()
		objects::base_object* find_oldest_child( int filter = find_filter::COLUMNS | find_filter::SCROLLBARS );

		objects::base_object* find_latest_child( int filter = find_filter::COLUMNS | find_filter::SCROLLBARS );

	public:

		void add( objects::base_ptr child ) {
			m_list.push_back( std::move( child ) );
		}

		void set_parent( objects::base_parent* p ) {
			m_parent = p;
		}

		const auto& first( ) const {
			return m_list.front( );
		}

		const auto& last( ) const {
			return m_list.back( );
		}

		const auto begin( ) const {
			return m_list.begin( );
		}

		const auto end( ) const {
			return m_list.end( );
		}

		uint32_t size( ) const {
			return m_list.size( );
		}

		auto& list( ) {
			return m_list;
		}

		const auto& list( ) const {
			return m_list;
		}
	};
} // namespace gui::managers