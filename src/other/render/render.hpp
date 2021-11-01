#ifndef RENDER_HPP
#define RENDER_HPP

#include "types/types.hpp"
#include "geometry/geometry.hpp"

struct ImGuiContext;
struct ImGuiIO;
struct ImDrawList;

namespace render {
	enum class align
	{
		NONE = 0,
		LEFT,
		RIGHT,
		X_CENTER,
		TOP,
		BOTTOM,
		Y_CENTER,
		CENTERED
	};

	enum class font
	{
		MENU = 0,
		MAX
	};

	struct render {
	private:

		// TODO(wini): move these two structs to their own files/folders
		struct d3d9 {
			struct {
				IDirect3DStateBlock9* state_block;
				IDirect3DVertexDeclaration9* vtx_decl;
				IDirect3DVertexShader9* vtx_shader;
				IDirect3DPixelShader9* pixel_shader;
				DWORD color_write, srgb_write;
			} m_backup;

			IDirect3DDevice9* m_device;

			template< typename T >
			void release( T*& obj ) {
				if ( obj ) {
					obj->Release( );
					obj = nullptr;
				}
			}

			void backup_render_states( );

			void restore_render_states( );
		} m_d3d9;

		struct imgui {
			ImGuiContext* m_ctx;

			ImGuiIO* m_io;

			bool init( IDirect3DDevice9* device );

			void begin( );

			void end( );
		} m_imgui;

		std::array< geometry::font, ENUM_IDX( font::MAX ) > m_fonts;

		// helper for drawing any geometry
		// this can't be put in render.inl for some reason
		template< geometry::shape T, typename... VA >
		T draw_shape( VA&&... args ) {
			T shape( std::forward< VA >( args )... );

			shape.draw( );

			return shape;
		}

		// to the abstraced wrappers we pass pos + size,
		// but the geometry classes use two positions,
		// so we use this function to handle that and aligment quickly
		point handle_alignment( align alignment, const point& pos, const size& size );

	public:

		bool init( );

		void unload( );

		void begin( );

		void end( );

		template< align alignment = align::NONE >
		geometry::rect rectangle( const point& pos, const size& size, color col, f32 thickness = 1.f );

		template< font font_choice >
		size text( const point& pos, std::string_view text, color col );

		void pre_reset( );

		void post_reset( );

		void update_screen_size( const math::v2i& screen_size );
	};
} // namespace render

inline render::render g_render;

#include "render.inl"

#endif // RENDER_HPP