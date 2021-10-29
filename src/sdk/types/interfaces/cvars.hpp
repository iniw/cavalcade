#ifndef CVARS_HPP
#define CVARS_HPP

namespace sdk::interfaces {
	struct cvars {
		enum idx
		{ FIND_VAR = 16 };

		VFUNC( cvar*, find_var, idx::FIND_VAR, ( std::string_view name ), name.data( ) );
	};
} // namespace sdk::interfaces

#endif // CVARS_HPP
