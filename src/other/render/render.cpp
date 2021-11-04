#include "render.hpp"

#include "imgui.h"
#include "imgui_impl_dx9.h"

#include "../../sdk/csgo/csgo.hpp"

#define FONT_GET( font ) m_fonts[ ENUM_IDX( font ) ]

bool render::impl::init( ) {
	m_d3d9.m_device = g_csgo.m_d3d9_device;

	MOCKING_TRY;

	MOCK m_imgui.init( m_d3d9.m_device );
	MOCK FONT_GET( font::MENU ).init( XOR( "C:\\Windows\\Fonts\\segoeui.ttf" ), 16.f );

	MOCKING_CATCH( return false );

	update_screen_size( g_csgo.m_client_mode_shared->m_root_size );

	g_io.log( "initialized renderer" );

	return true;
}

#undef FONT_GET

void render::impl::unload( ) {
	ImGui_ImplDX9_Shutdown( );
	ImGui::DestroyContext( );
}

void render::impl::begin( ) {
	m_imgui.begin( );

	m_d3d9.backup_render_states( );
}

void render::impl::end( ) {
	m_imgui.end( );

	m_d3d9.restore_render_states( );
}

void render::impl::update_screen_size( const math::v2i& screen_size ) {
	m_screen_size               = screen_size;
	m_imgui.m_io->DisplaySize.x = static_cast< f32 >( screen_size[ X ] );
	m_imgui.m_io->DisplaySize.y = static_cast< f32 >( screen_size[ Y ] );
}

const math::v2i& render::impl::get_screen_size( ) const {
	return m_screen_size;
}

void render::impl::pre_reset( ) {
	ImGui_ImplDX9_InvalidateDeviceObjects( );
}

void render::impl::post_reset( ) {
	update_screen_size( g_csgo.m_engine->get_screen_size( ) );

	ImGui_ImplDX9_CreateDeviceObjects( );
}

render::point render::impl::handle_alignment( align alignment, const point& pos, const size& size ) {
	point out = pos;

	switch ( alignment ) {
	case align::NONE:
		return out;
	case align::NORMAL:
		return out += size;
	case align::LEFT:
		out[ X ] -= size[ X ];
		return out;
	case align::RIGHT:
		out[ X ] += size[ X ];
		return out;
	case align::X_CENTER:
		out[ X ] -= size[ X ] / 2;
		return out;
	case align::TOP:
		out[ Y ] -= size[ Y ];
		return out;
	case align::BOTTOM:
		out[ Y ] += size[ Y ];
		return out;
	case align::Y_CENTER:
		out[ Y ] -= size[ Y ] / 2;
		return out;
	case align::CENTERED:
		out -= size / 2;
		return out;
	default:
		return out;
	}
}

void render::impl::d3d9::backup_render_states( ) {
	m_device->CreateStateBlock( D3DSBT_PIXELSTATE, &m_backup.state_block );

	m_backup.state_block->Capture( );

	m_device->GetRenderState( D3DRS_COLORWRITEENABLE, &m_backup.color_write );
	m_device->GetRenderState( D3DRS_SRGBWRITEENABLE, &m_backup.srgb_write );

	m_device->GetVertexDeclaration( &m_backup.vtx_decl );
	m_device->GetVertexShader( &m_backup.vtx_shader );
	m_device->GetPixelShader( &m_backup.pixel_shader );
}

void render::impl::d3d9::restore_render_states( ) {
	m_backup.state_block->Apply( );
	release( m_backup.state_block );

	m_device->SetVertexDeclaration( m_backup.vtx_decl );
	m_device->SetVertexShader( m_backup.vtx_shader );
	m_device->SetPixelShader( m_backup.pixel_shader );

	release( m_backup.vtx_decl );
	release( m_backup.vtx_shader );
	release( m_backup.pixel_shader );

	m_device->SetRenderState( D3DRS_COLORWRITEENABLE, m_backup.color_write );
	m_device->SetRenderState( D3DRS_SRGBWRITEENABLE, m_backup.srgb_write );
}

bool render::impl::imgui::init( IDirect3DDevice9* device ) {
	MOCKING_TRY;

	MOCK m_ctx = ImGui::CreateContext( );
	MOCK m_io  = &ImGui::GetIO( );
	MOCK ImGui_ImplDX9_Init( device );

	MOCKING_CATCH( return false );

	return true;
}

void render::impl::imgui::begin( ) {
	ImGui_ImplDX9_NewFrame( );

	auto& style = ImGui::GetStyle( );

	style.AntiAliasedLines       = true;
	style.AntiAliasedLinesUseTex = true;
	style.AntiAliasedFill        = true;
	ImGui::NewFrame( );

	geometry::base_shape::s_draw_list = ImGui::GetForegroundDrawList( );
}

void render::impl::imgui::end( ) {
	ImGui::EndFrame( );

	ImGui::Render( );

	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
}