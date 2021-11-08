#pragma once

namespace gui::managers {
	struct children {
	private:

		// the list of children
		std::vector< objects::base_ptr > m_list;

	public:

		void render( );

		bool think( );
	};
} // namespace gui::managers