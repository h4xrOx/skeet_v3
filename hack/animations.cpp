#include "../includes.h"

Animations g_anims;

void Animations::AnimationInfo_t::UpdateAnimations( LagComp::LagRecord_t* pRecord, LagComp::LagRecord_t* pPreviousRecord ) {
	if( !pPreviousRecord ) {
		pRecord->m_vecVelocity = m_pEntity->m_vecVelocity( );

		g_resolver.ResolveAngles(m_pEntity, pRecord);

		pRecord->Apply( m_pEntity );

		g_anims.UpdatePlayer( m_pEntity );

		return;
	}

	vec3_t vecVelocity = m_pEntity->m_vecVelocity( );

	m_pEntity->SetAnimLayers( pRecord->m_pLayers );
	m_pEntity->SetAbsOrigin( pRecord->m_vecOrigin );
	m_pEntity->SetAbsAngles( pRecord->m_angAbsAngles );
	m_pEntity->m_vecVelocity( ) = pPreviousRecord->m_vecVelocity;

	pRecord->m_vecVelocity = vecVelocity;

	pRecord->m_bDidShot = pRecord->m_flLastShotTime > pPreviousRecord->m_flSimulationTime && pRecord->m_flLastShotTime <= pRecord->m_flSimulationTime;

	vec3_t vecPreviousOrigin = pPreviousRecord->m_vecOrigin;
	int fPreviousFlags = pPreviousRecord->m_fFlags;

	for( int i = 0; i < pRecord->m_iChoked; ++i ) {
		const float flSimulationTime = pPreviousRecord->m_flSimulationTime + game::TICKS_TO_TIME( i + 1 );
		const float flLerp = 1.f - ( pRecord->m_flSimulationTime - flSimulationTime ) / ( pRecord->m_flSimulationTime - pPreviousRecord->m_flSimulationTime );

		if( !pRecord->m_bDidShot ) {
			auto vecEyeAngles = math::Interpolate( pPreviousRecord->m_angEyeAngles, pRecord->m_angEyeAngles, flLerp );
			m_pEntity->m_angEyeAngles( ).y = vecEyeAngles.y;
		}

		m_pEntity->m_flDuckAmount( ) = math::Interpolate( pPreviousRecord->m_flDuck, pRecord->m_flDuck, flLerp );

		if( pRecord->m_iChoked - 1 == i ) {
			m_pEntity->m_vecVelocity( ) = vecVelocity;
			m_pEntity->m_fFlags( ) = pRecord->m_fFlags;
		}
		else {
			g_game_movement.Extrapolate( m_pEntity, vecPreviousOrigin, m_pEntity->m_vecVelocity( ), m_pEntity->m_fFlags( ), fPreviousFlags & FL_ONGROUND );
			fPreviousFlags = m_pEntity->m_fFlags( );

			pRecord->m_vecVelocity = ( pRecord->m_vecOrigin - pPreviousRecord->m_vecOrigin ) * ( 1.f / game::TICKS_TO_TIME( pRecord->m_iChoked ) );
		}

		if( pRecord->m_bDidShot ) {
			/* NOTE FROM ALPHA:
			* This is to attempt resolving players' onshot desync, using the last angle they were at before they shot
			* It's good enough, but I will leave another (alternative) concept that could work when attempting to resolve on-shot desync
			* I haven't tested it at all, but the logic is there and in theory it should work fine (it's missing nullptr checks, obviously, since it's a concept)
			*
			* float flPseudoFireYaw = Math::NormalizeYaw( Math::CalculateAngle( m_pPlayer->GetBonePos( 8 ), m_pPlayer->GetBonePos( 0 ) ).y );
			* float flLeftFireYawDelta = fabsf( Math::NormalizeYaw( flPseudoFireYaw - ( pRecord->m_angEyeAngles.y + 58.f ) ) );
			* float flRightFireYawDelta = fabsf( Math::NormalizeYaw( flPseudoFireYaw - ( pRecord->m_angEyeAngles.y - 58.f ) ) );
			*
			* pRecord->m_pState->m_flGoalFeetYaw = pRecord->m_pState->m_flEyeYaw + ( flLeftFireYawDelta > flRightFireYawDelta ? -58.f : 58.f );
			*/

			m_pEntity->m_angEyeAngles( ) = pPreviousRecord->m_angLastReliableAngle;

			if( pRecord->m_flLastShotTime <= flSimulationTime ) {
				m_pEntity->m_angEyeAngles( ) = pRecord->m_angEyeAngles;
			}
		}

		const float flBackupSimtime = m_pEntity->m_flSimulationTime( );

		m_pEntity->m_flSimulationTime( ) = flSimulationTime;

		g_resolver.ResolveAngles(m_pEntity, pRecord);

		g_anims.UpdatePlayer( m_pEntity );

		m_pEntity->m_flSimulationTime( ) = flBackupSimtime;
	}
}

