#ifndef ENTITY_LIST_HPP
#define ENTITY_LIST_HPP

namespace sdk::interfaces {
	struct entity_list {
	private:

		enum idx
		{
			GET_CLIENT_ENTITY             = 3,
			GET_CLIENT_ENTITY_FROM_HANDLE = 4
		};

		VFUNC( client_entity*, get_client_entity, idx::GET_CLIENT_ENTITY, ( i32 idx ), idx );

		VFUNC( client_entity*, get_client_entity_from_handle, idx::GET_CLIENT_ENTITY_FROM_HANDLE, ( const handle& handle ),
		       handle );

	public:

		template< typename T = base_entity* >
		T get( i32 idx ) {
			return static_cast< T >( get_client_entity( idx ) );
		}

		template< typename T = base_entity* >
		T get_handle( const handle& handle ) {
			return static_cast< T >( get_client_entity_from_handle( handle ) );
		}
	};
} // namespace sdk::interfaces

#endif // ENTITY_LIST_HPP