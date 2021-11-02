#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

namespace gui::objects {
	struct base_object {
		// our parent, if it exists
		parent_ptr m_parent;

		// our type
		type m_type;

		// the total amount of space we take up
		render::rect m_static_rect;

		// the amount of space dedicated to interaction or rendering
		render::rect m_dynamic_rect;

		// the last tickcount we were interacted with
		u64 m_time;

		// our label
		std::string m_label;

		// our absolute index;
		// the total amount of existing objects + 1
		u32 m_abs_idx;

		// our id;
		// first 32 bits: hash of our label;
		// last 32 bits: our absolute index;
		u64 m_id;

	public:

		constexpr base_object( ) : m_time{ 0 }, m_id{ 0 }, m_abs_idx{ 0 } { }

		// our initialization routine, required by every object
		virtual void init( parent_ptr parent ) = 0;

		// our rendering routine, required by every object
		virtual void render( ) = 0;

		// our input-interpretation routine, required by every object
		// returns true if there was a successfull interaction
		virtual bool think( ) = 0;
	};
} // namespace gui::objects

#endif // BASE_OBJECT_HPP