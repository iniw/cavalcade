#include "colorpicker.hpp"
#include "detail.hpp"

void gui::objects::colorpicker::init( ) {
	base_child::init( );

	render::point pos;

	if ( m_attach && m_parent->children( ).size( ) ) {
		auto& last = m_parent->children( ).last( );
		pos        = { m_parent->dyn_area( ).x2( ) - detail::sizing::main.w,
                ( last->stt_area( ).y + last->stt_area( ).h / 2 ) - detail::sizing::main.h / 2 };
	} else
		pos = m_parent->cursor( );

	m_static_area.set_pos( pos );
	m_static_area.set_size( detail::sizing::main );

	m_dynamic_area = m_static_area;

	m_popup_area.set_pos( { m_static_area.x2( ) + style::padding::obj_spacing, m_static_area.y } );
	m_popup_area.set_size( detail::sizing::popup );

	m_sat_area.set_pos( m_popup_area.pos( ).offset( style::padding::margin ) );
	m_sat_area.set_size( detail::sizing::sat );

	m_hue_area.set_pos( { m_sat_area.x2( ) + style::padding::obj_spacing, m_sat_area.y } );
	m_hue_area.set_size( detail::sizing::hue );

	m_alpha_area.set_pos( { m_sat_area.x, m_sat_area.y2( ) + style::padding::obj_spacing } );
	m_alpha_area.set_size( detail::sizing::alpha );

	m_og_col_area.set_pos( { m_alpha_area.x2( ) + style::padding::obj_spacing, m_alpha_area.y } );
	m_og_col_area.set_size( detail::sizing::og_col );

	m_label_pos.y = ( m_static_area.y + m_static_area.h / 2 ) - m_label_size.h / 2;
	m_label_pos.x = m_static_area.x;

	if ( m_attach )
		m_label_pos.x -= m_label_size.w + style::padding::obj_spacing;
	else
		m_label_pos.x += detail::sizing::main.w + style::padding::obj_spacing;

	if ( !m_attach )
		m_parent->push_cursor( m_static_area.h );
}

void gui::objects::colorpicker::render( ) const {
	g_render.rectangle_filled( m_static_area, *m_var ).stroke( style::palette::stroke );
	g_render.text< render::font::MENU >( m_label_pos, m_label, style::palette::text );

	if ( m_open_alpha < 0.01f )
		return;

	g_render.push_alpha( m_open_alpha );

	// // background
	// render::overlay::rectangle_filled( m_popup_area, style::palette::background )->stroke( style::palette::stroke );

	// // saturation
	// {
	// 	// get color for the hue with max saturation and value
	// 	render::color picker_color = render::color::from_hsb( m_hue, 1, 1 );

	// 	// saturation rect
	// 	render::overlay::rectangle_filled_multi_grad( m_sat_area, render::color::white( ), picker_color, render::color::white( ), picker_color );
	// 	render::overlay::rectangle_filled_multi_grad( m_sat_area, render::color::black( 0 ), render::color::black( 0 ), render::color::black( ),
	// 	                                              render::color::black( ) );

	// 	// border
	// 	render::overlay::rectangle( m_sat_area, detail::palette::border );

	// 	// mouse pos from hsb
	// 	f32 x = m_visual_saturation;
	// 	f32 y = 1.f - m_visual_brightness;

	// 	// scale to rect
	// 	render::rect actual_sat_area = m_sat_area.shrink( 1 ); // account for border
	// 	render::size size            = actual_sat_area.size( );
	// 	render::point cursor_offset  = render::point( size.w * x, size.h * y );
	// 	render::point cursor_pos     = actual_sat_area.pos( ) + cursor_offset;

	// 	render::rect cursor_rect = { cursor_pos.x, cursor_pos.y, 0, 0 };

	// 	int cursor_size = 2;
	// 	if ( m_clicking == clicked_item::SATURATION )
	// 		cursor_size = 3;

	// 	// cursor
	// 	render::overlay::rectangle( render::rect( cursor_rect ).expand( cursor_size ), style::palette::text )
	// 		->dropshadow( detail::palette::cursor_shadow );
	// }

	// // hue bar
	// {
	// 	static constexpr std::array< render::color, 7 > hue_colors = { render::color( 255, 0, 0 ), render::color( 255, 255, 0 ),
	// 		                                                           render::color( 0, 255, 0 ), render::color( 0, 255, 255 ),
	// 		                                                           render::color( 0, 0, 255 ), render::color( 255, 0, 255 ),
	// 		                                                           render::color( 255, 0, 0 ) };

	// 	// fill bg with red in case the hues dont fill the area
	// 	render::overlay::rectangle_filled( m_hue_area, hue_colors[ 0 ] );

	// 	// hues
	// 	int hue_height = m_hue_area.h / ( hue_colors.size( ) - 1 );
	// 	for ( int i = 0; i < hue_colors.size( ) - 1; i++ ) {
	// 		render::overlay::rectangle_filled_grad( { m_hue_area.x, m_hue_area.y + i * hue_height, m_hue_area.w, hue_height }, hue_colors[ i ],
	// 		                                        hue_colors[ i + 1 ], GRADIENT_VERTICAL );
	// 	}

	// 	// border
	// 	render::overlay::rectangle( m_hue_area, detail::palette::border );

	// 	render::rect hue_indicator = { m_hue_area.x, m_hue_area.y + int( m_visual_hue * ( m_hue_area.h - detail::sizing::indicator_size ) ),
	// 		                           m_hue_area.w, detail::sizing::indicator_size };

	// 	// hue position indicator
	// 	render::overlay::rectangle( hue_indicator, style::palette::text )->dropshadow( detail::palette::cursor_shadow );
	// }

	// // alpha bar
	// {
	// 	render::overlay::rectangle_filled_grad( m_alpha_area, ( *m_var )( 0 ), ( *m_var )( 255 ), GRADIENT_HORIZONTAL )
	// 		->stroke( detail::palette::border );

	// 	render::rect alpha_indicator = { m_alpha_area.x + int( m_visual_alpha * ( m_alpha_area.w - detail::sizing::indicator_size ) ), m_alpha_area.y,
	// 		                             detail::sizing::indicator_size, m_alpha_area.h };

	// 	// alpha position indicator
	// 	render::overlay::rectangle( alpha_indicator, style::palette::text )->dropshadow( detail::palette::cursor_shadow );
	// }

	g_render.pop_alpha( );
}

