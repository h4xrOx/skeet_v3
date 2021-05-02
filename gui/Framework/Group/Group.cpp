#include "../gui.h"
#include <algorithm>

vec2_t _GroupPos;
vec2_t _GroupSize;
vec2_t _CursorPos;
int id;

std::map< int, float > Scrolling;
std::map< int, float > PreviousScrolling;
std::map<size_t, std::pair<float, bool>> ShouldScroll;

void GUI::Group::BeginGroup( const std::string& name, const vec2_t& size ) {
	size_t id = GUI::Hash( name );
	::id = id;

	if( Scrolling.find( id ) == Scrolling.end( ) ) {
		Scrolling.insert( { id, 0.f } );
		PreviousScrolling.insert( { id, 0.f } );
		ShouldScroll.insert( { id, { 0.f, false } } );
	}

	vec2_t CursorPos = PopCursorPos( );
	vec2_t GroupSize;

	float WidthPercent = ( float )size.x / 100.f;
	float HeightPercent = ( float )size.y / 100.f;

	int WidthAvailable = ctx->size.x - 47;
	int HeightAvailable = ctx->size.y - 110;

	if( CursorPos.x == 16 && WidthPercent == 1.0f ) {
		WidthAvailable += 15;
	}

	GroupSize.x = int( WidthAvailable * WidthPercent);

	if( CursorPos.y == 46 && HeightPercent == 1.0f ) {
		HeightAvailable += 15;
	}

	GroupSize.y = int(HeightAvailable * HeightPercent);

	if( ( CursorPos.y - 15 + GroupSize.y ) > ctx->size.y - 15 ) {
		CursorPos.x += GroupSize.x + 15;
		CursorPos.y = 46;

		PushCursorPos( CursorPos );
		Group::BeginGroup( name, size );
	}
	else {
		vec2_t draw_pos = ctx->pos + CursorPos;

		D3D::RectFilledLinearGradient( vec2_t( int( draw_pos.x - 1 ), int( draw_pos.y - 1 ) ), vec2_t( int( GroupSize.x + 2 ), int( GroupSize.y + 2 ) ),
			Color( 0, 0, 0, 0 ), Color( 0, 0, 0, 150 ).OverrideAlpha( 150 * ctx->animation, true ) );

		D3D::RectFilled( vec2_t( int( draw_pos.x ), int( draw_pos.y) ), vec2_t( int( GroupSize.x ), int( GroupSize.y ) ),
			Color::Palette_t::MenuColors_t::Container( ).OverrideAlpha( 255 * ctx->animation, true ) );

		_GroupPos = draw_pos;
		_GroupSize = GroupSize;

		if( name.length( ) > 0 ) {
			D3D::Text( draw_pos + vec2_t( 6, 3 ), name.data( ), Color( 220, 220, 220 ).OverrideAlpha( 255 * ctx->animation, true ),
				D3D::D3D9Fonts[ D3D::EFonts::GROUP_FONT ],
				D3D::EFontFlags::NONE );
		}

		vec2_t clip_pos = { ( int )draw_pos.x, int( draw_pos.y + 22 ) };
		vec2_t clip_size = { ( int )GroupSize.x, int( GroupSize.y - 22 ) };

		D3D::RectFilled( draw_pos + vec2_t( 1, 20 ), vec2_t( GroupSize.x - 2, 2 ), Color::Palette_t::MenuColors_t::Outlines( ).OverrideAlpha( 150 * ctx->animation, true ) );

		D3D::SetLimit( std::pair<vec2_t, vec2_t>( clip_pos, clip_size ) );

		vec2_t FinalPos( CursorPos + vec2_t( 20, 41 + Scrolling[ id ] ) );
		FinalPos = vec2_t( static_cast< int >( FinalPos.x ), static_cast< int >( FinalPos.y ) );

		PushCursorPos( FinalPos );

		ctx->parent = "root." + ctx->Tabs[ ctx->ActiveTab ].second + "." + ( ctx->SubTabs.at( ctx->ActiveTab ).empty( ) ? "-1" : ctx->SubTabs.at( ctx->ActiveTab )[ ctx->ActiveSubTab.at( ctx->ActiveTab ) ].second ) + "." + name;
		ctx->NextGroupPos = CursorPos + vec2_t( 0, ( int )GroupSize.y + 15 );

		ctx->ParentPos = draw_pos;
		ctx->ParentSize = vec2_t( ( int )GroupSize.x, (int)GroupSize.y);
	}
}

