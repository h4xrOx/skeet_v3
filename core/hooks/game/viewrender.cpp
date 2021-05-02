#include "../../../includes.h"

void Hooks::OnRenderStart( ) {
	// call og.
	g_hooks.m_view_render.GetOldMethod< OnRenderStart_t >( CViewRender::ONRENDERSTART )( this );

	if( g_cfg[ XOR( "misc_override_fov" ) ].get<bool>( ) ) {
		if( g_cl.m_local && g_cl.m_local->m_bIsScoped( ) ) {
			if( g_cfg[ XOR( "misc_override_fov_scoped" ) ].get<bool>( ) ) {
				if( g_cl.m_weapon && g_cl.m_weapon->m_zoomLevel( ) != 2 ) {
					g_csgo.m_view_render->m_view.m_fov = g_cfg[ XOR( "misc_override_fov_amount" ) ].get<int>( );
				}
				else {
					g_csgo.m_view_render->m_view.m_fov += 45.f;
				}
			}
		}

		else g_csgo.m_view_render->m_view.m_fov = g_cfg[ XOR( "misc_override_fov_amount" ) ].get<int>( );
	}

	if( g_cfg[ XOR( "misc_override_viewmodel_fov" ) ].get<bool>( ) )
		g_csgo.m_view_render->m_view.m_viewmodel_fov = g_cfg[ XOR( "misc_override_viewmodel_fov_amount" ) ].get<int>( );
}

void Hooks::RenderView( const CViewSetup& view, const CViewSetup& hud_view, int clear_flags, int what_to_draw ) {
	// ...

	g_hooks.m_view_render.GetOldMethod< RenderView_t >( CViewRender::RENDERVIEW )( this, view, hud_view, clear_flags, what_to_draw );
}

void Hooks::Render2DEffectsPostHUD( const CViewSetup& setup ) {
	if( !g_cfg[ XOR( "visuals_misc_remove_flash" ) ].get<bool>( ) )
		g_hooks.m_view_render.GetOldMethod< Render2DEffectsPostHUD_t >( CViewRender::RENDER2DEFFECTSPOSTHUD )( this, setup );
}

void Hooks::RenderSmokeOverlay( bool unk ) {
	// do not render smoke overlay.
	if( !g_cfg[ XOR( "visuals_misc_remove_smoke" ) ].get<bool>( ) )
		g_hooks.m_view_render.GetOldMethod< RenderSmokeOverlay_t >( CViewRender::RENDERSMOKEOVERLAY )( this, unk );
}
