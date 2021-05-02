#include "../includes.h"

int D3D::Width = 0;
int D3D::Height = 0;
int D3D::InternalBufferIndex = 0;

IDirect3DDevice9 *D3D::pDevice = nullptr;

std::unique_ptr<util::draw::draw_manager> D3D::D3D9DrawManager;
std::unordered_map< size_t, util::draw::font * > D3D::D3D9Fonts;
std::unordered_map< size_t, std::unique_ptr<D3D::SimpleSprite> > D3D::D3D9Textures;

std::once_flag flag;
void D3D::Initialise( IDirect3DDevice9 *device ) {
	if ( !device )
		return;

	pDevice = device;

	D3DVIEWPORT9 view_port{ };
	device->GetViewport( &view_port );

	Width = ( view_port.Width );
	Height = ( view_port.Height );

	D3D9DrawManager = std::make_unique<util::draw::d3d9_manager>( pDevice );

	if ( !D3D9DrawManager )
		return; //Maybe throw a std::runtime_error whatever

	D3D9DrawManager->update_screen_size(
		util::draw::position( ( Width ),
		( Height )
		) );

	InternalBufferIndex = D3D9DrawManager->register_buffer( );

	D3D9Fonts[ MAIN_FONT ] = D3D9DrawManager->add_font( XOR( "C:\\Windows\\Fonts\\segoeui.ttf" ), 14.f,
		false, false, util::draw::GLYPH_RANGE_LATIN, util::draw::NO_ANTIALIASING | util::draw::MONO_HINTING );	
	
	D3D9Fonts[ GROUP_FONT ] = D3D9DrawManager->add_font( XOR( "C:\\Windows\\Fonts\\segoeuib.ttf" ), 14.f,
		false, false, util::draw::GLYPH_RANGE_LATIN, util::draw::NO_ANTIALIASING | util::draw::MONO_HINTING );

	D3D9Fonts[ TAB_FONT ] = D3D9DrawManager->add_font(XOR( "C:\\Windows\\Fonts\\segoeuib.ttf" ), 21.f,
		false, false, util::draw::GLYPH_RANGE_LATIN, util::draw::NO_ANTIALIASING | util::draw::MONO_HINTING );	
	
	D3D9Fonts[ PIXEL_FONT ] = D3D9DrawManager->add_font(XOR( "C:\\Windows\\Fonts\\segoeuib.ttf" ), 8.f,
		false, false, util::draw::GLYPH_RANGE_LATIN, util::draw::NO_ANTIALIASING | util::draw::MONO_HINTING );

	D3D9Textures[ TITLEBAR_TEXTURE ] = std::make_unique<SimpleSprite>( D3D9DrawManager, util::draw::position( 598, 30 ), title_bar_data );
}

void D3D::Draw( ) {
	D3D9DrawManager->draw( );
}

void D3D::End( ) {
	D3D9DrawManager->swap_buffers( InternalBufferIndex );
}

void D3D::DrawTexture( const vec2_t& pos, const vec2_t& size, size_t texture, float alpha ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if( !buf )
		return;
	
	if( buf && D3D::D3D9Textures[ texture ] && D3D::D3D9Textures[ texture ]->IsValid( ) )
		D3D::D3D9Textures[ texture ]->Render( buf, util::draw::rect( ( int )pos.x, ( int )pos.y, ( int )size.x, ( int )size.y ), alpha );
}

void D3D::RectFilled( const vec2_t &pos, const vec2_t &size, Color &colour ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->rectangle_filled( util::draw::position( int( pos.x ), int( pos.y ) ), util::draw::position( int( size.x + pos.x ), int( size.y + pos.y ) ),
		{ colour.r( ), colour.g( ), colour.b( ), colour.a( ) } );
}

void D3D::SetBlur( float blur ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->set_blur( blur );
}

void D3D::TriangleFilled( const vec2_t &pos1, const vec2_t &pos2, const vec2_t &pos3, Color &colour ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->triangle_filled( util::draw::position( pos1.x, pos1.y ), util::draw::position( pos2.x, pos2.y ), util::draw::position( pos3.x, pos3.y ), { colour.r( ), colour.g( ), colour.b( ), colour.a( ) } );
}

void D3D::Rect( const vec2_t &pos, const vec2_t &size, Color &colour, const float &thickness ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->rectangle( util::draw::position( int( pos.x ), int( pos.y ) ), util::draw::position( int( pos.x + size.x ), int( pos.y + size.y ) ), 1.0f, { colour.r( ), colour.g( ), colour.b( ), colour.a( ) } );
}

void D3D::Circle( const vec2_t &center, const float radius, Color col, const float thickness, uint32_t parts, float degrees, float start_degree ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->circle( util::draw::position( int( center.x ), int( center.y ) ), radius, { col.r( ), col.g( ), col.b( ), col.a( ) }, 1.0f, parts, degrees, start_degree );
}

