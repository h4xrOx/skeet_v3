#include "../../includes.h"

Detours g_detours{ };

void Detours::init( ) {
	// install detours.

	oReportHit = ( decltype( &ReportHit ) )DetourFunction( reinterpret_cast< byte* >( g_csgo.ReportHit ), reinterpret_cast< byte* >( ReportHit ) );

	oCL_Move = (decltype(&CL_Move))DetourFunction(reinterpret_cast<byte*>(g_csgo.CL_Move), reinterpret_cast<byte*>(CL_Move));
}