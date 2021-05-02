#include "../GUI.h"

#pragma warning(disable : 4018)

std::map< int, float > iSlider;
std::map< int, float > iPreviousSlider;
std::map<size_t, std::pair<float, bool>> bShouldSlide;

std::map< int, std::string > szText;

std::map<size_t, bool> _ReversePulseOutline;

std::map<size_t, float> _PulseOutlineAnimation;
std::map<size_t, float> _PulseOutlineTimer;

std::map<size_t, float> _LastPusleText;
std::map<size_t, bool> _PusleText;

std::map<size_t, float> LastShiftHoldTime;

bool GUI::Controls::Listbox( const std::string& id, std::vector<std::string> elements, std::string option, bool bSearchBar, int iSizeInElements ) {
	vec2_t CursorPos = GUI::PopCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;

	std::map< size_t, std::vector<std::string> > vecFoundElements;
	std::map< int, int > iPos;

	if( iSlider.find( GUI::Hash( id ) ) == iSlider.end( ) ) {
		iSlider.insert( { GUI::Hash( id ), 0.f } );
		iPreviousSlider.insert( { GUI::Hash( id ), 0.f } );
		bShouldSlide.insert( { GUI::Hash( id ), { 0.f, false } } );
	}

	if( LastShiftHoldTime.find( GUI::Hash( id ) ) == LastShiftHoldTime.end( ) ) {
		LastShiftHoldTime.insert( { GUI::Hash( id ), 0.f } );
	}

	if( vecFoundElements.find( GUI::Hash( id ) ) == vecFoundElements.end( ) ) {
		vecFoundElements.insert( { GUI::Hash( id ), { "" } } );
	}

	if( szText.find( GUI::Hash( id ) ) == szText.end( ) ) {
		szText.insert( { GUI::Hash( id ), "" } );
	}

	if( iPos.find( GUI::Hash( id ) ) == iPos.end( ) ) {
		iPos.insert( { GUI::Hash( id ), -20 } );
	}

	if( _PulseOutlineAnimation.find( GUI::Hash( id ) ) == _PulseOutlineAnimation.end( ) ) {
		_PulseOutlineAnimation.insert( { GUI::Hash( id ), 0.f } );
		_PulseOutlineTimer.insert( { GUI::Hash( id ), 0.f } );

		_ReversePulseOutline.insert( { GUI::Hash( id ), true } );
		_PusleText.insert( { GUI::Hash( id ), false } );
		_LastPusleText.insert( { GUI::Hash( id ), 0.f } );
	}

	if( _ReversePulseOutline.at( GUI::Hash( id ) ) ) {
		_PulseOutlineTimer.at( GUI::Hash( id ) ) += ( 1.0f / 0.9f ) * g_csgo.m_globals->m_frametime;
		_PulseOutlineTimer.at( GUI::Hash( id ) ) = std::clamp<float>( _PulseOutlineTimer.at( GUI::Hash( id ) ), 0.f, 1.f );

		_PulseOutlineAnimation.at( GUI::Hash( id ) ) = Easing::OutCirc( _PulseOutlineTimer.at( GUI::Hash( id ) ) );

		if( _PulseOutlineAnimation.at( GUI::Hash( id ) ) >= 0.999998 ) {
			_PulseOutlineAnimation.at( GUI::Hash( id ) ) = 1.f;
		}
	}
	else {
		_PulseOutlineTimer.at( GUI::Hash( id ) ) -= ( 1.0f / 0.9f ) * g_csgo.m_globals->m_frametime;
		_PulseOutlineTimer.at( GUI::Hash( id ) ) = std::clamp<float>( _PulseOutlineTimer.at( GUI::Hash( id ) ), 0.f, 1.f );

		_PulseOutlineAnimation.at( GUI::Hash( id ) ) = Easing::OutCirc( _PulseOutlineTimer.at( GUI::Hash( id ) ) );

		if( _PulseOutlineAnimation.at( GUI::Hash( id ) ) <= 0.05f ) {
			_PulseOutlineAnimation.at( GUI::Hash( id ) ) = 0.f;
		}
	}

	if( _PulseOutlineAnimation.at( GUI::Hash( id ) ) >= 1.0f || _PulseOutlineAnimation.at( GUI::Hash( id ) ) <= 0.f ) {
		_ReversePulseOutline.at( GUI::Hash( id ) ) = !_ReversePulseOutline.at( GUI::Hash( id ) );
	}

	DrawPos.x += 22;
	if( bSearchBar ) {
		DrawPos.y += 19;
	}

	Color OutlineColor( Color::Blend( Color::Palette_t::MenuColors_t::Outlines( ), g_cfg[ XOR( "menu_color" ) ].get_color( ), _PulseOutlineAnimation.at( GUI::Hash( id ) ) ) );

	int iWidth = ( static_cast< int > ( ctx->ParentSize.x - 82 > 200.f ) ? 200.f : ctx->ParentSize.x - 82 );

	bool bHoveredSearch = InputHelper::Hovered( DrawPos - vec2_t( 0, 18 ), vec2_t( iWidth, 19 ) ) && InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && bSearchBar;
	bool bHovered = InputHelper::Hovered( DrawPos, vec2_t( iWidth, 20 * iSizeInElements ) ) && InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize );

	D3D::SetLimit( std::pair<vec2_t, vec2_t>( DrawPos, vec2_t( iWidth, 20 * iSizeInElements ) ) );
	for( int i = 0; i < elements.size( ); i++ ) {
		std::string transformed_elements( elements.at( i ) );
		std::transform( transformed_elements.begin( ), transformed_elements.end( ), transformed_elements.begin( ), ::tolower );

		std::string transformed_text( szText[ GUI::Hash( id ) ] );
		std::transform( transformed_text.begin( ), transformed_text.end( ), transformed_text.begin( ), ::tolower );

		if( transformed_elements.find( transformed_text ) == std::string::npos ) {
			continue;
		}
		else {
			vecFoundElements[ GUI::Hash( id ) ].push_back( elements.at( i ) );
			iPos[ GUI::Hash( id ) ] += 20;
		}

		vec2_t OptionPos = DrawPos + vec2_t( 0, iPos[ GUI::Hash( id ) ] + iSlider[ GUI::Hash( id ) ] );
		vec2_t OptionSize = vec2_t( iWidth - ( elements.size( ) > iSizeInElements ? 1 : 0 ), 20 );

		// add and subtract 20 for tolerance - we do this cos the "scroll" is a float
		// and not an int (for the epic smooth easing effect) - just clip the elements,
		// drawing 2 more elements wont affect performance that much.
		bool bIsInBounds = OptionPos.y >= ( DrawPos.y - 20 ) &&
			OptionPos.y + OptionSize.y <= ( DrawPos.y + 20 ) + 20 * iSizeInElements;

		if( bIsInBounds ) {
			bool bOptionHovered = InputHelper::Hovered( OptionPos, OptionSize ) && !bHoveredSearch && InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize );

			D3D::RectFilled( OptionPos, OptionSize, (bOptionHovered && ctx->FocusedID == 0 ?
				Color::Palette_t::MenuColors_t::ContainerHovered( ) : Color::Palette_t::MenuColors_t::Container( )).OverrideAlpha(255*ctx->animation) );

			if( ctx->FocusedID == 0 ) {
				if( bOptionHovered && InputHelper::Pressed( VK_LBUTTON ) )
					ctx->FocusedID = GUI::Hash( id );
			}
			else if( ctx->FocusedID == GUI::Hash( id ) ) {
				if( InputHelper::Pressed( VK_LBUTTON ) && !bOptionHovered ) {
					ctx->FocusedID = 0;
				}

				if( InputHelper::Pressed( VK_LBUTTON ) ) {
					g_cfg[ option ].set( i - 1 );
					ctx->FocusedID = 0;
				}
			}
		}

		if( bIsInBounds ) {
			if( g_cfg[ option ].get<int>( ) != i ) {
				D3D::Text( OptionPos + vec2_t( 10, 2 ), elements[ i ].c_str( ), Color( 220, 220, 220, 255 * ctx->animation ), D3D::D3D9Fonts[ D3D::EFonts::MAIN_FONT ], 0 );
			}
			else {
				D3D::Text( OptionPos + vec2_t( 10, 2 ), elements[ i ].c_str( ), g_cfg[ XOR( "menu_color" ) ].get_color( ).OverrideAlpha( 255 * ctx->animation ), D3D::D3D9Fonts[ D3D::EFonts::MAIN_FONT ], 0 );
			}
		}
	}
	D3D::ResetLimit( );

	// listbox
	if( bHovered && vecFoundElements[ GUI::Hash( id ) ].size( ) > iSizeInElements ) {
		if( ctx->FocusedID == 0 ) {
			if( g_input.m_scroll > 0 ) {
				bShouldSlide[ GUI::Hash( id ) ] = std::make_pair( iSlider[ GUI::Hash( id ) ] + 25, true );
			}
			else if( g_input.m_scroll < 0 ) {
				bShouldSlide[ GUI::Hash( id ) ] = std::make_pair( iSlider[ GUI::Hash( id ) ] - 25, true );
			}

			if( bShouldSlide[ GUI::Hash( id ) ].second ) {
				iSlider[ GUI::Hash( id ) ] = GUI::Approach( iPreviousSlider[ GUI::Hash( id ) ], bShouldSlide[ GUI::Hash( id ) ].first, ( 1.0f / 0.03f ) * g_csgo.m_globals->m_frametime );

				if( iPreviousSlider[ GUI::Hash( id ) ] == bShouldSlide[ GUI::Hash( id ) ].first ) {
					bShouldSlide[ GUI::Hash( id ) ].second = false;
				}
			}

			iPreviousSlider[ GUI::Hash( id ) ] = iSlider[ GUI::Hash( id ) ];
		}

		iSlider[ GUI::Hash( id ) ] = std::clamp<int>( iSlider[ GUI::Hash( id ) ], ( ( 20 * ( vecFoundElements[ GUI::Hash( id ) ].size( ) - 1 ) ) * -1 ) + ( int )( 20 * iSizeInElements ), 0 );
	}

	if( vecFoundElements[ GUI::Hash( id ) ].size( ) > iSizeInElements ) {
		int scrollbar_height = ( ( float )( 20 * iSizeInElements ) / ( float )( 20 * vecFoundElements[ GUI::Hash( id ) ].size( ) ) ) * ( 20 * iSizeInElements );
		int scrollbar_pos = ( -( float )iSlider[ GUI::Hash( id ) ] / ( float )( 20 * vecFoundElements[ GUI::Hash( id ) ].size( ) ) ) * ( float )( 20 * iSizeInElements );

		D3D::RectFilled( DrawPos + vec2_t( iWidth - 4, scrollbar_pos ), vec2_t( 2, scrollbar_height + 1 ), g_cfg[ XOR( "menu_color" ) ].get_color( ).OverrideAlpha( 255 * ctx->animation ) );
	}

	// search
	if( bSearchBar ) {
		D3D::RectFilled( DrawPos - vec2_t( 0, 18 ), vec2_t( iWidth, 18 ), Color::Palette_t::MenuColors_t::Container( ).OverrideAlpha( 255 * ctx->animation ) );

		if( ctx->FocusedID == 0 ) {
			if( ctx->typing )
				ctx->typing = false;

			if( bHoveredSearch && InputHelper::Pressed( VK_LBUTTON ) )
				ctx->FocusedID = GUI::Hash( id + XOR( "search" ) );
		}
		else if( ctx->FocusedID == GUI::Hash( id + XOR( "search" ) ) ) {
			if( !ctx->typing )
				ctx->typing = true;

			if( InputHelper::Pressed( VK_LBUTTON ) && !bHoveredSearch ) {
				ctx->FocusedID = 0;
			}

			if( g_csgo.m_globals->m_curtime - _LastPusleText.at( GUI::Hash( id ) ) > 0.5f ) {
				_PusleText.at( GUI::Hash( id ) ) = !_PusleText.at( GUI::Hash( id ) );
				_LastPusleText.at( GUI::Hash( id ) ) = g_csgo.m_globals->m_curtime;
			}

			for( int i = 0; i < 255; i++ ) {
				if( InputHelper::Pressed( i ) ) {
					if( i != VK_BACK ) {
						if( i == VK_ESCAPE || i == VK_RETURN ) {
							ctx->FocusedID = 0;
						}
						else if( i >= 'A' && i <= 'Z' ) {
							szText[ GUI::Hash( id ) ] += ( ( InputHelper::Down( VK_SHIFT ) ) || GetKeyState( VK_CAPITAL ) ) ? i : i + 32;
							iSlider[ GUI::Hash( id ) ] = 0;
						}
						else if( i >= '0' && i <= '9' || i == ' ' ) {
							szText[ GUI::Hash( id ) ] += i;
							iSlider[ GUI::Hash( id ) ] = 0;
						}
					}
					else {
						szText[ GUI::Hash( id ) ] = szText[ GUI::Hash( id ) ].substr( 0, szText[ GUI::Hash( id ) ].size( ) - 1 );
						iSlider[ GUI::Hash( id ) ] = 0;
					}
				}

				// left control + backspace = delete every fucking thing
				if( InputHelper::Down( VK_LCONTROL ) && GetAsyncKeyState( VK_BACK ) ) {
					szText[ GUI::Hash( id ) ] = szText[ GUI::Hash( id ) ].substr( 0, szText[ GUI::Hash( id ) ].size( ) - 1 );
					iSlider[ GUI::Hash( id ) ] = 0;
				}
			}

			// if we arent holding backspace store the time
			if( !GetAsyncKeyState( VK_BACK ) ) {
				LastShiftHoldTime[ GUI::Hash( id ) ] = g_csgo.m_globals->m_curtime;
			}

			// the delta between the current time and last "non held shift" time exceeds 500ms,
			// this means that we've been holding shift for 500ms. let's erase.
			if( g_csgo.m_globals->m_curtime - LastShiftHoldTime[ GUI::Hash( id ) ] > 0.5f ) {
				szText[ GUI::Hash( id ) ] = szText[ GUI::Hash( id ) ].substr( 0, szText[ GUI::Hash( id ) ].size( ) - 1 );
				iSlider[ GUI::Hash( id ) ] = 0;
			}
		}

		if( ctx->FocusedID == 0 ) {
			if( _PusleText.at( GUI::Hash( id ) ) )
				_PusleText.at( GUI::Hash( id ) ) = false;
		}

		D3D::Text( DrawPos - vec2_t( -4, 16 ), ( szText[ GUI::Hash( id ) ] + ( _PusleText.at( GUI::Hash( id ) ) ? '_' : ' ' ) ).c_str( ), Color( 190, 190, 190 )
			.OverrideAlpha( 255 * ctx->animation, true ), D3D::D3D9Fonts[ D3D::EFonts::MAIN_FONT ], 0 );
	}
	else {
		if( ctx->typing )
			ctx->typing = false;
	}

	if( bSearchBar ) {
		D3D::Rect( DrawPos - vec2_t( 0, 18 ),
			vec2_t( iWidth, 20 * iSizeInElements ) + vec2_t( 0, 18 ),
			Color::Palette_t::MenuColors_t::Outlines( ).OverrideAlpha( 255 * ctx->animation, true ) );

		D3D::Rect( DrawPos - vec2_t( 0, 18 ), vec2_t( iWidth, 19 ),( ctx->FocusedID == GUI::Hash( id + XOR( "search" ) ) ? OutlineColor : Color::Palette_t::MenuColors_t::Outlines( )).OverrideAlpha( 255 * ctx->animation, true ) );
	}
	else {
		D3D::Rect( DrawPos,
			vec2_t( iWidth, 20 * iSizeInElements ),
			Color::Palette_t::MenuColors_t::Outlines( ).OverrideAlpha( 255 * ctx->animation, true ) );
	}

	GUI::PushCursorPos( CursorPos + vec2_t( 0, ( 20 * iSizeInElements ) + ( bSearchBar ? ( GUI::ObjectPadding( ) + 20 ) : GUI::ObjectPadding( ) ) ) );
	return ctx->FocusedID == GUI::Hash( id );
}