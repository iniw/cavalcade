#pragma once

namespace gui::objects {
	struct base_object {
		// our parent, if it exists
		parent_ptr m_parent;

		// our type
		type m_type;

		// the total amount of space we take up
		render::rect m_static_area;

		// the space dedicated to interaction or rendering
		render::rect m_dynamic_area;

		// the last tickcount we were interacted with
		u64 m_time;

		// our flags, refer to the declaration of flags on base.hpp
		flags_set m_flags;

		// our label, used externally by the render() routine
		std::string m_label;

		// the position of our label
		render::point m_label_pos;

		// our name, used internally by the identifier and config system
		std::string m_name;

		// our absolute index
		// the total amount of existing objects + 1 (at the time of creation)
		u32 m_abs_idx;

		// our id;
		// first 32 bits: hash of our name
		// last 32 bits: our absolute index
		u64 m_id;

		// the total amount of objects
		static inline u32 s_object_count = 0;

	public:

		base_object( ) : m_time{ 0 }, m_id{ 0 }, m_abs_idx{ 0 } { }

		virtual ~base_object( ) = default;

		// our initialization routine, required by every object
		virtual void init( ) = 0;

		// our rendering routine, required by every object
		virtual void render( ) = 0;

		// our input-interpretation routine, required by every object
		// returns true if there was a successfull interaction
		virtual bool think( ) = 0;

		// identifies and validates us as a valid object
		virtual void identify( parent_ptr parent );
	};
} // namespace gui::objects