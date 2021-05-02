#include "../../../includes.h"

void __stdcall Hooks::PostDataUpdate(C_TEFireBullets* thisptr, DataUpdateType_t type ) {
	//C_TEFireBullets* shot = ( C_TEFireBullets* )this;
	
	//Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( shot->m_index + 1 );

	g_hooks.m_fire_bullets.GetOldMethod< PostDataUpdate_t >( 7 )(thisptr, type );
}

__declspec (naked) void __stdcall Hooks::PostDataUpdate_h(DataUpdateType_t updateType)
{
	__asm
	{
		push[esp + 4]
		push ecx
		call PostDataUpdate
		retn 4
	}
}