void D3D::CircleFilled( const vec2_t &center, const float radius, Color col, const float thickness, uint32_t parts, float degrees, float start_degree ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	//buf->circle_filled( util::draw::position( int( center.x ), int( center.y ) ), radius, { col.r( ), col.g( ), col.b( ), col.a( ) }, parts, true );
	buf->circle_filled( util::draw::position( int( center.x ), int( center.y ) ), radius, { col.r( ), col.g( ), col.b( ), col.a( ) }, { col.r( ), col.g( ), col.b( ), col.a( ) }, parts, degrees, start_degree );
}

vec2_t D3D::GetScreenCenter( ) {
	return GetScreenSize( ) / 2;
}

void D3D::RectFilledLinearGradient( const vec2_t &pos, const vec2_t &size, Color &colour1, Color &colour2, bool horizontal ) {
	if ( horizontal ) {
		RectFilled( pos, size, colour1 );

		auto first = colour2.r( );
		auto second = colour2.g( );
		auto third = colour2.b( );

		for ( auto i = 0; i < size.x; ++i ) {
			const float fi = i, fw = size.x;
			const auto a = fi / fw;
			const unsigned int ia = a * 255;

			RectFilled( vec2_t( pos.x + i, pos.y ), vec2_t( 1.f, size.y ), Color( first, second, third, int( ia * ( colour2.a( ) / 255.f ) ) ) );
		}
	}
	else {
		RectFilled( pos, size, colour1 );

		auto first = colour2.r( );
		auto second = colour2.g( );
		auto third = colour2.b( );

		for ( auto i = 0; i < size.y; ++i ) {
			const float fi = i, fh = size.y;
			const auto a = fi / fh;
			const unsigned int ia = a * 255;

			RectFilled( vec2_t( pos.x, pos.y + i ), vec2_t( size.x, 1.f ), Color( first, second, third, int( ia * ( (float)colour2.a( ) / 255.f ) ) ) );
		}
	}
}

void D3D::Text( const vec2_t &pos, const std::string &text, Color &colour, util::draw::font *font, const uint8_t flags ) {
	if ( !font )
		return;

	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	const auto text_data = text.c_str( );

	if ( flags & EFontFlags::OUTLINED ) {
		const auto outline_color{ util::draw::color( 1, 1, 1, colour.a( ) ) };

		buf->text( font, text_data, { ( pos.x ), ( pos.y - 1 ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x ), ( pos.y + 1 ) }, outline_color, false );

		buf->text( font, text_data, { ( pos.x - 1 ), ( pos.y ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y ) }, outline_color, false );

		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y + 1 ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x - 1 ), ( pos.y + 1 ) }, outline_color, false );

		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y - 1 ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x - 1 ), ( pos.y - 1 ) }, outline_color, false );
	}
	else if ( flags & EFontFlags::DROPSHADOW ) {
		const auto outline_color{ util::draw::color( 1, 1, 1, static_cast< uint8_t >( std::clamp( int( colour.a( ) ) - 10, 0, 255 ) ) ) };

		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y + 1 ) }, outline_color, false );
	}

	buf->text( font, text_data, { ( pos.x ), ( pos.y ) }, { colour.r( ), colour.g( ), colour.b( ), colour.a( ) }, false );
}