void Animations::UpdatePlayer( Player* pEntity ) {
	static bool& invalidate_bone_cache = **reinterpret_cast< bool** >( pattern::find( g_csgo.m_client_dll, XOR( "C6 05 ? ? ? ? ? 89 47 70" ) ).add( 2 ).as<uint32_t>( ) );

	const float frametime = g_csgo.m_globals->m_frametime;
	const float curtime = g_csgo.m_globals->m_curtime;

	CCSGOPlayerAnimState* const state = pEntity->m_PlayerAnimState( );
	if( !state ) {
		return;
	}

	g_csgo.m_globals->m_frametime = g_csgo.m_globals->m_interval;
	g_csgo.m_globals->m_curtime = pEntity->m_flSimulationTime( );

	// skip call to C_BaseEntity::CalcAbsoluteVelocity
	pEntity->m_iEFlags( ) &= ~0x1000; // EFL_DIRTY_ABSVELOCITY

	if( state->m_iLastClientSideAnimationUpdateFramecount == g_csgo.m_globals->m_frame ) {
		state->m_iLastClientSideAnimationUpdateFramecount -= 1;
	}

	const bool backup_invalidate_bone_cache = invalidate_bone_cache;

	pEntity->m_bClientSideAnimation( ) = true;
	pEntity->UpdateClientSideAnimation( );
	pEntity->m_bClientSideAnimation( ) = false;

	pEntity->InvalidatePhysicsRecursive( ANGLES_CHANGED );
	pEntity->InvalidatePhysicsRecursive( ANIMATION_CHANGED );
	pEntity->InvalidatePhysicsRecursive( SEQUENCE_CHANGED );

	invalidate_bone_cache = backup_invalidate_bone_cache;

	g_csgo.m_globals->m_frametime = frametime;
	g_csgo.m_globals->m_curtime = curtime;
}

bool Animations::GenerateMatrix( Player* pEntity, BoneArray* pBoneToWorldOut, int boneMask, float currentTime )
{
	if( !pEntity || !pEntity->IsPlayer( ) || !pEntity->alive( ) )
		return false;

	CStudioHdr* pStudioHdr = pEntity->GetModelPtr( );
	if( !pStudioHdr ) {
		return false;
	}

	// get ptr to bone accessor.
	CBoneAccessor* accessor = &pEntity->m_BoneAccessor( );
	if( !accessor )
		return false;

	// store origial output matrix.
	// likely cachedbonedata.
	BoneArray* backup_matrix = accessor->m_pBones;
	if( !backup_matrix )
		return false;

	vec3_t vecAbsOrigin = pEntity->GetAbsOrigin( );
	ang_t vecAbsAngles = pEntity->GetAbsAngles( );

	vec3_t vecBackupAbsOrigin = pEntity->GetAbsOrigin( );
	ang_t vecBackupAbsAngles = pEntity->GetAbsAngles( );

	matrix3x4a_t parentTransform;
	math::AngleMatrix( vecAbsAngles, vecAbsOrigin, parentTransform );

	pEntity->m_fEffects( ) |= 8;

	pEntity->SetAbsOrigin( vecAbsOrigin );
	pEntity->SetAbsAngles( vecAbsAngles );

	vec3_t pos[ 128 ];
	__declspec( align( 16 ) ) quaternion_t     q[ 128 ];

	m_bEnablePVS = true;

	uint32_t fBackupOcclusionFlags = pEntity->m_iOcclusionFlags( );

	pEntity->m_iOcclusionFlags( ) |= 0xA; // skipp call to accumulatelayers in standardblendingrules

	pEntity->StandardBlendingRules( pStudioHdr, pos, q, currentTime, boneMask );

	pEntity->m_iOcclusionFlags( ) = fBackupOcclusionFlags; // standardblendingrules was called now restore niggaaaa

	accessor->m_pBones = pBoneToWorldOut;

	uint8_t computed[ 0x100 ];
	std::memset( computed, 0, 0x100 );

	pEntity->BuildTransformations( pStudioHdr, pos, q, parentTransform, boneMask, computed );

	accessor->m_pBones = backup_matrix;

	pEntity->SetAbsOrigin( vecBackupAbsOrigin );
	pEntity->SetAbsAngles( vecBackupAbsAngles );

	m_bEnablePVS = false;

	return true;
}

