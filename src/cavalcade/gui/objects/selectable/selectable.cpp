#include "selectable.hpp"
#include "detail.hpp"

void gui::objects::selectable::init( ) {
	base_child::init( );

	button::init( );

	// make smaller
	m_static_area.w = m_dynamic_area.w =
		std::max( { detail::sizing::width, m_label_size.w + style::padding::margin * 2, detail::sizing::min_popup_width } );
	m_label_pos = utils::center_label( m_label, m_static_area );

	m_popup_area.set_pos( { m_static_area.x, m_static_area.y2( ) + detail::padding::popup_gap } );
	m_popup_area.set_size( { m_static_area.w, std::min( detail::sizing::max_popup_height( ), m_height ) } );

	m_dynamic_area = m_popup_area;

	m_cursor = m_popup_area.pos( );

	for ( auto& opt : m_options )
		base_parent::add< helpers::list_option >( opt );

	// we don't have to push_cursor here because we already do that on button::init
}

void gui::objects::selectable::render( ) const {
	g_render.rectangle_filled( m_static_area, style::palette::background ).stroke( style::palette::stroke );

	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );

	if ( m_open_alpha > std::numeric_limits< float >::epsilon( ) ) {
		g_render.push_alpha( m_open_alpha );
		render_children( );
		g_render.pop_alpha( );
	}
}

bool gui::objects::selectable::think( ) {
	popup::think( );

	if ( !m_open )
		return false;

	listbox::think( );

	return true;
}

void gui::objects::selectable::animate( ) {
	popup::animate( );
	m_children_mgr.animate( );
}

void gui::objects::selectable::update_flags( ) {
	base_object::update_flags( );
	// override hovered
	if ( m_open )
		m_flags.set( flags::HOVERED, m_popup_area.hovered( ) );
}

void gui::objects::selectable::render_debug_bounds( ) const {
	base_object::render_debug_bounds( );
	if ( m_open )
		g_render.m_safe.draw_shape_front< render::geometry::rect >( m_popup_area, style::palette::debug_bounds_other );
}

void gui::objects::selectable::reposition( const render::point& delta ) {
	button::reposition( delta );
	popup::reposition( delta );
	m_children_mgr.reposition( delta );
}

void gui::objects::selectable::resize( const render::size& delta ) {
	button::resize( delta );
	popup::resize( delta );
	m_children_mgr.resize( delta );
}

void gui::objects::selectable::render_children( ) const {
	// background
	// render::overlay::rectangle_filled( m_popup_area, style::palette::background );

	// render::overlay::push_clip_rect( m_popup_area );

	// // children
	// for ( auto& child : m_children_mgr ) {
	// 	// scrollbar should overlay the children, so skip it for now
	// 	if ( child->is< objects::scrollbar >( ) )
	// 		continue;

	// 	auto color = options_info::get_active( this ) == child.get( ) ? style::palette::highlight : style::palette::text;
	// 	auto& font = options_info::get_active( this ) == child.get( ) ? render::menu_font_bold : render::menu_font;

	// 	render::overlay::string( font, child->label( ), child->label_pos( ), color );
	// }

	// // scrollbar
	// if ( m_scrollbar )
	// 	render::overlay::rectangle_filled( m_scrollbar->dyn_area( ), style::palette::highlight ); // Poo

	// // outline
	// render::overlay::rectangle( m_popup_area, style::palette::stroke );

	// render::overlay::pop_clip_rect(  );
}