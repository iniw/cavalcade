#pragma once

#include "types/types.hpp"
#include "geometry/geometry.hpp"

struct ImGuiContext;
struct ImGuiIO;
struct ImDrawList;

namespace render {
	enum class align
	{
		NONE = 0,
		NORMAL,
		LEFT,
		RIGHT,
		X_CENTER,
		TOP,
		BOTTOM,
		Y_CENTER,
		CENTERED
	};

	enum font
	{
		MENU = 0,
		ESP,
		IND_BIG,
		IND_SMALL,
		MAX
	};

	struct impl {
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

		math::v2i m_screen_size;

		// helper for drawing any geometry
		// this can't be put in render.inl for some reason
		template< geometry::Shape T, typename... VA >
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

		std::array< geometry::font, font::MAX > m_fonts;

		struct safe {
			std::shared_mutex m_rendering_mutex;
			std::vector< std::shared_ptr< geometry::base_shape > > m_queue_back;
			std::vector< std::shared_ptr< geometry::base_shape > > m_queue_front;

			template< geometry::Shape T, typename... VA >
			inline void draw_shape( VA&&... args ) {
				m_queue_back.push_back( std::make_shared< T >( std::forward< VA >( args )... ) );
			}

			template< geometry::Shape T, typename... VA >
			inline void draw_shape_front( VA&&... args ) {
				m_queue_front.push_back( std::make_shared< T >( std::forward< VA >( args )... ) );
			}

			template< geometry::Shape T >
			inline void draw_shape_p( std::shared_ptr< T >&& p ) {
				m_queue_back.push_back( p );
			}

			template< geometry::Shape T >
			inline void draw_shape_front_p( std::shared_ptr< T >&& p ) {
				m_queue_front.push_back( p );
			}

			inline void clear( ) {
				m_queue_back.clear( );
				m_queue_front.clear( );
			}

			void frame( std::function< void( ) > fn ) {
				std::unique_lock lock( m_rendering_mutex );
				clear( );
				fn( );
			}
		} m_safe;

		bool init( );

		void unload( );

		void begin( );

		void end( );

		template< align alignment = align::NORMAL >
		geometry::rect rectangle( const point& pos, const size& size, color col, f32 thickness = 1.f );

		template< align alignment = align::NORMAL >
		geometry::rect rectangle( const rect& rect, color col, f32 thickness = 1.f );

		template< align alignment = align::NORMAL >
		geometry::rect_filled rectangle_filled( const point& pos, const size& size, color col );

		template< align alignment = align::NORMAL >
		geometry::rect_filled rectangle_filled( const rect& rect, color col );

		template< font font_choice >
		size text( const point& pos, std::string_view text, color col, align alignment = align::NONE );

		template< font font_choice >
		size text_size( std::string_view text );

		void push_clip_rect( const rect& rect );

		void pop_clip_rect( );

		void pre_reset( );

		void post_reset( );

		void update_screen_size( const math::v2i& screen_size );

		const math::v2i& get_screen_size( ) const;
	};
} // namespace render

inline render::impl g_render;

#include "render.inl"
