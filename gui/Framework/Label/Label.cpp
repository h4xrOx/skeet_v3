#include "../gui.h"

bool GUI::Controls::Label( const std::string &name ) {
	vec2_t CursorPos = PopCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;

	D3D::Text( DrawPos - 2, SplitStr( name.data( ), '#' )[ 0 ].data( ), Color( 255, 255, 255, 255 * ctx->animation ),
		D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), D3D::EFontFlags::NONE );

	PushCursorPos( CursorPos + vec2_t( 0, D3D::GetTextSize( name, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ).y + GUI::ObjectPadding( ) ) );
	
	return ( name.size( ) > 0 );
}
