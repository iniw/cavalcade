#include "tab.hpp"
#include "styling.hpp"

gui::objects::tab::tab( std::string_view name, std::string_view label ) {
	// basic identification
	m_name  = name;
	m_label = label;
}

// TODO(wini): figure out a way for tabs to work with dynamically sized groupboxes
void gui::objects::tab::init( ) {
	auto& info = s_info[ m_parent->m_id ];

	// add us to the list
	info.m_list.emplace_back( reinterpret_cast< tab* >( m_parent ) );

	// our areas will mimic those of our parent's
	m_static_area  = m_parent->m_dynamic_area;
	m_dynamic_area = m_static_area; // this is temporary

	// the height is the same for all tabs
	m_button_area[ HEIGHT ] = personal::sizing::button_height;

	// the width available for all tabs to share
	i32 available_width = m_dynamic_area[ WIDTH ];

	// update the button area for all tabs in this parent
	for ( i32 i = 0; i < info.m_list.size( ); i++ ) {
		auto& tab         = info.m_list[ i ];
		auto& button_area = tab->m_button_area;

		button_area.pos( m_dynamic_area.pos( ) );
		button_area[ WIDTH ] = available_width / info.m_list.size( );
		button_area[ X ] += button_area[ WIDTH ] * i;

		// also update the label position
		auto label_size = g_render.text_size< render::font::MENU >( tab->m_label );

		tab->m_label_pos[ X ] = ( button_area[ X ] + button_area[ WIDTH ] / 2 ) - label_size[ X ] / 2;
		tab->m_label_pos[ Y ] = button_area[ Y ] + label_size[ Y ] / 2;
	}

	// fix our dynamic area (after updating everyone)
	// TODO(wini): make a function that wraps this operation of adding to an axis and subtracting from the size's equivalent
	m_dynamic_area[ Y ] += personal::sizing::button_height + general::padding::obj_margin;
	m_dynamic_area[ HEIGHT ] -= personal::sizing::button_height + general::padding::obj_margin;

	m_cursor = m_dynamic_area.pos( );
}

void gui::objects::tab::render( ) const {
	bool active      = is_active( );
	auto outline_col = active ? general::pallete::highlight : general::pallete::secondary;

	g_render.rectangle( m_button_area, general::pallete::primary ).outline( outline_col );

	g_render.text< render::font::MENU >( m_label_pos, m_label, general::pallete::text );

	if ( active )
		// NOTE(wini): the expand is a bit of a hack because our dynamic and static area are the same
		// so we need to accout for that
		return m_children.render( m_static_area.expand( 1 ), this );
}

bool gui::objects::tab::think( ) {
	// override our HOVERED flag
	m_flags.set( flags::HOVERED, g_io.mouse_pos( ).in_rect( m_button_area ) );

	bool active = m_flags.test( flags::HOVERED ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON );

	if ( active )
		set_active( );

	if ( is_active( ) )
		if ( m_children.think( this ) )
			active = true;

	return active;
}

void gui::objects::tab::reposition( const render::point& delta ) {
	m_button_area.pos( m_button_area.pos( ) + delta );
	base_parent::reposition( delta );
}

void gui::objects::tab::resize( const render::point& delta ) {
	base_parent::resize( delta );

	// add the new width to the button area too
	m_button_area[ WIDTH ] += delta[ X ];
}