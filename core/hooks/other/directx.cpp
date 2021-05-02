#include "../../../includes.h"

HRESULT WINAPI Hooks::Present( IDirect3DDevice9* pDevice, RECT* pRect1, const RECT* pRect2, HWND hWnd, const RGNDATA* pRGNData ) {

	static bool initialised_renderer = false;
	if( !initialised_renderer ) {
		D3D::Initialise( pDevice );
		initialised_renderer = true;
	}
	else {
		g_input.update( );

		InputHelper::Update( );

		bool old_night = g_cfg[ XOR( "misc_world_night" ) ].get<bool>( );
		bool old_tickbase = g_tickbase.m_shift_data.m_should_attempt_shift;
		float old_night_darkness = g_cfg[ XOR( "misc_world_night_darkness" ) ].get<float>( );
		float old_prop_opacity = g_cfg[ XOR( "misc_world_prop_opacity" ) ].get<float>( );
		
		bool old_skins = g_cfg[ XOR( "skins_enable" ) ].get<bool>( );
		int old_knife = g_cfg[ XOR( "skins_knife" ) ].get<int>( );
		int old_gloves = g_cfg[ XOR( "skins_gloves" ) ].get<int>( );
		int old_glove_skins = g_cfg[ XOR( "skins_glove_kit" ) ].get<int>( );
		float old_volume = g_cfg[XOR("misc_hitsound_volume")].get<float>();
		int old_sound = g_cfg[XOR("misc_custom_hitsound")].get<int>();

		D3D::Draw( );

		g_visuals.HitmarkerWorld( );
		Menu::Draw( );

		if( old_night != g_cfg[ XOR( "misc_world_night" ) ].get<bool>( ) ||
			old_night_darkness != g_cfg[ XOR( "misc_world_night_darkness" ) ].get<float>( ) ||
			old_prop_opacity != g_cfg[ XOR( "misc_world_prop_opacity" ) ].get<float>( ) ) {
			g_cl.m_update_night = true;
		}

		if( old_skins != g_cfg[ XOR( "skins_enable" ) ].get<bool>( ) ||
			old_knife != g_cfg[ XOR( "skins_knife" ) ].get<int>( ) ||
			old_gloves != g_cfg[ XOR( "skins_gloves" ) ].get<int>( ) ||
			old_glove_skins != g_cfg[ XOR( "skins_glove_kit" ) ].get<int>( ) ) {
			g_skins.m_update = true;
		}

		if (old_volume != g_cfg[XOR("misc_hitsound_volume")].get<float>() || old_sound != g_cfg[XOR("misc_custom_hitsound")].get<int>()) {
			g_cl.m_update_sound = true;
		}

		D3D::End( );

		g_visuals.m_thirdperson = g_config.get_hotkey( XOR( "misc_thirdperson_key" ) );
		g_hvh.m_fake_duck = g_config.get_hotkey( XOR( "rage_aa_fakeduck_key" ) );
		g_movement.m_slow_motion = g_config.get_hotkey( XOR( "movement_fakewalk_key" ) );
		g_cl.m_negate_desync = g_config.get_hotkey( XOR( "rage_aa_negate_key" ) );
		g_aimbot.m_override_damage = g_config.get_hotkey( XOR( "rage_aimbot_minimal_damage_override_key" ) );
		g_aimbot.m_force_body = g_config.get_hotkey( XOR( "rage_aimbot_baim_key" ) );
		g_tickbase.m_shift_data.m_should_attempt_shift = g_cfg[ XOR( "rage_exploit_type" ) ].get<int>( ) > 0 && g_config.get_hotkey(XOR("rage_exploit_key"));
		g_aimbot.m_force_safepoint = g_config.get_hotkey(XOR("rage_safepoint_key"));


		// note - alpha;
		// the maximum amount of ticks we can shift is sv_maxusrcmdprocessticks
		// which is defaulted to 16 - but since we want to be fakelagging atleast 2 ticks
		// for our fake angle to work, we need to reserve 2 ticks for the fakelag. 
		// we want our doubletap to be as fast as possible, and our fake to be as wide as possible
		// therefore let's just fakelag 2 ticks - resulting in our max shift ticks being 14
		// cos sv_maxusrcmdprocessticks Take Away Two Is Fourteen
		// (assuming that the convar wasn't changed to a higher/lower value).
		g_cl.m_goal_shift = g_cfg[XOR("rage_exploit_type")].get<int>() == 1 ? 13 : 7;

		// (!) Temporary - replace with checkbox & hotkey later.
		if(old_tickbase != g_tickbase.m_shift_data.m_should_attempt_shift) {
			//g_notify.add( tfm::format( XOR( "Tried shifting tickbase" )) );

			if (g_tickbase.m_shift_data.m_should_attempt_shift)
				g_tickbase.m_shift_data.m_needs_recharge = g_cl.m_goal_shift;
			else
				g_tickbase.m_shift_data.m_needs_recharge = 0;

			g_tickbase.m_shift_data.m_did_shift_before = false;
		}

		g_input.updateMouse( );
		g_input.m_scroll = 0;
	}

	return g_hooks.m_device.GetOldMethod<decltype( &Present )>( 17 )( pDevice, pRect1, pRect2, hWnd, pRGNData );
}

HRESULT WINAPI Hooks::Reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentParameters )
{
	D3D::HandleReset( );

	auto result = g_hooks.m_device.GetOldMethod<decltype( &Reset )>( 16 )( pDevice, pPresentParameters );

	if( result == D3D_OK ) {
		D3D::HandleReset( );
	}

	return result;
}