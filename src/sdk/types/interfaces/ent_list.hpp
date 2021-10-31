#ifndef ENT_LIST_HPP
#define ENT_LIST_HPP

namespace sdk::interfaces {
	struct ent_list {
	private:

		enum idx
		{
			GET_ENTITY             = 3,
			GET_ENTITY_FROM_HANDLE = 4
		};

		VFUNC( client_entity*, get_entity, idx::GET_ENTITY, ( i32 idx ), idx );

		VFUNC( client_entity*, get_entity_from_handle, idx::GET_ENTITY_FROM_HANDLE, ( const handle& handle ), handle );

	public:

		template< typename T = base_entity* >
		T get( i32 idx ) {
			return static_cast< T >( get_entity( idx ) );
		}

		template< typename T = base_entity* >
		T get_handle( const handle& handle ) {
			return static_cast< T >( get_entity_from_handle( handle ) );
		}
	};
} // namespace sdk::interfaces

#endif // ENT_LIST_HPP