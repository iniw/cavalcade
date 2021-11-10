#pragma once

namespace gui::objects {
	template< typename T >
	struct base_child : public base_object {
	protected:

		// our variable
		T m_var;

	public:

		// zero-initialize our variable
		base_child( ) : m_var{ } { }

		virtual ~base_child( ) = default;
	};
}; // namespace gui::objects