void Animations::UpdateLocalAnimations( ) {
	if( !g_cl.m_cmd || !g_cl.m_processing )
		return;

	CCSGOPlayerAnimState* state = g_cl.m_local->m_PlayerAnimState( );
	if( !state ) {
		return;
	}

	if( !g_csgo.m_cl ) {
		return;
	}

	// allow re-animating in the same frame.
	if( state->m_iLastClientSideAnimationUpdateFramecount == g_csgo.m_globals->m_frame ) {
		state->m_iLastClientSideAnimationUpdateFramecount -= 1;
	}

	// update anim update delta as server build.
	state->m_flAnimUpdateDelta = math::Max( 0.0f, g_csgo.m_globals->m_curtime - state->m_flLastClientSideAnimationUpdateTime ); // negative values possible when clocks on client and server go out of sync..

	if( g_cl.m_animate ) {
		// get layers.
		g_cl.m_local->GetAnimLayers( g_cl.m_real_layers );

		// allow the game to update animations this tick.
		g_cl.m_update = true;

		// update animations.
		game::UpdateAnimationState( state, g_cl.m_angle );
		g_cl.m_local->UpdateClientSideAnimation( );

		// disallow the game from updating animations this tick.
		g_cl.m_update = false;

		// save data when our choke cycle resets.
		if( !g_csgo.m_cl->m_choked_commands ) {
			g_cl.m_rotation.y = state->m_flGoalFeetYaw;
			g_cl.m_local->GetPoseParameters( g_cl.m_real_poses );
		}

		// remove model sway.
		g_cl.m_real_layers[ 12 ].m_weight = 0.f;

		// make sure to only animate once per tick.
		g_cl.m_animate = false;
	}

	// update our layers and poses with the saved ones.
	g_cl.m_local->SetAnimLayers( g_cl.m_real_layers );
	g_cl.m_local->SetPoseParameters( g_cl.m_real_poses );

	// update our real rotation.
	g_cl.m_local->SetAbsAngles( g_cl.m_rotation );

	// build bones.
	GenerateMatrix( g_cl.m_local, g_cl.m_real_matrix, 0x7FF00, g_cl.m_local->m_flSimulationTime( ) );
}

