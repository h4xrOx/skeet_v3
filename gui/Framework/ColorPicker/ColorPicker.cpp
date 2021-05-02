#include "../gui.h"

std::map<size_t, float> PickerAnimation;
std::map<size_t, float> PickerTimer;

void GUI::Controls::ColorPicker( const std::string& name, const std::string& var_name ) {
	vec2_t CursorPos = GetLastCursorPos( );
	vec2_t DrawPos = ctx->pos + CursorPos;
	vec2_t DrawSize = vec2_t( 16, 8 );

	if( PickerAnimation.find( GUI::Hash( name ) ) == PickerAnimation.end( ) ) {
		PickerAnimation.insert( { GUI::Hash( name ), 0.f } );
		PickerTimer.insert( { GUI::Hash( name ), 0.f } );
	}

	if( ctx->FocusedID == GUI::Hash( name ) ) {
		if( ctx->ColorPickerInfo.OutAnimation ) {
			PickerTimer.at( GUI::Hash( name ) ) -= ( 1.0f / 0.1f ) * g_csgo.m_globals->m_frametime;
			PickerTimer.at( GUI::Hash( name ) ) = std::clamp<float>( PickerTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			PickerAnimation.at( GUI::Hash( name ) ) = Easing::InCirc( PickerTimer.at( GUI::Hash( name ) ) );

			if( PickerAnimation.at( GUI::Hash( name ) ) <= 0.01 ) {				
				ctx->FocusedID = 0;
				ctx->ColorPickerInfo.HashedID = 0;
				ctx->ColorPickerInfo.OutAnimation = false;

				PickerTimer.at( GUI::Hash( name ) ) = PickerAnimation.at( GUI::Hash( name ) ) = 0.f;
			}
		}
		else {
			PickerTimer.at( GUI::Hash( name ) ) += ( 1.0f / 0.1f ) * g_csgo.m_globals->m_frametime;
			PickerTimer.at( GUI::Hash( name ) ) = std::clamp<float>( PickerTimer.at( GUI::Hash( name ) ), 0.f, 1.f );

			PickerAnimation.at( GUI::Hash( name ) ) = Easing::OutCirc( PickerTimer.at( GUI::Hash( name ) ) );

			if( PickerAnimation.at( GUI::Hash( name ) ) >= 0.999998 ) {
				PickerAnimation.at( GUI::Hash( name ) ) = 1.f;
			}
		}
	}

	bool Hovered = InputHelper::Hovered( ctx->ParentPos, ctx->ParentSize ) && InputHelper::Hovered( DrawPos + vec2_t( ( ctx->ParentSize.x - 41 ) - DrawSize.x, 4 ) - 1, DrawSize + 2 );

	D3D::RectFilled( DrawPos + vec2_t( ( ctx->ParentSize.x - 42 ) - DrawSize.x, 3 ), DrawSize + 1, g_cfg[ var_name ].get_color( ).OverrideAlpha( 255 * ctx->animation ) );
	D3D::Rect( DrawPos + vec2_t( ( ctx->ParentSize.x - 41 ) - DrawSize.x, 4 ) - 1, DrawSize + 2, Color::Palette_t::MenuColors_t::Outlines( ).OverrideAlpha( 255 * ctx->animation ) );

	if( ctx->FocusedID == 0 ) {
		if( Hovered && InputHelper::Pressed( VK_LBUTTON ) ) {
			ctx->FocusedID = GUI::Hash( name );
		}
	}
	else if( ctx->FocusedID == GUI::Hash( name ) ) {
		ctx->ColorPickerInfo.Animation = PickerAnimation.at( GUI::Hash( name ) );

		ctx->ColorPickerInfo.Value = var_name;
		ctx->ColorPickerInfo.Size = 200;
		ctx->ColorPickerInfo.Pos = DrawPos + vec2_t( ( ctx->ParentSize.x - 42 ) - DrawSize.x, 5 + DrawSize.y );
		ctx->ColorPickerInfo.HashedID = GUI::Hash( name );
	}
}