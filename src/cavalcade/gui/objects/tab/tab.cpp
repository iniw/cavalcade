#include "tab.hpp"

// TODO(wini): figure out a way for tabs to work with dynamically sized groupboxes
void gui::objects::tab::init( ) {
	children_info::add_to_list( this );

	// our areas will mimic our parent's
	m_static_area = m_parent->dyn_area( );

	m_dynamic_area = m_static_area;
	m_dynamic_area.vertical_cut( style::sizing::button.h + style::padding::margin );

	m_cursor = m_dynamic_area.pos( );

	center_tabs( );
}

void gui::objects::tab::render( ) const {
	if ( children_info::is_active( this ) ) {
		g_render.line( { m_button_area.x, m_button_area.y2( ) }, m_button_area.max( ), style::palette::highlight );
		m_children_mgr.render( m_dynamic_area );
	}

	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );
}

bool gui::objects::tab::think( ) {
	bool active = m_button_area.hovered( ) && g_io.key_state< io::key_state::RELEASED >( VK_LBUTTON );

	if ( active )
		children_info::set_active( this );

	if ( children_info::is_active( this ) )
		return m_children_mgr.think( );

	return active;
}

void gui::objects::tab::update_flags( ) {
	base_object::update_flags( );
	// override hovered
	m_flags.set( flags::HOVERED, m_flags.test( flags::HOVERED ) || m_button_area.hovered( ) );
}

void gui::objects::tab::on_add_child( base_object* child ) {
	bool gone_past = child->stt_area( ).y2( ) > m_dynamic_area.y2( );
	if ( !gone_past )
		return;

	if ( m_scrollbar ) {
		// enable it again if it already existed
		m_scrollbar->enable( );
		// now update
		m_scrollbar->update_height( );
	} else {
		m_children_mgr.resize( { -style::padding::margin, 0 } );
		// create it if it doesn't exist
		create_scrollbar( );
		m_static_area.w -= style::padding::margin;
		m_dynamic_area.w -= style::padding::margin;
	}
}

void gui::objects::tab::reposition( const render::point& delta ) {
	base_parent::reposition( delta );
	m_button_area.add_pos( delta );
}

void gui::objects::tab::resize( const render::size& delta ) {
	base_parent::resize( delta );
	center_tabs( );
}

void gui::objects::tab::render_debug_bounds( ) const {
	base_parent::render_debug_bounds( );
	g_render.m_safe.draw_shape_front< render::geometry::rect >( m_button_area, style::palette::debug_bounds_other );
}

void gui::objects::tab::center_tabs( ) {
	auto& info = children_info::get( this );

	// the width available for all tabs to share
	int available_width = m_static_area.w;
	// the width of all buttons
	int button_width = available_width / info.list.size( );

	// update the button area for all tabs in this parent
	for ( int i = 0; i < info.list.size( ); i++ ) {
		auto tab = info.list[ i ];

		auto& button_area = tab->m_button_area;
		button_area.set_pos( { m_static_area.x + button_width * i, m_static_area.y } );
		button_area.set_size( { button_width, style::sizing::button.h } );

		// also update the label position
		tab->m_label_pos = utils::center_label( tab->m_label, tab->m_button_area );
	}
}