bool gui::objects::colorpicker::think( ) {
	popup::think( );

	if ( m_open ) {
		if ( !m_init ) {
			// update color
			auto [ hue, sat, bright ] = m_var->to_hsb( );
			m_hue = m_visual_hue = hue;
			m_saturation = m_visual_saturation = sat;
			m_brightness = m_visual_brightness = bright;
			m_init                             = true;
		}

		if ( g_io.key_state( VK_LBUTTON ) ) {
			// color picking
			if ( m_clicking == clicked_item::SATURATION || ( m_clicking == clicked_item::NONE && m_sat_area.hovered( ) ) ) {
				// get new m_saturation and brightness
				m_saturation = m_sat_area.mouse_percent_x( );
				m_brightness = 1.f - m_sat_area.mouse_percent_y( );

				// set hsv
				*m_var = render::color::from_hsb( m_hue, m_saturation, m_brightness ).mod_alpha( m_var->m_a );

				m_clicking = clicked_item::SATURATION;
			}
			// hue changing
			else if ( m_clicking == clicked_item::HUE || ( m_clicking == clicked_item::NONE && m_hue_area.hovered( ) ) ) {
				// get the new hue
				m_hue = m_hue_area.mouse_percent_y( );

				// set hsv
				*m_var = render::color::from_hsb( m_hue, m_saturation, m_brightness ).mod_alpha( m_var->m_a );

				m_clicking = clicked_item::HUE;
			}
			// alpha changing
			else if ( m_clicking == clicked_item::ALPHA || ( m_clicking == clicked_item::NONE && m_alpha_area.hovered( ) ) ) {
				f32 alpha = m_alpha_area.mouse_percent_x( ) * 255.f;

				// set alpha
				m_var->m_a = alpha;

				m_clicking = clicked_item::ALPHA;
			}
		} else {
			m_clicking = clicked_item::NONE;
		}
	} else {
		m_init = false;
	}

	return m_open;
}

void gui::objects::colorpicker::animate( ) {
	popup::animate( );
	utils::animate( m_visual_hue, m_hue, detail::anim::cursor_speed );
	utils::animate( m_visual_saturation, m_saturation, detail::anim::cursor_speed );
	utils::animate( m_visual_brightness, m_brightness, detail::anim::cursor_speed );
	utils::animate( m_visual_alpha, m_var->m_a / 255.f, detail::anim::cursor_speed );
}

void gui::objects::colorpicker::reposition( const render::point& delta ) {
	base_object::reposition( delta );
	popup::reposition( delta );
	m_hue_area.add_pos( delta );
	m_sat_area.add_pos( delta );
	m_alpha_area.add_pos( delta );
	m_og_col_area.add_pos( delta );
}