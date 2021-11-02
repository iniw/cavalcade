#ifndef MOVE_HELPER_HPP
#define MOVE_HELPER_HPP

namespace sdk {
	struct base_entity;

	struct move_helper {
		VFUNC( void, set_host, 1, ( base_entity * entity ), entity );

		VFUNC( void, process_impacts, 4, ( ) );
	};
} // namespace sdk

#endif // MOVE_HELPER_HPP