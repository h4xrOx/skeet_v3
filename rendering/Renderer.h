#pragma once

#include "Wrapper/DrawManager.h"
#include "Wrapper/Font.h"
#include "Wrapper/Maths.h"
#include "Wrapper/STBRectPack.h"
#include "Wrapper/Implementation/D3D9Manager.h"
#include "Wrapper/Implementation/D3D9Shaders.h"
#include "Wrapper/Implementation/D3D9Texture.h"

#include "Textures/titlebar.h"

namespace D3D {
	// full credits to shiro for writing this class
	// and being nice enough to share it with me :d
	class SimpleSprite {
		util::draw::tex_id m_texture_id;
	public:
		SimpleSprite( ) : m_texture_id{ nullptr } {

		}

		SimpleSprite( const std::unique_ptr<util::draw::draw_manager>& renderer, const util::draw::position& size, const uint8_t* data ) {
			Initialise( renderer, size, data );
		}

		/*
		 * No need to destroy the texture yourself because the tex_dict destructor does that.
		 */
		~SimpleSprite( ) = default;

		bool Initialise( const std::unique_ptr<util::draw::draw_manager>& renderer, const util::draw::position& size, const uint8_t* data ) {
			if( !renderer )
				return false;

			const auto w = static_cast< uint32_t >( size.x ), h = static_cast< uint32_t >( size.y );
			m_texture_id = renderer->create_texture( w, h );

			if( !m_texture_id )
				return false;

			if( !renderer->set_texture_rgba( m_texture_id, data, w, h ) )
				return false;

			return true;
		}

		/*
		 * If you want to have alpha, just pass { 255, 255, 255, alpha }
		 * The format is x, y, w, h
		 */
		void Render( util::draw::draw_buffer* const buffer, const util::draw::rect& rect, float alpha = 1.f ) const {
			if( !buffer || !m_texture_id )
				return;

			buffer->push_tex_id( m_texture_id );
			buffer->prim_reserve( 6, 4 );
			buffer->prim_rect_uv( { rect.x, rect.y }, { rect.z + rect.x, rect.w + rect.y }, { 0, 0 }, { 1, 1 }, util::draw::color( 255, 255, 255, 255 * alpha ) );
			buffer->pop_tex_id( );
		}

		[[nodiscard]] util::draw::tex_id GetTexture( ) const {
			return m_texture_id;
		}

		[[nodiscard]] bool IsValid( ) const {
			return m_texture_id != nullptr;
		}
	};

	extern int Width;
	extern int Height;
	extern int InternalBufferIndex;

	extern IDirect3DDevice9* pDevice;

	extern std::unique_ptr<util::draw::draw_manager> D3D9DrawManager;
	extern std::unordered_map< size_t, util::draw::font* > D3D9Fonts;
	extern std::unordered_map< size_t, std::unique_ptr<SimpleSprite> > D3D9Textures;

	enum EFonts {
		MAIN_FONT,
		PIXEL_FONT,
		GROUP_FONT,
		TAB_FONT,
		COUNTER_STRIKE_24
	};

	enum ETextures {
		TITLEBAR_TEXTURE
	};

	enum EStringAlign {
		TEXT_ALIGN_CENTER = 0,
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_RIGHT,
		TEXT_ALIGN_CENTER_CENTER,
		TEXT_ALIGN_CENTER_TOP,
		TEXT_ALIGN_CENTER_BOT,
		TEXT_ALIGN_LEFT_CENTER,
		TEXT_ALIGN_LEFT_TOP,
		TEXT_ALIGN_LEFT_BOT,
		TEXT_ALIGN_RIGHT_CENTER,
		TEXT_ALIGN_RIGHT_TOP,
		TEXT_ALIGN_RIGHT_BOT,
		TEXT_ALIGN_NONE
	};

	enum EFontFlags {
		NONE = 1 << 0,
		CENTERED_X = 1 << 1,
		CENTERED_Y = 1 << 2,
		OUTLINED = 1 << 3,
		DROPSHADOW = 1 << 4,
		ANTIALIASED = 1 << 5
	};

	util::draw::draw_buffer* GetBuffer( );

	void Initialise( IDirect3DDevice9* device );
	void Draw( );
	void End( );
	void DrawTexture( const vec2_t& pos, const vec2_t& size, size_t texture, float alpha );

	void HandleReset( );

	void RectFilled( const vec2_t& pos, const vec2_t& size, Color& colour );
	void SetBlur( float blur );
	void TriangleFilled( const vec2_t& pos1, const vec2_t& pos2, const vec2_t& pos3, Color& colour );
	void Rect( const  vec2_t& pos, const vec2_t& size, Color& colour, const float& thickness = 1.f );
	void Circle( const vec2_t& center, const float radius, Color col, const float thickness = 1.0f, uint32_t parts = 12U, float degrees = 360.f, float start_degree = 0.f );
	void CircleFilled( const vec2_t& center, const float radius, Color col, const float thickness = 1.0f, uint32_t parts = 12U, float degrees = 360.f, float start_degree = 0.f );
	void RectFilledLinearGradient( const vec2_t& pos, const vec2_t& size, Color& colour1, Color& colour2, bool horizontal = false );
	void Text( const vec2_t& pos, const std::string& text, Color& colour, util::draw::font* font, const uint8_t flags );
	void Text( const vec2_t& upos, const std::string& text, Color& colour, util::draw::font* font, const uint8_t flags, const EStringAlign align );
	void Line( const vec2_t& pos, const vec2_t& pos1, Color& colour, float thickness = 1.0f );
	void SmoothLine( const vec2_t& pos, const vec2_t& pos1, bool anti_aliasing, float thickness, Color& colour );
	void Pixel( const vec2_t& pos, Color& colour );
	vec2_t GetTextSize( const std::string& text, util::draw::font* font );

	vec2_t GetScreenCenter( );

	vec2_t GetScreenSize( );
	void GetScreenSize( int& width, int& height );

	std::pair<vec2_t, vec2_t> GetLimit( );
	void SetLimit( std::pair<vec2_t, vec2_t> limit );
	void ResetLimit( );
}