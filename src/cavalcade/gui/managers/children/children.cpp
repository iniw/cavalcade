#include "../../objects/base/base.hpp"

void gui::managers::children::render( ) {
	// sort our children by time in reverse, guarantees that the most recently interacted-with element renders last
	std::ranges::sort( m_list, []( objects::base_ptr& a, objects::base_ptr& b ) { return a->m_time < b->m_time; } );

	for ( auto& child : m_list )
		child->render( );
}

bool gui::managers::children::think( ) {
	// sort our children by time, guarantees that the most recently interacted-with element thinks first
	std::ranges::sort( m_list, []( objects::base_ptr& a, objects::base_ptr& b ) { return a->m_time > b->m_time; } );

	for ( auto& child : m_list ) {
		// first update the flags
		// NOTE(wini): might be of interest to make a virtual to handle flags
		// since not all objects behave the same, e.g: tab's HOVERED doesn't used static_area

		// don't think if the child is disabled
		if ( child->m_flags.test( objects::flags::DISABLED ) )
			continue;

		child->m_flags.reset( );

		child->m_flags.set( objects::flags::HOVERED, g_io.mouse_pos( ).in_rect( child->m_static_area ) );

		// then call think()
		child->m_flags.set( objects::flags::ACTIVE, child->think( ) );

		// if the child had an interaction: update it's time and stop iterating
		if ( child->m_flags.test( objects::flags::ACTIVE ) )
			return child->m_time = LI_FN( GetTickCount64 )( );
	}

	return false;
}