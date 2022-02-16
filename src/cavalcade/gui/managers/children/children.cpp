#include "../../objects/objects.hpp"

void gui::managers::children::render( const render::rect& area, bool intersect_clip_rect ) const {
	if ( m_list.empty( ) )
		return;

	g_render.push_clip_rect( area, intersect_clip_rect );

	// iterate our children in reverse
	// guarantees that the most recently interacted-with object renders last
	for ( auto it = m_list.rbegin( ); it != m_list.rend( ); ++it ) {
		auto& child = *it;

		if ( child->disabled( ) )
			continue;

		// don't render the scrollbar now
		if ( !child->is< objects::scrollbar >( ) )
			child->render( );

		if ( child->flags( ).test( objects::flags::DEBUG_BOUNDS ) || style::debug::render_bounds )
			child->render_debug_bounds( );
	}

	g_render.pop_clip_rect( );

	// we have to render the scrollbar here for two reasons:
	// give a false sense of overlay, since it'll render above all other children
	// but mostly importantly, to respect our grandparent's clip_rect
	auto scrollbar = m_parent->scrollbar( );
	if ( scrollbar && !scrollbar->disabled( ) )
		scrollbar->render( );
}

bool gui::managers::children::think( ) {
	if ( m_list.empty( ) )
		return false;

	bool activity = false;

	for ( auto& child : m_list ) {
		// we need to check for vertically out of bounds elements when we have a scrollbar
		if ( m_parent->scrollbar( ) )
			// don't do oob check if the chld is a master_checkbox
			if ( !child->is< objects::master_checkbox >( ) &&
			     ( child->stt_area( ).y2( ) <= m_parent->dyn_area( ).y || child->stt_area( ).y >= m_parent->dyn_area( ).y2( ) ) )
				continue;

		if ( child->disabled( ) )
			continue;

		child->update_flags( );

		activity = child->think( );

		if ( activity ) {
			child->flags( ).set( objects::flags::ACTIVE );

			child->set_time( GetTickCount64( ) );

			auto& callback = child->callback( );
			if ( callback )
				callback( child );

			break;
		}
	}

	// if there was activity sort our children by time
	// guarantees that the most recently interacted-with object thinks first
	if ( activity )
		m_list.sort( []( const auto& a, const auto& b ) { return a->time( ) > b->time( ); } );

	return activity;
}

void gui::managers::children::animate( ) {
	for ( auto& child : m_list )
		child->animate( );
}

void gui::managers::children::reposition( const render::point& delta ) {
	for ( auto& child : m_list )
		child->reposition( delta );
}

void gui::managers::children::resize( const render::size& delta ) {
	for ( auto& child : m_list )
		child->resize( delta );
}

gui::objects::base_object* gui::managers::children::find_oldest_child( int filter ) {
	if ( m_list.empty( ) )
		return nullptr;

	objects::base_object* ret = nullptr;

	for ( const auto& obj : m_list ) {
		if ( filter & find_filter::COLUMNS )
			if ( obj->is< objects::column >( ) )
				continue;

		if ( filter & find_filter::SCROLLBARS )
			if ( obj->is< objects::scrollbar >( ) )
				continue;

		if ( obj->idx( ) == 0 ) {
			ret = obj.get( );
			break;
		}
	}

	return ret;
}

gui::objects::base_object* gui::managers::children::find_latest_child( int filter ) {
	if ( m_list.empty( ) )
		return nullptr;

	objects::base_object* ret = nullptr;
	uint32_t idx              = 0;

	for ( const auto& obj : m_list ) {
		if ( filter & find_filter::COLUMNS )
			if ( obj->is< objects::column >( ) )
				continue;

		if ( filter & find_filter::SCROLLBARS )
			if ( obj->is< objects::scrollbar >( ) )
				continue;

		if ( obj->idx( ) > idx ) {
			ret = obj.get( );
			idx = obj->idx( );
		}
	}

	return ret;
}