#include "../gui.h"

std::string GetKeyName( const int VirtualKey ) {
	auto Code = MapVirtualKeyA( VirtualKey, MAPVK_VK_TO_VSC );

	int Result;
	char Buffer[ 128 ];

	switch( VirtualKey ) {
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_RCONTROL: case VK_RMENU:
	case VK_LWIN: case VK_RWIN: case VK_APPS:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		Code |= KF_EXTENDED;
	default:
		Result = GetKeyNameTextA( Code << 16, Buffer, 128 );
	}

	if( Result == 0 ) {
		switch( VirtualKey ) {
		case VK_XBUTTON1:
			return XOR( "[MOUSE 4]" );
		case VK_XBUTTON2:
			return XOR( "[MOUSE 5]" );
		case VK_LBUTTON:
			return XOR( "[MOUSE 1]" );
		case VK_MBUTTON:
			return XOR( "[MOUSE 3]" );
		case VK_RBUTTON:
			return XOR( "[MOUSE 2]" );
		default:
			return XOR( "[_]" );
		}
	}

	auto transformer = std::string( XOR( "[" ) ) + std::string( Buffer ) + std::string( XOR( "]" ) );
	std::transform( transformer.begin( ), transformer.end( ), transformer.begin( ), ::toupper );
	return transformer;
}

std::map<size_t, float> ContextAnimation;
std::map<size_t, float> ContextTimer;

void GUI::Controls::Hotkey( const std::string& name, const std::string& var_name, bool type_selection ) {
	vec2_t CursorPos = GetLastCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;
	vec2_t DrawSize = vec2_t( D3D::GetTextSize( GetKeyName( g_cfg[ var_name ].get<int>( ) ), D3D::D3D9Fonts.at( D3D::EFonts::PIXEL_FONT ) ).x, 8 );
	vec2_t DotSize = vec2_t( D3D::GetTextSize( XOR( "..." ), D3D::D3D9Fonts.at( D3D::EFonts::PIXEL_FONT ) ).x, 8 );

	bool Hovered = InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && InputHelper::Hovered( DrawPos + vec2_t( ( ctx->ParentSize.x - 42 ) - DrawSize.x, 3 ), DrawSize );

	D3D::Text( ( DrawPos + vec2_t( ( ctx->ParentSize.x - 42 ) - ( ctx->FocusedID == GUI::Hash( name ) ? DotSize.x : DrawSize.x ), 3 ) ) + 1,
		ctx->FocusedID == GUI::Hash( name ) ? XOR( "..." ) : GetKeyName( g_cfg[ var_name ].get<int>( ) ),
		Color( 0, 0, 0, 75 * ctx->animation ), D3D::D3D9Fonts.at( D3D::EFonts::PIXEL_FONT ), 0 );

	D3D::Text( DrawPos + vec2_t( ( ctx->ParentSize.x - 42 ) - ( ctx->FocusedID == GUI::Hash( name ) ? DotSize.x : DrawSize.x ), 3 ),
		ctx->FocusedID == GUI::Hash( name ) ? XOR( "..." ) : GetKeyName( g_cfg[ var_name ].get<int>( ) ),
		ctx->FocusedID == GUI::Hash( name ) ? g_cfg[ XOR( "menu_color" ) ].get_color( ) : Color( 180, 180, 180, 255 * ctx->animation ), D3D::D3D9Fonts.at( D3D::EFonts::PIXEL_FONT ), 0 );

	if( ContextAnimation.find( GUI::Hash( name ) ) == ContextAnimation.end( ) ) {
		ContextAnimation.insert( { GUI::Hash( name ), 0.f } );
		ContextTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->FocusedID == GUI::Hash( name + XOR( "_type" ) ) ) {
		if( ctx->DropdownInfo.OutAnimation ) {
			ContextTimer.at( GUI::Hash( name ) ) -= ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			ContextTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ContextTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			ContextAnimation.at( GUI::Hash( name ) ) = Easing::InQuint( ContextTimer.at( GUI::Hash( name ) ) );

			if( ContextAnimation.at( GUI::Hash( name ) ) <= 0.01f ) {
				ctx->FocusedID = 0;
				ctx->DropdownInfo.HashedID = 0;
				ctx->DropdownInfo.OutAnimation = false;

				ContextTimer.at( GUI::Hash( name ) ) = ContextAnimation.at( GUI::Hash( name ) ) = 0.f;
			}
		}
		else {
			ContextTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
			ContextTimer.at( GUI::Hash( name ) ) = std::clamp<float>( ContextTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			ContextAnimation.at( GUI::Hash( name ) ) = Easing::OutQuint( ContextTimer.at( GUI::Hash( name ) ) );

			if( ContextAnimation.at( GUI::Hash( name ) ) >= 0.9f ) {
				ContextAnimation.at( GUI::Hash( name ) ) = 1.f;
			}
		}
	}

	if( ctx->FocusedID == 0 ) {
		if( Hovered && InputHelper::Pressed( VK_LBUTTON ) ) {
			ctx->FocusedID = GUI::Hash( name );
		}

		if( type_selection ) {
			if( Hovered && InputHelper::Pressed( VK_RBUTTON ) ) {
				ctx->FocusedID = GUI::Hash( name + XOR( "_type" ) );
			}
		}
	}
	else if( ctx->FocusedID == GUI::Hash( name + XOR( "_type" ) ) ) {
		ctx->DropdownInfo.Animation = ContextAnimation.at( GUI::Hash( name ) );

		ctx->DropdownInfo.Elements = { XOR( "Toggle" ), XOR( "On key" ), XOR( "Off key" ), XOR( "Always" ) };
		ctx->DropdownInfo.Option = var_name + XOR( "_type" );
		ctx->DropdownInfo.Size = 90;
		ctx->DropdownInfo.Pos = DrawPos + vec2_t( ( ctx->ParentSize.x - 42 ) - DrawSize.x, 10 );
		ctx->DropdownInfo.HashedID = GUI::Hash( name + XOR( "_type" ) );
	}
	else if( ctx->FocusedID == GUI::Hash( name ) ) {
		if( !Hovered && InputHelper::Down( VK_LBUTTON ) ) {
			ctx->FocusedID = 0;
		}

		for( int i = 0; i < 255; i++ ) {
			if( InputHelper::Released( i ) ) {
				if( i != VK_LBUTTON ) {
					ctx->FocusedID = 0;
					g_cfg[ var_name ].set<int>( i );

					if( i == VK_ESCAPE ) {
						g_cfg[ var_name ].set<int>( 0 );
					}
				}
			}
		}
	}
}
