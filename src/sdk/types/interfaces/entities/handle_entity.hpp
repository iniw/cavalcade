#ifndef HANDLE_ENTITY_HPP
#define HANDLE_ENTITY_HPP

namespace sdk::interfaces {
	struct handle_entity {
		virtual ~handle_entity( ) { }

		virtual void set_handle( const handle& handle ) = 0;
		virtual const handle& get_handle( ) const       = 0;
	};
} // namespace sdk::interfaces

#endif // HANDLE_ENTITY_HPP