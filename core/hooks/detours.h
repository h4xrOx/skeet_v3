#pragma once

class Detours {
public:
	void init( );

public:
	// prototypes.
	static bool __cdecl ReportHit( Hit_t* pHurtInfo );
	static void __cdecl CL_Move(float accumulated_extra_samples, bool bFinalTick);

	decltype( &ReportHit ) oReportHit;
	decltype(&CL_Move) oCL_Move;
};

extern Detours g_detours;