void GUI::Group::EndGroup( ) {
	vec2_t v1 = PopCursorPos( ); ;
	int MaxHeight = ( int )v1.y - ( ( int )_GroupPos.y - ( int )ctx->pos.y ) - Scrolling[ id ];
	int VisibleHeight = _GroupSize.y + 5;
	MaxHeight = abs( MaxHeight );

	PushCursorPos( v1 );

	int ScrollBarHeight, ScrollBarPos;

	if( MaxHeight > VisibleHeight ) {
		ScrollBarHeight = ( ( float )_GroupSize.y / ( float )MaxHeight ) * _GroupSize.y;
		ScrollBarPos = std::min( std::max( ( -( float )Scrolling[ id ] / ( float )MaxHeight ) * ( float )_GroupSize.y, 2.f ), _GroupSize.y - ScrollBarHeight - 2.f );

		bool ScrollBarHovered = InputHelper::Hovered( _GroupPos + vec2_t( _GroupSize.x - 6, ScrollBarPos ), vec2_t( 4, ScrollBarHeight ) );

		if( ctx->FocusedID == 0 ) {
			if( InputHelper::Hovered( _GroupPos, _GroupSize ) ) {
				if( g_input.m_scroll > 0 ) {
					ShouldScroll[ id ] = std::make_pair( Scrolling[ id ] + 25, true );
				}
				else if( g_input.m_scroll < 0 ) {
					ShouldScroll[ id ] = std::make_pair( Scrolling[ id ] - 25, true );
				}
			}

			if( ShouldScroll[ id ].second ) {
				Scrolling[ id ] = GUI::Approach( PreviousScrolling[ id ], ShouldScroll[ id ].first, ( 1.0f / 0.03f ) * g_csgo.m_globals->m_frametime );

				if( PreviousScrolling[ id ] == ShouldScroll[ id ].first ) {
					ShouldScroll[ id ].second = false;
				}
			}

			PreviousScrolling[ id ] = Scrolling[ id ];
		}

		Scrolling[ id ] = std::clamp<float>( Scrolling[ id ], -MaxHeight + ( int )_GroupSize.y, 0 );
	}
	else {
		Scrolling[ id ] = 0;
	}

	D3D::ResetLimit( );

	PushCursorPos( ctx->NextGroupPos );
	ctx->NextGroupPos = vec2_t( 0, 0 );

	if( MaxHeight > VisibleHeight ) {
		D3D::RectFilledLinearGradient( vec2_t( ( int )_GroupPos.x, (int)_GroupPos.y ) + vec2_t( 1, int( _GroupSize.y - 10) ), vec2_t( int( _GroupSize.x - 2), 10 ), Color( 0, 0, 0, 0 ),
			Color::Palette_t::MenuColors_t::Container( ).OverrideAlpha( 255 * ctx->animation, true ) );

		bool ScrollBarHovered = InputHelper::Hovered( vec2_t( ( int )_GroupPos.x, ( int )_GroupPos.y ) + vec2_t( int( _GroupSize.x) - 6, ScrollBarPos ), vec2_t( 4, ScrollBarHeight ) );
		D3D::RectFilled( vec2_t( ( int )_GroupPos.x, ( int )_GroupPos.y ) + vec2_t( int( _GroupSize.x )- 4 - ( ( false ) ? 2 : 0 ), ScrollBarPos + 22 ),
			vec2_t( 2 + ( ( false ) ? 2 : 0 ), ScrollBarHeight - 22 ), g_cfg[ XOR( "menu_color" ) ].get_color( ).OverrideAlpha( 255 * ctx->animation, true ) );
	}

	_GroupPos = _GroupSize = vec2_t( 0, 0 );
}