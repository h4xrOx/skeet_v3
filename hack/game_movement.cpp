#include "../includes.h"

GameMovement g_game_movement;

void GameMovement::Extrapolate( Player* pEntity, vec3_t& vecOrigin, vec3_t& vecVelocity, int& fFlags, bool bOnGround ) {
	vec3_t                start, end, normal;
	CGameTrace            trace;
	CTraceFilterWorldOnly filter;

	// define trace start.
	start = vecOrigin;

	// move trace end one tick into the future using predicted velocity.
	end = start + ( vecVelocity * g_csgo.m_globals->m_interval );

	// trace.
	g_csgo.m_engine_trace->TraceRay( Ray( start, end, pEntity->m_vecMins(), pEntity->m_vecMaxs() ), CONTENTS_SOLID, &filter, &trace );

	// we hit shit
	// we need to fix shit.
	if( trace.m_fraction != 1.f ) {

		// fix sliding on planes.
		for( int i{ }; i < 2; ++i ) {
			vecVelocity -= trace.m_plane.m_normal * vecVelocity.dot( trace.m_plane.m_normal );

			float adjust = vecVelocity.dot( trace.m_plane.m_normal );
			if( adjust < 0.f )
				vecVelocity -= ( trace.m_plane.m_normal * adjust );

			start = trace.m_endpos;
			end = start + ( vecVelocity * ( g_csgo.m_globals->m_interval * ( 1.f - trace.m_fraction ) ) );

			g_csgo.m_engine_trace->TraceRay( Ray( start, end, pEntity->m_vecMins( ), pEntity->m_vecMaxs( ) ), CONTENTS_SOLID, &filter, &trace );
			if( trace.m_fraction == 1.f )
				break;
		}
	}

	// set new final origin.
	start = end = vecOrigin = trace.m_endpos;

	// move endpos 2 units down.
	// this way we can check if we are in/on the ground.
	end.z -= 2.f;

	// trace.
	g_csgo.m_engine_trace->TraceRay( Ray( start, end, pEntity->m_vecMins( ), pEntity->m_vecMaxs( ) ), CONTENTS_SOLID, &filter, &trace );

	// strip onground flag.
	fFlags &= ~FL_ONGROUND;

	// add back onground flag if we are onground.
	if( trace.m_fraction != 1.f && trace.m_plane.m_normal.z > 0.7f )
		fFlags |= FL_ONGROUND;
}