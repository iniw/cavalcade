#ifndef BASE_CHILD_HPP
#define BASE_CHILD_HPP

namespace gui::objects {
	template< typename T >
	struct base_child : public base_object {
		// used by the config system to access the type of our variable
		using var_type = T;

		// our variable
		var_type m_var;

		// zero-initialize our variable
		base_child( ) : m_var{ } { }

		virtual ~base_child( ) = default;
	};
}; // namespace gui::objects

#endif // BASE_CHILD_HPP