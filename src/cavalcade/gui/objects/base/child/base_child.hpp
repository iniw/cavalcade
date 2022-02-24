#pragma once

namespace gui::objects {
	template< typename T >
	struct base_child : virtual base_object {
	protected:

		using var_type = T;

		// our variable, controlled by the cfg system
		var_type* m_var;

	public:

		base_child( ) : base_object( ), m_var{ nullptr } { }

		virtual ~base_child( ) = default;

		virtual void init( ) override {
			m_var = cfg::add< var_type >( m_id >> 32 );
		}

		void set_var( var_type val ) {
			*m_var = val;
		}
	};
}; // namespace gui::objects