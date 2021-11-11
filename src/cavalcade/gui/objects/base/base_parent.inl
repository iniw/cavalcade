#pragma once

template< typename T, typename... VA >
std::shared_ptr< T > gui::objects::base_parent::add( VA&&... args ) {
	std::shared_ptr< T > child = std::make_shared< T >( std::forward< VA >( args )... );

	// identify our new child
	child->identify( this );

	// initialize it
	child->init( );

	// call our custom event
	on_add_child( child );

	// add it
	m_children.add( child );

	// return the proper type
	return child;
}