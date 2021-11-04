#pragma once

template< typename T, typename... VA >
std::shared_ptr< T > gui::objects::base_parent::add( VA&&... args ) {
	base_ptr child = std::make_shared< T >( std::forward< VA >( args )... );

	// set the parent and identify
	child->m_parent = get( );
	child->identify( );

	// initialize it
	child->init( );

	// call our custom event
	on_add_child( child );

	// return by the proper type
	return std::dynamic_pointer_cast< T >( m_children.emplace_back( child ) );
}

#pragma once