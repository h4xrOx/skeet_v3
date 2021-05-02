#include <algorithm>
#include "../gui.h"

std::map<size_t, float> ValueAnimation;
std::map<size_t, float> ValueTimer;

std::map<size_t, float> PreviewAnimation;
std::map<size_t, float> PreviousAmount;

std::map<size_t, float> LastChangeTime;

std::map<size_t, std::pair<float, bool>> ShouldChangeValue;

bool GUI::Controls::Slider( const std::string& name, const std::string& var_name, float min, float max, const std::string& display ) {
	vec2_t CursorPos = PopCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;
	vec2_t DrawSize( ctx->ParentSize.x - 40 > 300 ? 300 : ctx->ParentSize.x - 40, 10 );

	if( ctx->SliderInfo.ValueAnimation.find( GUI::Hash( name ) ) == ctx->SliderInfo.ValueAnimation.end( ) ) {
		ctx->SliderInfo.ValueAnimation.insert( { GUI::Hash( name ), 0.f } );
		ctx->SliderInfo.ValueTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->SliderInfo.PreviewAnimation.find( GUI::Hash( name ) ) == ctx->SliderInfo.PreviewAnimation.end( ) ) {
		ctx->SliderInfo.PreviewAnimation.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->SliderInfo.PreviousAmount.find( GUI::Hash( name ) ) == ctx->SliderInfo.PreviousAmount.end( ) ) {
		ctx->SliderInfo.PreviousAmount.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->SliderInfo.LastChangeTime.find( GUI::Hash( name ) ) == ctx->SliderInfo.LastChangeTime.end( ) ) {
		ctx->SliderInfo.LastChangeTime.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->SliderInfo.ShouldChangeValue.find( GUI::Hash( name ) ) == ctx->SliderInfo.ShouldChangeValue.end( ) ) {
		ctx->SliderInfo.ShouldChangeValue.insert( { GUI::Hash( name ), { 0.f, false } } );
	}

	if( !( g_csgo.m_globals->m_curtime - ctx->SliderInfo.LastChangeTime.at( GUI::Hash( name ) ) >= 1.5f ) ) {
		ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
		ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

		ctx->SliderInfo.ValueAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ) );

		if( ctx->SliderInfo.ValueAnimation.at( GUI::Hash( name ) ) >= 0.999998 ) {
			ctx->SliderInfo.ValueAnimation.at( GUI::Hash( name ) ) = 1.f;
		}
	}
	else {
		ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ) -= ( 1.0f / 0.3f ) * g_csgo.m_globals->m_frametime;
		ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

		ctx->SliderInfo.ValueAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( ctx->SliderInfo.ValueTimer.at( GUI::Hash( name ) ) );
	}

	ctx->SliderInfo.PreviewAnimation.at( GUI::Hash( name ) ) = 1.0f - ctx->SliderInfo.ValueAnimation.at( GUI::Hash( name ) );

	D3D::Text( DrawPos - 2, GUI::SplitStr( name, '#' )[ 0 ].data( ), Color( 255, 255, 255 ).OverrideAlpha( 255 * ctx->animation ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );

	char Buffer[ 32 ];
	sprintf_s( Buffer, display.data( ), g_cfg[ var_name ].get<float>( ) );

	D3D::Text( DrawPos + vec2_t( DrawSize.x - D3D::GetTextSize( Buffer, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).x, -2 ), Buffer,
		Color( 255, 255, 255 ).OverrideAlpha( 255 * ( ( 1.0f - ctx->SliderInfo.PreviewAnimation.at( GUI::Hash( name ) ) ) * ctx->animation ) ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );

	if( InputHelper::Hovered( DrawPos + vec2_t( 0, D3D::GetTextSize( name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y ),
		DrawSize ) && InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && ctx->FocusedID == 0 || ctx->FocusedID == GUI::Hash( name ) ) {

		float PreviewValue = std::clamp( GUI::MapNumber( std::clamp( ( int )vec2_t( g_input.m_mouse - DrawPos ).x, 0, ( int )DrawSize.x ),
			0, DrawSize.x, min, max ), min, max );

		char PreviewBuffer[ 32 ];
		sprintf_s( PreviewBuffer, display.data( ), std::clamp( GUI::MapNumber( std::clamp( ( int )vec2_t( g_input.m_mouse - DrawPos ).x, 0, ( int )DrawSize.x ),
			0, DrawSize.x, min, max ), min, max ) );

		if( ctx->SliderInfo.PreviewAnimation.at( GUI::Hash( name ) ) > 0.f ) {
			D3D::Text( DrawPos + vec2_t( DrawSize.x - D3D::GetTextSize( PreviewBuffer, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).x, -2 ), PreviewBuffer,
				Color( 255, 255, 255 ).OverrideAlpha( 100 * ( ( ctx->SliderInfo.PreviewAnimation.at( GUI::Hash( name ) ) ) * ctx->animation ) ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );
		}

		D3D::RectFilled( DrawPos + vec2_t( 0, D3D::GetTextSize( name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y ), vec2_t( GUI::MapNumber( PreviewValue, min, max, 0, DrawSize.x ), DrawSize.y ),
			g_cfg[ XOR( "menu_color" ) ].get_color( ).OverrideAlpha( 25 * ctx->animation ) );
	}
	else {
		D3D::Text( DrawPos + vec2_t( DrawSize.x - D3D::GetTextSize( Buffer, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).x, -2 ), Buffer,
			Color( 255, 255, 255 ).OverrideAlpha( 100 * ctx->animation ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), 0 );
	}

	DrawPos.y += D3D::GetTextSize( name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y;

	bool Hovered = InputHelper::Hovered( DrawPos, DrawSize ) && InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize );

	D3D::RectFilled( DrawPos, vec2_t( GUI::MapNumber( g_cfg[ var_name ].get<float>( ), min, max, 0, DrawSize.x ), DrawSize.y ), g_cfg[ XOR( "menu_color" ) ].get_color( ).OverrideAlpha( 255 * ctx->animation, true ) );

	D3D::Rect( DrawPos, DrawSize, Color::Palette_t::MenuColors_t::Outlines( ).OverrideAlpha( 255 * ctx->animation, true ) );

	if( ctx->FocusedID == 0 ) {
		if( Hovered && InputHelper::Pressed( VK_LBUTTON ) ) {
			ctx->FocusedID = GUI::Hash( name );
		}
	}
	else if( ctx->FocusedID == GUI::Hash( name ) ) {
		ctx->SliderInfo.LastChangeTime.at( GUI::Hash( name ) ) = g_csgo.m_globals->m_curtime;

		if( InputHelper::Down( VK_LBUTTON ) ) {
			float FinalValue = std::clamp( GUI::MapNumber( std::clamp( ( int )vec2_t( g_input.m_mouse - DrawPos ).x, 0, ( int )DrawSize.x ), 0, DrawSize.x, min, max ), min, max );

			ctx->SliderInfo.ShouldChangeValue.at( GUI::Hash( name ) ) = std::make_pair( FinalValue, true );
		}
		else {
			ctx->FocusedID = 0;
		}
	}

	if( ctx->SliderInfo.ShouldChangeValue.at( GUI::Hash( name ) ).second ) {
		g_cfg[ var_name ].set<float>( GUI::Approach( ctx->SliderInfo.PreviousAmount.at( GUI::Hash( name ) ), ctx->SliderInfo.ShouldChangeValue.at( GUI::Hash( name ) ).first, ( 1.0f / 0.03f ) * g_csgo.m_globals->m_frametime ) );

		if( ctx->SliderInfo.PreviousAmount.at( GUI::Hash( name ) ) == ctx->SliderInfo.ShouldChangeValue.at( GUI::Hash( name ) ).first ) {
			ctx->SliderInfo.ShouldChangeValue.at( GUI::Hash( name ) ).second = false;
		}
	}

	ctx->SliderInfo.PreviousAmount.at( GUI::Hash( name ) ) = g_cfg[ var_name ].get<float>( );

	if( g_cfg[ var_name ].get<float>( ) < min ) {
		g_cfg[ var_name ].set<float>( min );
	}

	if( g_cfg[ var_name ].get<float>( ) > max ) {
		g_cfg[ var_name ].set<float>( max );
	}

	PushCursorPos( CursorPos + vec2_t( 0, DrawSize.y + D3D::GetTextSize( name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y + GUI::ObjectPadding( ) ) );
	return ctx->FocusedID == GUI::Hash( name );
}