#pragma once

namespace gui::managers {
	struct children {
	private:

		// the list of children
		std::vector< objects::base_ptr > m_list;

	public:

		objects::base_ptr& operator[]( u32 idx ) {
			return m_list[ idx ];
		}

		const objects::base_ptr& operator[]( u32 idx ) const {
			return m_list[ idx ];
		}

		u32 size( );

		void render( );

		bool think( );

		void reposition( const render::point& delta );

		void resize( const render::point& delta );

		objects::base_ptr& add( const objects::base_ptr& child );
	};
} // namespace gui::managers