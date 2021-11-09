#include "../../objects/base/base.hpp"

void gui::managers::children::render( ) {
	// iterate our children in reverse, guarantees that the most recently interacted-with object renders last
	for ( auto& child : m_list | std::views::reverse )
		child->render( );
}

bool gui::managers::children::think( ) {
	bool active = false;

	for ( auto& child : m_list ) {
		// NOTE(wini): might be of interest to make a virtual to handle flags
		// since not all objects behave the same, e.g: tab's HOVERED doesn't use static_area

		// don't think if the child is disabled
		if ( child->m_flags.test( objects::flags::DISABLED ) )
			continue;

		child->m_flags.reset( );

		child->m_flags.set( objects::flags::HOVERED, g_io.mouse_pos( ).in_rect( child->m_static_area ) );

		child->m_flags.set( objects::flags::ACTIVE, child->think( ) );

		// if the child had an interaction: update it's time and stop iterating
		if ( child->m_flags.test( objects::flags::ACTIVE ) ) {
			active = child->m_time = LI_FN( GetTickCount64 )( );
			break;
		}
	}

	// if there was activity sort our children by time, guarantees that the most recently interacted-with object thinks first
	if ( active )
		std::ranges::sort( m_list, []( objects::base_ptr& a, objects::base_ptr& b ) { return a->m_time > b->m_time; } );

	return active;
}

gui::objects::base_ptr& gui::managers::children::add( const objects::base_ptr& child ) {
	return m_list.emplace_back( child );
}