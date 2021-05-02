#include "../../../includes.h"

int Hooks::DebugSpreadGetInt( ) {
	if(g_cl.m_processing && !g_cl.m_local->m_bIsScoped() && g_cfg[XOR("misc_force_crosshair")].get<bool>() )
		return 3;

	return g_hooks.m_debug_spread.GetOldMethod< GetInt_t >( ConVar::GETINT )( this );
}

int Hooks::CsmShadowGetInt( ) {
	// fuck the blue nigger shit on props with nightmode
	// :nauseated_face:
	return 0;
}