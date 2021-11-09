#pragma once

namespace gui::managers {
	struct children {
	private:

		// the list of children
		std::vector< objects::base_ptr > m_list;

	public:

		void render( );

		bool think( );

		objects::base_ptr& add( const objects::base_ptr& child );
	};
} // namespace gui::managers