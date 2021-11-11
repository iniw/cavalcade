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

		const auto begin( ) const {
			return m_list.begin( );
		}

		const auto end( ) const {
			return m_list.end( );
		}

		u32 size( );

		void render( const render::rect& area, const objects::parent_raw_ptr parent ) const;

		bool think( const objects::parent_raw_ptr parent );

		void reposition( const render::point& delta );

		void resize( const render::point& delta );

		objects::base_ptr& add( const objects::base_ptr& child );
	};
} // namespace gui::managers