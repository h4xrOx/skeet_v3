#include "../../../includes.h"

bool Hooks::IsConnected( ) {
	if( !this || !g_csgo.m_engine )
		return false;
	
	Stack stack;

	static const Address IsLoadoutAllowed{ pattern::find( g_csgo.m_client_dll, XOR( "84 C0 75 04 B0 01 5F" ) ) };

	if(g_cfg[XOR("misc_unlock_inventory")].get<bool>() && stack.ReturnAddress( ) == IsLoadoutAllowed )
		return false;

	return g_hooks.m_engine.GetOldMethod< IsConnected_t >( IVEngineClient::ISCONNECTED )( this );
}

void Hooks::FireEvents()
{
	/*if (!g_cl.m_processing) {
		g_hooks.m_engine.GetOldMethod< IsConnected_t >(IVEngineClient::FIREEVENTS)(this);
		return;
	}

	if (g_cl.m_allow_fire_events) {
		g_hooks.m_engine.GetOldMethod< IsConnected_t >(IVEngineClient::FIREEVENTS)(this);

		g_cl.m_allow_fire_events = false;
	}*/
	
	g_hooks.m_engine.GetOldMethod< IsConnected_t >(IVEngineClient::FIREEVENTS)(this);
}

bool Hooks::IsHLTV( ) {
	if( !this || !g_csgo.m_engine )
		return false;
	
	Stack stack;

	static const Address SetupVelocity{ pattern::find( g_csgo.m_client_dll, XOR( "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80" ) ) };

	// AccumulateLayers
	if( g_anims.m_bEnablePVS)
		return true;

	// fix for animstate velocity.
	if( stack.ReturnAddress( ) == SetupVelocity )
		return true; 

	return g_hooks.m_engine.GetOldMethod< IsHLTV_t >( IVEngineClient::ISHLTV )( this );
}