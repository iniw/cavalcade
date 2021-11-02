#include "../../types/types.hpp"

#include "imgui.h"

#include "font.hpp"

bool render::geometry::font::init( std::string_view path, f32 size ) {
	ImGuiIO* io = &ImGui::GetIO( );

	m_size = size;
	m_font = io->Fonts->AddFontFromFileTTF( path.data( ), m_size );

	return m_font;
}

void render::geometry::font::draw( const point& pos, std::string_view text, color col ) {
	base_shape::s_draw_list->AddText( m_font, m_size, pos, col.to_imgui( ), text.data( ) );
}

render::size render::geometry::font::calc_size( std::string_view text ) {
	return m_font->CalcTextSizeA( m_size, FLT_MAX, 0.f, text.data( ) );
}