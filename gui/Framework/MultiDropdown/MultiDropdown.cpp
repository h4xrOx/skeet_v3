#include "../gui.h"
#include <algorithm>

bool GUI::Controls::MultiDropdown( const std::string& name, std::vector< MultiItem_t > options ) {
	vec2_t CursorPos = PopCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;
	vec2_t DrawSize = vec2_t( ctx->ParentSize.x - 40 > 300 ? 300 : ctx->ParentSize.x - 40, 23 );

	if( ctx->MultiDropdownInfo._ArrowAnimation.find( GUI::Hash( name ) ) == ctx->MultiDropdownInfo._ArrowAnimation.end( ) ) {
		ctx->MultiDropdownInfo._ArrowAnimation.insert( { GUI::Hash( name ), 0.f } );
		ctx->MultiDropdownInfo._ArrowTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->MultiDropdownInfo._CrossAnimation.find( GUI::Hash( name ) ) == ctx->MultiDropdownInfo._CrossAnimation.end( ) ) {
		ctx->MultiDropdownInfo._CrossAnimation.insert( { GUI::Hash( name ), 0.f } );
		ctx->MultiDropdownInfo._CrossTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->FocusedID == GUI::Hash( name ) ) {
		if( ctx->MultiDropdownInfo.OutAnimation ) {
			ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ) -= ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) = Easing::InQuint( ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ) );

			if( ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) <= 0.01f ) {
				ctx->FocusedID = 0;
				ctx->MultiDropdownInfo.HashedID = 0;
				ctx->MultiDropdownInfo.OutAnimation = false;

				ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ) = ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) = 0.f;
			}
		}
		else {
			ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( ctx->MultiDropdownInfo._ArrowTimer.at( GUI::Hash( name ) ) );

			if( ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) >= 0.9f ) {
				ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) = 1.f;
			}
		}
	}

	if( ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) >= 1.0f ) {
		ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
		ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

		ctx->MultiDropdownInfo._CrossAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ) );

		if( ctx->MultiDropdownInfo._CrossAnimation.at( GUI::Hash( name ) ) >= 0.999998 ) {
			ctx->MultiDropdownInfo._CrossAnimation.at( GUI::Hash( name ) ) = 1.f;
		}
	}
	else {
		ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ) -= ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
		ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

		ctx->MultiDropdownInfo._CrossAnimation.at( GUI::Hash( name ) ) = Easing::InQuint( ctx->MultiDropdownInfo._CrossTimer.at( GUI::Hash( name ) ) );
	}

	D3D::Text( DrawPos - 2, SplitStr( name.data( ), '#' )[ 0 ].data( ), Color( 255, 255, 255 ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );

	DrawPos.y += D3D::GetTextSize( SplitStr( name.data( ), '#' )[ 0 ].data( ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y + 1;

	bool Hovered = InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && InputHelper::Hovered( DrawPos, DrawSize );

	Color ArrowClr(
		Color::Blend( Color::Palette_t::MenuColors_t::Outlines( ), g_cfg[ XOR( "menu_color" ) ].get_color( ), ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) ) );
	
	Color PixelClr(
		Color::Blend( Color::Palette_t::MenuColors_t::Container( ), ArrowClr, ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) ) ) );

	vec2_t CastedDrawPos = vec2_t( ( int )DrawPos.x, ( int )DrawPos.y );
	vec2_t CastedDrawSize = vec2_t( ( int )DrawSize.x, ( int )DrawSize.y );

	float anim = ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) );
	vec2_t ArrowDown[ 3 ] = {
		( ( CastedDrawPos + CastedDrawSize ) - vec2_t( ( int )18, ( int )14 ) ) + vec2_t( 0, math::blend( 0, 5, anim ) ),
		( ( CastedDrawPos + CastedDrawSize ) - vec2_t( ( int )18, ( int )14 ) ) + vec2_t( ( int )5, ( int )math::blend( 5, 0, anim ) ),
		( ( CastedDrawPos + CastedDrawSize ) - vec2_t( ( int )18, ( int )14 ) ) + vec2_t( ( int )10, math::blend( 0, 5, anim ) ) + vec2_t( math::blend( 1, 0, anim ), math::blend( -1, 0, anim ) )
	};

	// arrow ting
	{
		D3D::SmoothLine(
			ArrowDown[ 1 ],//middle top
			ArrowDown[ 0 ],//bottom left
			false, 2.f, ArrowClr.OverrideAlpha( 255 * ctx->animation, true ) );

		D3D::SmoothLine(
			ArrowDown[ 2 ] - vec2_t( 1, 0 ),//bottom right
			ArrowDown[ 1 ] - vec2_t( 1, 0 ),//middle top
			false, 2.f, ArrowClr.OverrideAlpha( 255 * ctx->animation, true ) );

		// dont ask
		D3D::Pixel( ArrowDown[ 2 ] - vec2_t( 2, 0 ), ArrowClr.OverrideAlpha( 255 * ctx->animation, true ) );
		D3D::Pixel( ArrowDown[ 2 ] - vec2_t( math::blend( 2, 1, anim ), math::blend( 0, 1, anim ) ), PixelClr.OverrideAlpha( 255 * ctx->animation, true ) );
	}

	D3D::Rect( DrawPos, DrawSize, ArrowClr.OverrideAlpha( 255 * ctx->animation, true ) );

	std::string buffer;
	if( options.size( ) ) {
		for( auto n = 0; n < options.size( ); ++n ) {
			auto& it = options.at( n ).value;
			auto& it_name = options.at( n ).name;
			auto format_len = buffer.length( ) < 30;

			if( g_cfg[ it ].get<bool>( ) && format_len ) {
				if( buffer.length( ) > 0 )
					buffer += XOR( ", " );

				buffer += it_name;
			}
		}

		if( buffer.length( ) > 30 ) {
			buffer.resize( 30 );
			buffer += XOR( " ..." );
		}

		if( !buffer.length( ) ) {
			buffer += XOR( "None" );
		}
	}

	D3D::Text( DrawPos + vec2_t( 4, 3 ), buffer.c_str( ), Color( 255, 255, 255 ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );

	if( ctx->FocusedID == 0 ) {
		if( Hovered && InputHelper::Pressed( VK_LBUTTON ) ) {
			ctx->FocusedID = GUI::Hash( name );
		}
	}
	else if( ctx->FocusedID == GUI::Hash( name ) ) {
		ctx->MultiDropdownInfo.Animation = ctx->MultiDropdownInfo._ArrowAnimation.at( GUI::Hash( name ) );

		ctx->MultiDropdownInfo.Elements = options;
		ctx->MultiDropdownInfo.Size = DrawSize.x;
		ctx->MultiDropdownInfo.Pos = DrawPos + vec2_t( 0, DrawSize.y );
		ctx->MultiDropdownInfo.HashedID = GUI::Hash( name );
	}

	PushCursorPos( CursorPos + vec2_t( 0, DrawSize.y + D3D::GetTextSize( name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y + GUI::ObjectPadding( ) ) );
	return ctx->FocusedID == GUI::Hash( name );
}
