#include "../gui.h"

#pragma warning(disable : 4018)

std::map<int, float> HoverAnimation;
std::map<int, float> HoverTimer;

std::map<int, float> SelectAnimation;
std::map<int, float> SelectTimer;

bool TitleBarHovered;

std::string FormName;

bool GUI::Form::BeginWindow( std::string name ) {

	if( InputHelper::Pressed( VK_INSERT ) ) {
		if( ctx->animation <= 1.0f ) {
			if( ctx->FocusedID != 0 )
				ctx->FocusedID = 0;

			if( ctx->DropdownInfo.HashedID != 0 )
				ctx->DropdownInfo.HashedID = ctx->DropdownInfo.Open = 0;

			if( ctx->ColorPickerInfo.HashedID != 0 )
				ctx->ColorPickerInfo.HashedID = 0;

			if( ctx->MultiDropdownInfo.HashedID != 0 )
				ctx->MultiDropdownInfo.HashedID = ctx->MultiDropdownInfo.Open = 0;

			if( ctx->DropdownInfo.ArrowAnimation.size( ) )
				ctx->DropdownInfo.ArrowAnimation.clear( );

			if( ctx->DropdownInfo.ArrowTimer.size( ) )
				ctx->DropdownInfo.ArrowTimer.clear( );

			if( ctx->MultiDropdownInfo._ArrowAnimation.size( ) )
				ctx->MultiDropdownInfo._ArrowAnimation.clear( );

			if( ctx->MultiDropdownInfo._ArrowTimer.size( ) )
				ctx->MultiDropdownInfo._ArrowTimer.clear( );

			if( ctx->dragging )
				ctx->dragging = false;

			if( ctx->typing )
				ctx->typing = false;
		}

		ctx->open = !ctx->open;
	}

	// i'll work on this later, one thing that's a todo is
	// to fix the abrupt and sudden animation change when spamming insert
	if( ctx->open ) {
		ctx->timer += ( 1.0f / 0.3f ) * g_csgo.m_globals->m_frametime;
		ctx->timer = std::clamp<float>( ctx->timer, 0.f, 1.f );

		ctx->animation = Easing::OutQuint( ctx->timer );

		if( ctx->animation >= 0.999998 ) {
			ctx->animation = 1.f;
		}
	}
	else {
		ctx->timer -= ( 1.0f / 0.3f ) * g_csgo.m_globals->m_frametime;
		ctx->timer = std::clamp<float>( ctx->timer, 0.f, 1.f );

		ctx->animation = Easing::InQuint( ctx->timer );

		if( ctx->animation <= 0.001f ) {
			ctx->animation = 0.f;
		}
	}

	if( ctx->animation <= 0.0f ) {
		return false;
	}

	TitleBarHovered = InputHelper::Hovered( ctx->pos, vec2_t( ctx->size.x, 30 ) );
	if( ctx->FocusedID == 0 ) {
		if( !ctx->dragging && InputHelper::Pressed( VK_LBUTTON ) && TitleBarHovered )
			ctx->dragging = true;
		else if( ctx->dragging && InputHelper::Down( VK_LBUTTON ) )
			ctx->pos -= g_input.m_delta;
		else if( ctx->dragging && !InputHelper::Down( VK_LBUTTON ) )
			ctx->dragging = false;
	}

	if( ctx->animation >= 0.6969f ) {
		D3D::RectFilled( ctx->pos - 1, ctx->size + 2, Color( 0, 0, 0 ).OverrideAlpha( 220 * ctx->animation ) );

		D3D::SetBlur( 2.0f );
		D3D::RectFilled( ctx->pos - 3, ctx->size + 6, Color( 0, 0, 0 ).OverrideAlpha( 255 * ctx->animation, true ) );
		D3D::SetBlur( 0.f );
	}

	// form (fuck the transparency bro)
	D3D::RectFilled( ctx->pos, ctx->size, Color::Palette_t::MenuColors_t::Form( ).OverrideAlpha( 255 * ctx->animation, true ) );

	// swap cursor pos stack
	std::stack< vec2_t >( ).swap( ctx->CursorPosStack );

	// at what (x, y) relative to the menu's initial cursor position (0,0) we should draw the menu elements
	GUI::PushCursorPos( vec2_t( 16, 46 ) );

	return true;
}

