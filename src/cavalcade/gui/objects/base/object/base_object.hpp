#pragma once

namespace gui::objects {
	struct base_object {
	protected:

		// our parent, if it exists
		base_parent* m_parent;

		// the total amount of space we take up
		render::rect m_static_area;

		// the space dedicated to interaction or rendering
		render::rect m_dynamic_area;

		// the last tickcount we were interacted with
		uint64_t m_time;

		// refer to the declaration of flags_set on base.h
		flags_set m_flags;

		// our identifier for the cfg system
		std::string m_cfg_entry;

		// our label, used to identify us and for rendering
		std::string m_label;

		// the position of our label
		render::point m_label_pos;

		// the size of our label
		render::size m_label_size;

		// our index in our parent's children list
		uint32_t m_idx;

		// the total amount of existing objects at the time of creation
		uint32_t m_abs_idx;

		// first 32 bits: hash of our label
		// last 32 bits: our absolute index
		uint64_t m_id;

		// our callback, if it exists
		// gets called by the children manager when we are active
		// refer to the declaration of callback_fn on base.h
		callback_fn m_callback;

		// the amount of time (in ms) we have been hovered
		int m_hovered_time;

		// our tooltip, used in conjunction with m_hovered_time
		std::string m_tooltip;

		// the total amount of objects
		static inline uint32_t s_object_count = 0;

	public:

		base_object( ) : m_parent{ nullptr }, m_time{ 0 }, m_idx{ 0 }, m_abs_idx{ 0 }, m_id{ 0 }, m_hovered_time{ 0 } { }

		base_object( std::string_view label ) : base_object( ) {
			m_label      = label;
			m_label_size = g_render.text_size< render::font::MENU >( m_label );
		}

		virtual ~base_object( ) = default;

		// our initialization routine, required by every object
		virtual void init( ) = 0;

		// our rendering routine, required by every object
		virtual void render( ) const = 0;

		// our input-interpretation routine, required by every object
		// returns true if there was a succesfull interaction
		virtual bool think( ) = 0;

		virtual void animate( ) { }

		// handle our flags behavior
		virtual void update_flags( );

		// repositions ourselves, adds "delta" to our positions
		virtual void reposition( const render::point& delta );

		// resizes ourselves, adds "delta" to our sizes
		virtual void resize( const render::size& delta );

		// draws rectangles on our bounds
		virtual void render_debug_bounds( ) const;

		// identifies and validates us as a menu object
		void identify( base_parent* parent );

		template< Object T >
		T* as( ) {
			return dynamic_cast< T* >( this );
		}

		template< Object T >
		const T* as( ) const {
			return dynamic_cast< const T* >( this );
		}

		// what a great trick! god bless RTTI.
		template< Object T >
		bool is( ) const {
			return as< T >( );
		}

	public:

		const auto& parent( ) const {
			return m_parent;
		}

		const auto& stt_area( ) const {
			return m_static_area;
		}

		auto& stt_area( ) {
			return m_static_area;
		}

		const auto& dyn_area( ) const {
			return m_dynamic_area;
		}

		auto& dyn_area( ) {
			return m_dynamic_area;
		}

		const auto& time( ) const {
			return m_time;
		}

		void set_time( uint64_t time ) {
			m_time = time;
		}

		const auto& flags( ) const {
			return m_flags;
		}

		auto& flags( ) {
			return m_flags;
		}

		bool disabled( ) const {
			return m_flags.test( flags::DISABLED );
		}

		void enable( ) {
			m_flags.set( flags::DISABLED, false );
		}

		void disable( ) {
			m_flags.set( flags::DISABLED, true );
		}

		const auto& cfg_entry( ) const {
			return m_cfg_entry;
		}

		std::string_view label( ) const {
			return m_label;
		}

		const auto& label_pos( ) const {
			return m_label_pos;
		}

		const auto& idx( ) const {
			return m_idx;
		}

		const auto& abs_idx( ) const {
			return m_abs_idx;
		}

		const auto& id( ) const {
			return m_id;
		}

		const auto& callback( ) const {
			return m_callback;
		}

		void set_callback( const callback_fn& fn ) {
			m_callback = fn;
		}

		const auto& hover_time( ) const {
			return m_hovered_time;
		}

		void set_hover_time( int time ) {
			m_hovered_time = time;
		}

		std::string_view tooltip( ) const {
			return m_tooltip;
		}

		void set_tooltip( std::string_view str ) {
			m_tooltip = str;
		}

		static auto object_count( ) {
			return s_object_count;
		}
	};
} // namespace gui::objects