void D3D::Text( const vec2_t &upos, const std::string &text, Color &colour, util::draw::font *font, const uint8_t flags, const EStringAlign align ) {
	if ( !font )
		return;

	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	const auto text_data = text.c_str( );

	int width, height;
	width = GetTextSize( text, font ).x;
	height = GetTextSize( text, font ).y;

	vec2_t pos( upos );

	switch ( align ) {
	case TEXT_ALIGN_CENTER:
		pos.x -= width / 2;
		break;
	case TEXT_ALIGN_RIGHT:
		pos.x -= width;
		break;
	case TEXT_ALIGN_CENTER_CENTER:
		pos.x -= height / 2;
		pos.y -= height / 2;
		break;
	case TEXT_ALIGN_CENTER_TOP:
		pos.x -= width / 2;
		pos.y -= height;
		break;
	case TEXT_ALIGN_CENTER_BOT:
		pos.x -= width / 2;
		pos.y += height;
		break;
	case TEXT_ALIGN_LEFT_CENTER:
		pos.y -= height / 2;
		break;
	case TEXT_ALIGN_LEFT_TOP:
		pos.y -= height;
		break;
	case TEXT_ALIGN_LEFT_BOT:
		pos.y += height;
		break;
	case TEXT_ALIGN_RIGHT_CENTER:
		pos.x -= width;
		pos.y -= height / 2;
		break;
	case TEXT_ALIGN_RIGHT_TOP:
		pos.x -= width;
		pos.y -= height;
		break;
	case TEXT_ALIGN_RIGHT_BOT:
		pos.x -= width;
		pos.y += height;
		break;
	case TEXT_ALIGN_NONE:
		break;
	default: break;
	}

	if ( flags & EFontFlags::OUTLINED ) {
		const auto outline_color{ util::draw::color( 1, 1, 1, colour.a( ) ) };

		buf->text( font, text_data, { ( pos.x ), ( pos.y - 1 ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x ), ( pos.y + 1 ) }, outline_color, false );

		buf->text( font, text_data, { ( pos.x - 1 ), ( pos.y ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y ) }, outline_color, false );

		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y + 1 ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x - 1 ), ( pos.y + 1 ) }, outline_color, false );

		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y - 1 ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x - 1 ), ( pos.y - 1 ) }, outline_color, false );
	}
	else if ( flags & EFontFlags::DROPSHADOW ) {
		const auto outline_color{ util::draw::color( 1, 1, 1, static_cast< uint8_t >( std::clamp( int( colour.a( ) ) - 10, 0, 255 ) ) ) };

		buf->text( font, text_data, { ( pos.x ), ( pos.y + 1 ) }, outline_color, false );
		buf->text( font, text_data, { ( pos.x + 1 ), ( pos.y ) }, outline_color, false );
	}

	buf->text( font, text_data, { ( pos.x ), ( pos.y ) }, { colour.r( ), colour.g( ), colour.b( ), colour.a( ) }, false );
}

void D3D::Line( const vec2_t &pos, const vec2_t &pos1, Color &colour, float thickness ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->line( { ( pos.x ), ( pos.y ) }, { ( pos1.x ), ( pos1.y ) }, { colour.r( ), colour.g( ), colour.b( ), colour.a( ) }, thickness, false );
}

void D3D::SmoothLine( const vec2_t &pos, const vec2_t &pos1, bool anti_aliasing, float thickness, Color &colour ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->line( util::draw::position( int( pos.x ), int( pos.y ) ), util::draw::position( int( pos1.x ), int( pos1.y ) ), { colour.r( ), colour.g( ), colour.b( ), colour.a( ) }, thickness, anti_aliasing );
}

void D3D::Pixel( const vec2_t &pos, Color &colour ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->rectangle_filled( util::draw::position( int( pos.x ), int( pos.y ) ), util::draw::position( int( 1 + pos.x ), int( 1 + pos.y ) ),
		{ colour.r( ), colour.g( ), colour.b( ), colour.a( ) } );
}

vec2_t D3D::GetTextSize( const std::string &text, util::draw::font *font ) {
	if ( !font )
		return vec2_t( 0, 0 );

	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return vec2_t( 0, 0 );

	const auto size{ buf->text_size( font, text.c_str( ) ) };

	return vec2_t( size.x, size.y );
}

std::pair<vec2_t, vec2_t> D3D::GetLimit( ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return std::pair<vec2_t, vec2_t>
		( vec2_t( 0, 0 ), vec2_t( 0, 0 ) );

	return std::pair<vec2_t, vec2_t>
		( vec2_t( buf->cur_clip_rect( ).x, buf->cur_clip_rect( ).y ),
			vec2_t( buf->cur_clip_rect( ).x + buf->cur_clip_rect( ).z, buf->cur_clip_rect( ).y + buf->cur_clip_rect( ).w ) );
}

void D3D::SetLimit( std::pair<vec2_t, vec2_t> limit ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->push_clip_rect( { ( limit.first.x ), ( limit.first.y ), ( limit.first.x + limit.second.x ), ( limit.first.y + limit.second.y ) }, false );
}

void D3D::ResetLimit( ) {
	const auto buf{ D3D9DrawManager->get_buffer( InternalBufferIndex ) };

	if ( !buf )
		return;

	buf->pop_clip_rect( );
}

void D3D::HandleReset( ) {
	reinterpret_cast< util::draw::d3d9_manager * >( D3D9DrawManager.get( ) )->invalidate_device_objects( );
	reinterpret_cast< util::draw::d3d9_manager * >( D3D9DrawManager.get( ) )->create_device_objects( );

	//should work
	D3DVIEWPORT9 view_port{ };
	pDevice->GetViewport( &view_port );

	Width = ( view_port.Width );
	Height = ( view_port.Height );

	D3D9DrawManager->update_screen_size(
		util::draw::position( ( Width ),
		( Height )
		) );
}

vec2_t D3D::GetScreenSize( ) {
	return vec2_t( Width, Height );
}

void D3D::GetScreenSize( int &width, int &height ) {
	width = Width;
	height = Height;
}

util::draw::draw_buffer *D3D::GetBuffer( ) {
	return D3D9DrawManager->get_buffer( InternalBufferIndex );
}
