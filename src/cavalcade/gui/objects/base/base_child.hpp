#pragma once

namespace gui::objects {
	template< typename T >
	struct base_child : public base_object {
	protected:

		// our variable, controlled by the cfg system
		T* m_var;

	public:

		base_child( ) = default;

		virtual ~base_child( ) = default;
	};
}; // namespace gui::objects