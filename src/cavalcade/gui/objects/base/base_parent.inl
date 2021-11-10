#pragma once

template< typename T >
std::shared_ptr< T > gui::objects::base_parent::get( ) {
	if constexpr ( std::is_same_v< T, base_parent > )
		return shared_from_this( );
	else
		return std::dynamic_pointer_cast< T >( shared_from_this( ) );
}

template< typename T, typename... VA >
std::shared_ptr< T > gui::objects::base_parent::add( VA&&... args ) {
	std::shared_ptr< T > child = std::make_shared< T >( std::forward< VA >( args )... );

	// identify our new child
	child->identify( get( ) );

	// initialize it
	child->init( );

	// call our custom event
	on_add_child( child );

	// add it
	m_children.add( child );

	// return the proper type
	return child;
}