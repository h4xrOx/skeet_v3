#include "../gui.h"
#include <algorithm>

bool GUI::Controls::Dropdown( const std::string& name, std::vector< std::string > options, const std::string& var_name ) {
	vec2_t CursorPos = PopCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;
	vec2_t DrawSize = vec2_t( ctx->ParentSize.x - 40 > 300 ? 300 : ctx->ParentSize.x - 40, 23 );

	if( ctx->DropdownInfo.ArrowAnimation.find( GUI::Hash( name ) ) == ctx->DropdownInfo.ArrowAnimation.end( ) ) {
		ctx->DropdownInfo.ArrowAnimation.insert( { GUI::Hash( name ), 0.f } );
		ctx->DropdownInfo.ArrowTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->DropdownInfo.CrossAnimation.find( GUI::Hash( name ) ) == ctx->DropdownInfo.CrossAnimation.end( ) ) {
		ctx->DropdownInfo.CrossAnimation.insert( { GUI::Hash( name ), 0.f } );
		ctx->DropdownInfo.CrossTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->FocusedID == GUI::Hash( name ) ) {
		if( ctx->DropdownInfo.OutAnimation ) {
			ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ) -= ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) = Easing::InQuint( ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ) );

			if( ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) <= 0.01f ) {
				ctx->FocusedID = 0;
				ctx->DropdownInfo.HashedID = 0;
				ctx->DropdownInfo.OutAnimation = false;

				ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ) = ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) = 0.f;
			}
		}
		else {
			ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( ctx->DropdownInfo.ArrowTimer.at( GUI::Hash( name ) ) );

			if( ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) >= 0.9f ) {
				ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) = 1.f;
			}
		}
	}

	if( ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) >= 1.0f ) {
		ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
		ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

		ctx->DropdownInfo.CrossAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ) );

		if( ctx->DropdownInfo.CrossAnimation.at( GUI::Hash( name ) ) >= 0.999998 ) {
			ctx->DropdownInfo.CrossAnimation.at( GUI::Hash( name ) ) = 1.f;
		}
	}
	else {
		ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ) -= ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
		ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

		ctx->DropdownInfo.CrossAnimation.at( GUI::Hash( name ) ) = Easing::InQuint( ctx->DropdownInfo.CrossTimer.at( GUI::Hash( name ) ) );
	}

	D3D::Text( DrawPos - 2, GUI::SplitStr( name, '#' )[ 0 ].data( ), Color( 255, 255, 255 ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );

	DrawPos.y += D3D::GetTextSize( GUI::SplitStr( name, '#' )[ 0 ].data( ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y + 1;

	bool Hovered = InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && InputHelper::Hovered( DrawPos, DrawSize );

	Color ArrowClr(
		Color::Blend( Color::Palette_t::MenuColors_t::Outlines( ), g_cfg[ XOR( "menu_color" ) ].get_color( ), ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) ) );

	Color PixelClr(
		Color::Blend( Color::Palette_t::MenuColors_t::Container( ), ArrowClr, ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) ) ) );

	vec2_t CastedDrawPos = vec2_t( ( int )DrawPos.x, ( int )DrawPos.y );
	vec2_t CastedDrawSize = vec2_t( ( int )DrawSize.x, ( int )DrawSize.y );

	float anim = ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) );
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

	int value = g_cfg[ var_name ].get<int>( );

	if( value > options.size( ) - 1 ) {
		g_cfg[ var_name ].set<int>( options.size( ) - 1 );
	}
	else if( value < 0 ) {
		g_cfg[ var_name ].set<int>( 0 );
	}

	D3D::Text( DrawPos + vec2_t( 4, 3 ), options.at( g_cfg[ var_name ].get<int>( ) ), Color( 255, 255, 255 ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );

	if( ctx->FocusedID == 0 ) {
		if( Hovered && InputHelper::Pressed( VK_LBUTTON ) ) {
			ctx->FocusedID = GUI::Hash( name );
		}
	}
	else if( ctx->FocusedID == GUI::Hash( name ) ) {
		ctx->DropdownInfo.Animation = ctx->DropdownInfo.ArrowAnimation.at( GUI::Hash( name ) );

		ctx->DropdownInfo.Elements = options;
		ctx->DropdownInfo.Option = var_name;
		ctx->DropdownInfo.Size = DrawSize.x;
		ctx->DropdownInfo.Pos = DrawPos + vec2_t( 0, DrawSize.y );
		ctx->DropdownInfo.HashedID = GUI::Hash( name );
	}

	PushCursorPos( CursorPos + vec2_t( 0, DrawSize.y + D3D::GetTextSize( name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y + GUI::ObjectPadding( ) ) );
	return ctx->FocusedID == GUI::Hash( name );
}
