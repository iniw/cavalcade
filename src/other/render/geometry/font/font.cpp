#include "font.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_freetype.h"

bool render::geometry::font::init( std::string_view path, f32 size ) {
	ImGuiIO* io = &ImGui::GetIO( );

	ImFontConfig cfg = ImFontConfig( );
	cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LightHinting;

	m_size   = size;
	m_font   = io->Fonts->AddFontFromFileTTF( path.data( ), m_size, &cfg );
	m_height = calc_size( XOR( "Q" ) ).h;

	return m_font;
}

void render::geometry::font::draw( const point& pos, std::string_view text, color col ) const {
	base_shape::s_draw_list->AddText( m_font, m_size, pos, col.convert( ), text.data( ) );
}

render::size render::geometry::font::calc_size( std::string_view text ) const {
	return m_font->CalcTextSizeA( m_size, FLT_MAX, 0.f, text.data( ) );
}