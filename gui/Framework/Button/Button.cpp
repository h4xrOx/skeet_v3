#include "../gui.h"
#include <algorithm>

std::map<size_t, std::vector<std::pair<vec2_t, float>>> Ripple;

void GUI::Controls::Button( const std::string& name, std::function< void( ) > callback, bool use_unique_id ) {
	vec2_t CursorPos = PopCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;
	vec2_t DrawSize = vec2_t( ctx->ParentSize.x - 40 > 300 ? 300 : ctx->ParentSize.x - 40, 23 );

	std::string split_str = GUI::SplitStr( name, '#' )[ 0 ].data( );
	std::string unique_id = use_unique_id ? std::string( name ).erase( 0, split_str.length( ) ) : name;

	bool Hovered = InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && InputHelper::Hovered( DrawPos, DrawSize );

	if( Ripple.find( GUI::Hash( unique_id ) ) == Ripple.end( ) ) {
		Ripple.insert( { GUI::Hash( unique_id ), { } } );
	}

	if( Hovered && ctx->FocusedID == 0 ) {
		D3D::RectFilled( DrawPos, DrawSize, Color::Palette_t::MenuColors_t::ContainerHovered( ).OverrideAlpha( 255 * ctx->animation ) );
	}

	D3D::SetLimit( { DrawPos, DrawSize - 1 } );
	for( int r = 0; r < Ripple.at( GUI::Hash( unique_id ) ).size( ); ++r ) {
		if( Ripple.at( GUI::Hash( unique_id ) ).empty( ) ) {
			continue;
		}

		// ripples should not be eased as they are constant
		Ripple.at( GUI::Hash( unique_id ) ).at( r ).second -= ( 1.0f / 0.6f ) * g_csgo.m_globals->m_frametime;

		if( Ripple.at( GUI::Hash( unique_id ) ).at( r ).second <= 0.f ) {
			Ripple.at( GUI::Hash( unique_id ) ).at( r ).second = 0.f;
			continue;
		}

		if( Ripple.at( GUI::Hash( unique_id ) ).at( r ).first > DrawPos &&
			Ripple.at( GUI::Hash( unique_id ) ).at( r ).first < DrawPos + DrawSize ) {
			if( Ripple.at( GUI::Hash( unique_id ) ).at( r ).second != 0.f ) {

				D3D::CircleFilled( Ripple.at( GUI::Hash( unique_id ) ).at( r ).first,
					120 * ( 1.0f - Ripple.at( GUI::Hash( unique_id ) ).at( r ).second ),
					Color( 82, 87, 96, int( 255 * ( ( Ripple.at( GUI::Hash( unique_id ) ).at( r ).second ) * ctx->animation ) ) ) );

			}
		}
	}
	D3D::ResetLimit( );

	D3D::Rect( DrawPos, DrawSize, Color::Palette_t::MenuColors_t::Outlines( ).OverrideAlpha( 255 * ctx->animation ) );

	// render this with the split str!!!!
	vec2_t TextSize( D3D::GetTextSize( split_str, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ) );
	D3D::Text( ( DrawPos + ( DrawSize / 2 ) ) - ( TextSize / 2 ), split_str, Color( 255, 255, 255, 255 * ctx->animation ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), D3D::EFontFlags::NONE );

	if( ctx->FocusedID == 0 ) {
		if( Hovered && InputHelper::Pressed( VK_LBUTTON ) ) {
			ctx->FocusedID = GUI::Hash( unique_id );
		}
	}
	else if( ctx->FocusedID == GUI::Hash( unique_id ) ) {
		if( !InputHelper::Down( VK_LBUTTON ) ) {
			ctx->FocusedID = 0;

			if( Hovered ) {
				callback( );

				Ripple.at( GUI::Hash( unique_id ) ).push_back( { g_input.m_mouse, 1.0f } );
			}
		}
	}

	PushCursorPos( CursorPos + vec2_t( 0, DrawSize.y + GUI::ObjectPadding( ) ) );
}