void GUI::Form::EndWindow( ) {
	// dropshadow under titlebar
	D3D::RectFilledLinearGradient( ctx->pos + vec2_t( 0, 30 ), vec2_t( ctx->size.x, 5 ), Color( 35, 40, 49 ).OverrideAlpha( 255 * ctx->animation, true ), Color::Palette_t::MenuColors_t::Form( ).OverrideAlpha( 255 * ctx->animation, true ) );

	// titlebar texture
	D3D::DrawTexture( GUI::ctx->pos, vec2_t( GUI::ctx->size.x, 30 ), D3D::TITLEBAR_TEXTURE, ctx->animation );

	// "pandora" text
	D3D::Text( ctx->pos + vec2_t( 6, 3 ), XOR( "Pandora Beta" ), Color( 220, 220, 220 ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts[ D3D::TAB_FONT ], 0 );

	// tabs
	if( !ctx->Tabs.empty( ) ) {
		constexpr int MaxTabs{ 8 };

		std::array<vec2_t, MaxTabs> TabPos;
		std::array<vec2_t, MaxTabs> TabSize;

		for( int i = 0; i < ctx->Tabs.size( ); ++i ) {
			TabPos.at( i ) = ( i == 0 ) ? ctx->pos + vec2_t( ctx->size.x / 2, 3 ) : TabPos.at( i - 1 ) + vec2_t( TabSize.at( i - 1 ).x + 25, 0 );
			TabSize.at( i ) = D3D::GetTextSize( ctx->Tabs.at( i ).second, D3D::D3D9Fonts[ D3D::TAB_FONT ] );

			D3D::Text( TabPos.at( i ), ctx->Tabs.at( i ).second, Color( 220, 220, 220 ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts[ D3D::TAB_FONT ], 0 );

			bool Hovered = InputHelper::Hovered( TabPos.at( i ) - vec2_t( 0, 3 ), vec2_t( TabSize.at( i ).x, 30 ) );

			if( HoverAnimation.find( i ) == HoverAnimation.end( ) ) {
				HoverAnimation.insert( { i, 0.f } );
				HoverTimer.insert( { i, 0.f } );

				SelectAnimation.insert( { i, 0.f } );
				SelectTimer.insert( { i, 0.f } );
			}

			if( Hovered ) {
				HoverTimer.at( i ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
				HoverTimer.at( i ) = std::clamp<float>( HoverTimer.at( i ), 0.f, 1.f );

				HoverAnimation.at( i ) = Easing::OutCirc( HoverTimer.at( i ) );

				if( HoverAnimation.at( i ) >= 0.999998 ) {
					HoverAnimation.at( i ) = 1.f;
				}
			}
			else {
				// do this so if we were previously hovering over a tab
				// and we selected it, the animation won't go back, and instead
				// the purple animation will "reach" the hover animation
				if( i != ctx->ActiveTab ) {
					HoverTimer.at( i ) -= ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
					HoverTimer.at( i ) = std::clamp<float>( HoverTimer.at( i ), 0.f, 1.f );

					HoverAnimation.at( i ) = Easing::OutCirc( HoverTimer.at( i ) );

					if( HoverAnimation.at( i ) <= 0.05f ) {
						HoverAnimation.at( i ) = 0.f;
					}
				}
			}

			if( i == ctx->ActiveTab ) {
				SelectTimer.at( i ) += ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
				SelectTimer.at( i ) = std::clamp<float>( SelectTimer.at( i ), 0.f, 1.f );

				SelectAnimation.at( i ) = Easing::OutCirc( SelectTimer.at( i ) );

				if( SelectAnimation.at( i ) >= 0.999998 ) {
					SelectAnimation.at( i ) = 1.f;
				}
			}
			else {
				SelectTimer.at( i ) -= ( 1.0f / 0.2f ) * g_csgo.m_globals->m_frametime;
				SelectTimer.at( i ) = std::clamp<float>( SelectTimer.at( i ), 0.f, 1.f );

				SelectAnimation.at( i ) = Easing::OutCirc( SelectTimer.at( i ) );

				if( SelectAnimation.at( i ) <= 0.05f ) {
					SelectAnimation.at( i ) = 0.f;
				}
			}

			D3D::RectFilled( TabPos.at( i ) + vec2_t( 0, 27 ), vec2_t( TabSize.at( i ).x * HoverAnimation.at( i ), 2 ),
				Color::Palette_t::MenuColors_t::Outlines( ).OverrideAlpha( 255 * ctx->animation, true ) );

			D3D::RectFilled( TabPos.at( i ) + vec2_t( 0, 27 ), vec2_t( TabSize.at( i ).x * SelectAnimation.at( i ), 2 ),
				g_cfg[ XOR( "menu_color" ) ].get_color( ).OverrideAlpha( 255 * ctx->animation, true ) );

			if( ctx->FocusedID == 0 ) {
				if( Hovered && InputHelper::Pressed( VK_LBUTTON ) ) {
					ctx->FocusedID = GUI::Hash( std::string( XOR( "SelectTab#" ) ).append( std::to_string( i ) ) );
				}
			}
			else if( ctx->FocusedID == GUI::Hash( std::string( XOR( "SelectTab#" ) ).append( std::to_string( i ) ) ) ) {
				if( !InputHelper::Down( VK_LBUTTON ) ) {
					ctx->ActiveTab = i;
					ctx->FocusedID = 0;
				}
			}
		}
	}

	// sub-tabs
	if( !ctx->SubTabs.empty( ) && !ctx->SubTabs.at( ctx->ActiveTab ).empty( ) ) {
		auto& SubTab = ctx->SubTabs.at( ctx->ActiveTab );

		std::array<vec2_t, 15> SubTabPos;
		std::array<vec2_t, 15> SubTabSize;

		for( int i = 0; i < SubTab.size( ); ++i ) {
			SubTabSize.at( i ) = vec2_t( int( ( ctx->size.x - 32 ) / SubTab.size( ) ), 25 );
			SubTabPos.at( i ) = ( i == 0 ) ? ctx->pos + vec2_t( 16, ( int )ctx->size.y - 36 ) : SubTabPos.at( i - 1 ) + vec2_t( SubTabSize.at( i - 1 ).x, 0 );

			if( InputHelper::Hovered( SubTabPos.at( i ), SubTabSize.at( i ) ) && ctx->FocusedID == 0 ) {
				D3D::RectFilled( SubTabPos.at( i ), SubTabSize.at( i ), Color::Palette_t::MenuColors_t::ContainerHovered( ).OverrideAlpha( 255 * ctx->animation, true ) );

				if( InputHelper::Pressed( VK_LBUTTON ) ) {
					ctx->FocusedID = GUI::Hash( XOR( "SubTabSelect" ) );
				}
			}
			else {
				D3D::RectFilled( SubTabPos.at( i ), SubTabSize.at( i ), Color::Palette_t::MenuColors_t::Container( ).OverrideAlpha( 255 * ctx->animation, true ) );
			}

			if( ctx->FocusedID == GUI::Hash( XOR( "SubTabSelect" ) ) ) {
				if( !InputHelper::Down( VK_LBUTTON ) ) {
					ctx->ActiveSubTab.at( ctx->ActiveTab ) = i;

					ctx->FocusedID = 0;
				}
			}

			vec2_t TextSize( D3D::GetTextSize( SubTab.at( i ).second, D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ) ) );
			D3D::Text( ( SubTabPos.at( i ) + ( SubTabSize.at( i ) / 2 ) ) - ( TextSize / 2 ), SubTab.at( i ).second,
				( ctx->ActiveSubTab.at( ctx->ActiveTab ) == i ? g_cfg[ XOR( "menu_color" ) ].get_color( ) : Color( 255, 255, 255 ) ).OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), D3D::EFontFlags::NONE );

			D3D::Rect( SubTabPos.at( i ), SubTabSize.at( i ), Color( 27, 30, 38 ).OverrideAlpha( 255 * ctx->animation ) );
		}
	}

	if( ctx->DropdownInfo.HashedID != 0 ) {
		vec2_t DrawPos = ctx->DropdownInfo.Pos;
		D3D::RectFilled( DrawPos, vec2_t( ctx->DropdownInfo.Size, ( 20 * ctx->DropdownInfo.Elements.size( ) ) * ctx->DropdownInfo.Animation ) - 1, Color::Palette_t::MenuColors_t::Container( ) );

		D3D::Rect( DrawPos, vec2_t( ctx->DropdownInfo.Size, ( 20 * ctx->DropdownInfo.Elements.size( ) ) * ctx->DropdownInfo.Animation ), g_cfg[ XOR( "menu_color" ) ].get_color( ) );

		D3D::SetLimit( std::pair<vec2_t, vec2_t>( DrawPos, vec2_t( ctx->DropdownInfo.Size, ( 20 * ctx->DropdownInfo.Elements.size( ) ) * ctx->DropdownInfo.Animation ) - 1 ) );
		for( int i = 0; i < ctx->DropdownInfo.Elements.size( ); i++ ) {
			vec2_t ElementPos = DrawPos + vec2_t( 0, 20 * i );
			vec2_t ElementSize = vec2_t( ctx->DropdownInfo.Size, 20 );

			if( InputHelper::Hovered( ElementPos, ElementSize ) ) {
				if( InputHelper::Pressed( VK_LBUTTON ) ) {
					ctx->DropdownInfo.OutAnimation = true;
					g_cfg[ ctx->DropdownInfo.Option ].set<int>( i );
				}

				D3D::RectFilled( ElementPos, ElementSize - vec2_t( 1, 0 ), Color::Palette_t::MenuColors_t::ContainerHovered( ) );
			}


			D3D::Text( ElementPos + vec2_t( 4, 2 ), ctx->DropdownInfo.Elements[ i ].data( ),
				g_cfg[ ctx->DropdownInfo.Option ].get<int>( ) != i ? Color( 255, 255, 255 ) : g_cfg[ XOR( "menu_color" ) ].get_color( ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), D3D::EFontFlags::NONE );

		}
		D3D::ResetLimit( );

		if( InputHelper::Pressed( VK_LBUTTON ) ) {
			ctx->DropdownInfo.OutAnimation = true;
		}
	}

	if( ctx->MultiDropdownInfo.HashedID != 0 ) {
		vec2_t DrawPos = ctx->MultiDropdownInfo.Pos;
		D3D::RectFilled( DrawPos, vec2_t( ctx->MultiDropdownInfo.Size, ( 20 * ctx->MultiDropdownInfo.Elements.size( ) ) * ctx->MultiDropdownInfo.Animation ) - 1,
			Color::Palette_t::MenuColors_t::Container( ) );

		D3D::Rect( DrawPos, vec2_t( ctx->MultiDropdownInfo.Size, ( 20 * ctx->MultiDropdownInfo.Elements.size( ) ) * ctx->MultiDropdownInfo.Animation ),
			g_cfg[ XOR( "menu_color" ) ].get_color( ) );

		D3D::SetLimit( std::pair<vec2_t, vec2_t>( DrawPos, vec2_t( ctx->MultiDropdownInfo.Size, ( 20 * ctx->MultiDropdownInfo.Elements.size( ) ) * ctx->MultiDropdownInfo.Animation ) - 1 ) );
		for( int i = 0; i < ctx->MultiDropdownInfo.Elements.size( ); i++ ) {
			vec2_t ElementPos = DrawPos + vec2_t( 0, 20 * i );
			vec2_t ElementSize = vec2_t( ctx->MultiDropdownInfo.Size, 20 );

			int val = pow( 2, ( i + 2 ) );

			if( InputHelper::Hovered( ElementPos, ElementSize ) ) {
				if( InputHelper::Pressed( VK_LBUTTON ) ) {
					bool v1 = g_cfg[ ctx->MultiDropdownInfo.Elements.at( i ).value ].get<bool>( );

					g_cfg[ ctx->MultiDropdownInfo.Elements.at( i ).value ].set<bool>( !v1 );
				}

				D3D::RectFilled( ElementPos, ElementSize - vec2_t( 1, 0 ), Color::Palette_t::MenuColors_t::ContainerHovered( ) );
			}


			D3D::Text( ElementPos + vec2_t( 4, 2 ), ctx->MultiDropdownInfo.Elements.at( i ).name,
				!g_cfg[ ctx->MultiDropdownInfo.Elements.at( i ).value ].get<bool>( ) ? Color( 255, 255, 255 ) : g_cfg[ XOR( "menu_color" ) ].get_color( ), D3D::D3D9Fonts.at( D3D::EFonts::MAIN_FONT ), D3D::EFontFlags::NONE );

		}
		D3D::ResetLimit( );

		if( InputHelper::Pressed( VK_LBUTTON )
			&& !InputHelper::Hovered( DrawPos,
				vec2_t( ctx->MultiDropdownInfo.Size, ( 20 * ctx->MultiDropdownInfo.Elements.size( ) ) * ctx->MultiDropdownInfo.Animation ) ) ) {

			ctx->MultiDropdownInfo.OutAnimation = true;
		}

	}

	static float ColorPickerHue = -1.f;
	static std::map<size_t, float> ColorPickerAlpha;
	if( ctx->ColorPickerInfo.HashedID != 0 ) {
		if( ColorPickerAlpha.find( ctx->ColorPickerInfo.HashedID ) == ColorPickerAlpha.end( ) ) {
			ColorPickerAlpha.insert( { ctx->ColorPickerInfo.HashedID, 0.f } );
		}

		vec2_t DrawPos = ctx->ColorPickerInfo.Pos;

		Color::Hsv_t v1 = Color::RGBtoHSV( g_cfg[ ctx->ColorPickerInfo.Value ].get_color( ) );

		float NewHue;

		if( ColorPickerHue == -1.f ) {
			NewHue = v1.Hue;
			ColorPickerHue = NewHue;
		}
		else {
			NewHue = ColorPickerHue;
		}

		float NewSaturation = v1.Saturation;
		float NewValue = v1.Value;
		ColorPickerAlpha.at( ctx->ColorPickerInfo.HashedID ) = ( g_cfg[ ctx->ColorPickerInfo.Value ].get_color( ) ).a( );

		float MaxHeight = ( ctx->ColorPickerInfo.Size - 25 );
		vec2_t DrawSize = vec2_t( ctx->ColorPickerInfo.Size, MaxHeight );
		vec2_t AnimatedSize = vec2_t( ctx->ColorPickerInfo.Size * ctx->ColorPickerInfo.Animation, DrawSize.y * ctx->ColorPickerInfo.Animation );

		D3D::RectFilled( DrawPos, AnimatedSize - 1, Color::Palette_t::MenuColors_t::Container( ) );
		D3D::Rect( DrawPos, AnimatedSize, g_cfg[ XOR( "menu_color" ) ].get_color( ) );

		D3D::SetLimit( std::pair<vec2_t, vec2_t>( DrawPos, AnimatedSize - 1 ) );
		{
			vec2_t ColorDrawPos( DrawPos + 10 );
			vec2_t ColorDrawSize( DrawSize - 40 );

			vec2_t AlphaDrawPos( ColorDrawPos + vec2_t( 0, ColorDrawSize.y + 4 ) );
			vec2_t AlphaDrawSize = vec2_t( ColorDrawSize.x, 16 );

			vec2_t HueDrawPos( ColorDrawPos + vec2_t( ColorDrawSize.x + 4, -1 ) );
			vec2_t HueDrawSize = vec2_t( 16, ColorDrawSize.y );

			static vec2_t ColorPreviewPos;

			bool HoveredMain = InputHelper::Hovered( DrawPos, DrawSize );
			bool HoveredPicker = InputHelper::Hovered( DrawPos, DrawSize - 30 );
			bool HoveredAlpha = InputHelper::Hovered( AlphaDrawPos, AlphaDrawSize );
			bool HoveredHue = InputHelper::Hovered( HueDrawPos, HueDrawSize );

			// alpha bar
			if( !ctx->ColorPickerInfo.PickingAlpha && !ctx->ColorPickerInfo.PickingHue && !ctx->ColorPickerInfo.PickingColor ) {
				if( HoveredAlpha && InputHelper::Pressed( VK_LBUTTON ) ) {
					ctx->ColorPickerInfo.PickingAlpha = true;
				}
			}
			else if( ctx->ColorPickerInfo.PickingAlpha ) {
				if( InputHelper::Down( VK_LBUTTON ) ) {
					float FinalValue = std::clamp<float>( GUI::MapNumber( std::clamp<float>(
						vec2_t( g_input.m_mouse - AlphaDrawPos ).x, 0, AlphaDrawSize.x ),
						0, AlphaDrawSize.x, 0, 255 ), 0, 255 );

					ColorPickerAlpha.at( ctx->ColorPickerInfo.HashedID ) = FinalValue;
				}
				else {
					ctx->ColorPickerInfo.PickingAlpha = false;
				}
			}

			D3D::RectFilled( AlphaDrawPos, AlphaDrawSize, ( g_cfg[ ctx->ColorPickerInfo.Value ].get_color( ) ) );
			D3D::Rect( AlphaDrawPos, AlphaDrawSize, Color::Palette_t::MenuColors_t::Outlines( ) );

			// hue bar
			if( !ctx->ColorPickerInfo.PickingHue && !ctx->ColorPickerInfo.PickingAlpha && !ctx->ColorPickerInfo.PickingColor ) {
				if( HoveredHue && InputHelper::Pressed( VK_LBUTTON ) ) {
					ctx->ColorPickerInfo.PickingHue = true;
				}
			}
			else if( ctx->ColorPickerInfo.PickingHue ) {
				if( InputHelper::Down( VK_LBUTTON ) ) {
					NewHue = ( ( g_input.m_mouse.y - ( HueDrawPos.y ) ) / HueDrawSize.y ) * 360.f;

					NewHue = std::clamp<float>( NewHue, 0.f, 359.f );
					ColorPickerHue = NewHue;
				}
				else {
					ctx->ColorPickerInfo.PickingHue = false;
				}
			}

			for( int i = 0; i < HueDrawSize.y; i++ ) {
				float Hue = ( ( float )i / HueDrawSize.y ) * 360.f;
				Color HueColor = Color::HSVtoRGB( Hue, 1, 1 );

				D3D::Line( HueDrawPos + vec2_t( 0, i + 1 ), HueDrawPos + vec2_t( HueDrawSize.x, i ), HueColor );
			}
			D3D::Rect( HueDrawPos + vec2_t( 0, 1 ), HueDrawSize + vec2_t( 1, 0 ), Color::Palette_t::MenuColors_t::Outlines( ) );

			// main picker
			if( !ctx->ColorPickerInfo.PickingColor && !ctx->ColorPickerInfo.PickingHue && !ctx->ColorPickerInfo.PickingAlpha ) {
				if( HoveredPicker && InputHelper::Pressed( VK_LBUTTON ) ) {
					ctx->ColorPickerInfo.PickingColor = true;
				}
			}
			else if( ctx->ColorPickerInfo.PickingColor ) {
				if( InputHelper::Down( VK_LBUTTON ) ) {
					vec2_t CursorDelta = g_input.m_mouse - DrawPos;

					float s = ( float )CursorDelta.x / ( DrawSize.x - 30 );
					float v = 1.f - ( float )CursorDelta.y / ( DrawSize.y - 30 );

					// xd
					ColorPreviewPos = g_input.m_mouse;

					NewSaturation = s;
					NewValue = v;
				}
				else {
					ctx->ColorPickerInfo.PickingColor = false;
				}
			}

			g_cfg[ ctx->ColorPickerInfo.Value ].set_color( Color::HSVtoRGB( NewHue, std::clamp<float>( NewSaturation, 0.0f, 1.0f ), std::clamp<float>( NewValue, 0.0f, 1.0f ) )
				.OverrideAlpha( ColorPickerAlpha.at( ctx->ColorPickerInfo.HashedID ) ) );

			D3D::RectFilledLinearGradient( ColorDrawPos, DrawSize - 40 - vec2_t( 1, 0 ), Color::Palette_t::White( ), Color::HSVtoRGB( NewHue, 1, 1 ), true );
			D3D::RectFilledLinearGradient( ColorDrawPos, DrawSize - 40 - vec2_t( 0, 1 ), Color( 0, 0, 0, 0 ), Color( 0, 0, 0 ) );
			D3D::Rect( ColorDrawPos, DrawSize - 40, Color::Palette_t::MenuColors_t::Outlines( ) );

			// the rects that signify what each part of the colorpicker is at

			// alpha
			float AlphaAdditive = ( AlphaDrawSize.x * ( ColorPickerAlpha.at( ctx->ColorPickerInfo.HashedID ) / 255.f ) );
			D3D::Rect( AlphaDrawPos + vec2_t( ( AlphaAdditive - 3 ) < 0 ? 0 : AlphaAdditive - 3, 0 ), vec2_t( 3, AlphaDrawSize.y ), Color::Palette_t::MenuColors_t::Outlines( ) );

			// actual color
			D3D::SetLimit( std::pair<vec2_t, vec2_t>( ColorDrawPos, ColorDrawSize ) );
			{
				D3D::Rect( ColorPreviewPos, vec2_t( 3, 3 ), Color::Palette_t::MenuColors_t::Outlines( ) );
			}
			D3D::ResetLimit( );

			// hue
			float HueAdditive = ( HueDrawSize.y * ( NewHue / 360.f ) );
			D3D::Rect( HueDrawPos + vec2_t( 0, ( HueAdditive - 3 ) < 0 ? 1 : HueAdditive - 2 ), vec2_t( HueDrawSize.x + 1, 3 ), Color::Palette_t::MenuColors_t::Outlines( ) );

			if( InputHelper::Pressed( VK_LBUTTON ) && !HoveredMain ) {
				ctx->ColorPickerInfo.OutAnimation = true;
			}
		}
		D3D::ResetLimit( );
	}
	else {
		ColorPickerHue = -1.f;
	}

	if( !ctx->SubTabs.empty( ) ) {
		if( !ctx->SubTabs.at( ctx->ActiveTab ).empty( ) ) {
			ctx->SubTabs.at( ctx->ActiveTab ).clear( );
		}
	}

	if( !ctx->Tabs.empty( ) ) {
		ctx->Tabs.clear( );
	}
}

bool GUI::Form::BeginTab( std::string icon, std::string name ) {
	ctx->Tabs.push_back( std::make_pair( icon, name ) );

	for( int i = 0; i < ctx->Tabs.size( ); ++i ) {
		if( ctx->SubTabs.find( i ) == ctx->SubTabs.end( ) ) {
			ctx->SubTabs.insert( { i, { } } );
			ctx->ActiveSubTab.insert( { i, { } } );
		}
	}

	return ctx->ActiveTab == ctx->Tabs.size( ) - 1;
}

bool GUI::Form::BeginSubTab( std::string icon, std::string name ) {
	ctx->SubTabs.at( ctx->ActiveTab ).push_back( std::make_pair( icon, name ) );

	return ctx->ActiveSubTab.at( ctx->ActiveTab ) == ctx->SubTabs.at( ctx->ActiveTab ).size( ) - 1;
}