#pragma once

#include "../base/base.hpp"

namespace gui::objects {
	struct tab : public base_parent, public base_traits< tab > {
	protected:

		// the area of our clickable button
		render::rect m_button_area;

		// helper struct that stores information about the state of all other tabs in a parent
		struct info {
			// the list of all tabs
			std::vector< ptr > m_list;
			// the id of the currently active tab
			u64 m_active;
		};

		// accessor for our helper struct
		static inline std::unordered_map< u64, info > s_info{ };

		// return true if we are the active tab
		bool is_active( ) const {
			return s_info[ m_parent->m_id ].m_active == m_id;
		}

	public:

		tab( std::string_view name, std::string_view label );

		void init( ) override;

		void render( ) const override;

		bool think( ) override;

		void reposition( const render::point& delta ) override;

		void resize( const render::point& delta ) override;

		// sets us as the active tab
		void set_active( ) {
			s_info[ m_parent->m_id ].m_active = m_id;
		}
	};
} // namespace gui::objects