void Animations::UpdateFakeAnimations( ) {
	if( !g_cl.m_cmd || !g_cl.m_processing )
		return;

	if( !g_csgo.m_cl )
		return;

	bool allocate = ( g_cl.m_fake_state == nullptr );
	bool change = ( !allocate ) && ( g_cl.m_local->GetRefEHandle( ) != g_cl.m_ent_handle );
	bool reset = ( !allocate && !change ) && ( g_cl.m_local->m_flSpawnTime( ) != g_cl.m_spawn_time );

	// player changed, free old animation state.
	if( change && g_cl.m_fake_state_allocated)
		g_csgo.m_mem_alloc->Free( g_cl.m_fake_state_allocated );

	// need to reset? (on respawn)
	if( reset ) {
		// reset animation state.
		game::ResetAnimationState( g_cl.m_fake_state );

		// note new spawn time.
		g_cl.m_spawn_time = g_cl.m_local->m_flSpawnTime( );
	}

	// need to allocate or create new due to player change.
	if( allocate || change ) {
		// only works with games heap alloc.
		g_cl.m_fake_state_allocated = ( CCSGOPlayerAnimState* )g_csgo.m_mem_alloc->Alloc( sizeof( CCSGOPlayerAnimState ) );

		if( g_cl.m_fake_state_allocated != nullptr )
			game::CreateAnimationState( g_cl.m_local, g_cl.m_fake_state_allocated );

		// used to detect if we need to recreate / reset.
		g_cl.m_ent_handle = g_cl.m_local->GetRefEHandle( );
		g_cl.m_spawn_time = g_cl.m_local->m_flSpawnTime( );

		// note anim state for future use.
		g_cl.m_fake_state = g_cl.m_fake_state_allocated;
	}
	else {
		// make sure our state isn't null.
		if( !g_cl.m_fake_state )
			return;

		// allow re-animating in the same frame.
		g_cl.m_fake_state->m_iLastClientSideAnimationUpdateFramecount = 0.f;

		// update anim update delta as server would.
		g_cl.m_fake_state->m_flAnimUpdateDelta = fmaxf( g_csgo.m_globals->m_curtime - g_cl.m_fake_state->m_flLastClientSideAnimationUpdateTime, 0.0 );

		// update fake animations per tick.
		if( g_cl.m_update_fake ) {
			// update fake animations when we send commands.
			if( !g_csgo.m_cl->m_choked_commands ) {

				// update fake animation state.
				game::UpdateAnimationState( g_cl.m_fake_state, g_cl.m_angle );

				g_cl.m_local->GetAnimLayers( g_cl.m_fake_layers );
				g_cl.m_local->GetPoseParameters( g_cl.m_fake_poses );
			}

			// remove model sway.
			g_cl.m_fake_layers[ 12 ].m_weight = 0.f;

			// make sure to only animate once per tick.
			g_cl.m_update_fake = false;
		}

		// note - alpha; 
		// you don't need to set animation layers/poses to the fake ones here 
		// since they will already be fake, as UpdateAnimationState called 
		// UpdateClientsideAnimations - And since this is being called before
		// we update local player angles/layers, it will get overwritten by them
		// in the end.

		// replace current animation layers and poses with the fake ones.
		g_cl.m_local->SetAnimLayers( g_cl.m_fake_layers );
		g_cl.m_local->SetPoseParameters( g_cl.m_fake_poses );

		// replace the model rotation and build a matrix with our fake data.
		g_cl.m_local->SetAbsAngles( ang_t( 0.f, g_cl.m_fake_state->m_flGoalFeetYaw, 0.f ) );

		// generate a fake matrix.
		GenerateMatrix( g_cl.m_local, g_cl.m_fake_matrix, 0x7FF00, g_cl.m_local->m_flSimulationTime( ) );

		// fix interpolated model.
		for( auto& i : g_cl.m_fake_matrix ) {
			i[ 0 ][ 3 ] -= g_cl.m_local->GetRenderOrigin( ).x;
			i[ 1 ][ 3 ] -= g_cl.m_local->GetRenderOrigin( ).y;
			i[ 2 ][ 3 ] -= g_cl.m_local->GetRenderOrigin( ).z;
		}

		// revert our layers and poses back.
		g_cl.m_local->SetAnimLayers( g_cl.m_real_layers );
		g_cl.m_local->SetPoseParameters( g_cl.m_real_poses );

		// replace the model rotation with the proper rotation.
		g_cl.m_local->SetAbsAngles( ang_t( 0.f, g_cl.m_rotation.y, 0.f ) );
	}
}

Animations::AnimationInfo_t* Animations::GetAnimationInfo( Player* pEntity ) {
	auto pInfo = Animations::m_ulAnimationInfo.find( pEntity->GetRefEHandle( ) );
	if( pInfo == Animations::m_ulAnimationInfo.end( ) ) {
		return nullptr;
	}

	return &pInfo->second;
}