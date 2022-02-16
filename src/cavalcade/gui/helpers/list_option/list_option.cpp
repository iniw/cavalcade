#include "list_option.hpp"

void gui::helpers::list_option::init( ) {
	children_info::add_to_list( this );

	m_static_area.set_pos( m_parent->cursor( ) );
	m_static_area.set_size( { m_parent->dyn_area( ).w, height( ) } );

	m_dynamic_area = m_static_area;

	m_label_pos.x = m_static_area.x + style::padding::obj_spacing;
	m_label_pos.y = utils::center_label( m_label, m_static_area ).y;

	// we don't push_cursor to not have any padding between options
	m_parent->add_to_cursor( height( ) );
}

void gui::helpers::list_option::render( ) const {
	auto col = children_info::is_active( this ) ? style::palette::highlight : style::palette::text;
	g_render.text< render::font::MENU >( m_label_pos, m_label, col );
}

bool gui::helpers::list_option::think( ) {
	if ( m_flags.test( objects::flags::HOVERED ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON ) )
		children_info::set_active( this );

	// there's no point in us ever stealing focus
	return false;
}