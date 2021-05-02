#include "../includes.h"

LagComp g_lagcompensation{ };;

LagComp::LagRecord_t::LagRecord_t( Player* pEntity ) {
	const auto pWeapon = pEntity->GetActiveWeapon( );

	this->m_pEntity = pEntity;
	m_iEntIndex = pEntity->index( );
	m_bDormant = pEntity->dormant( );
	m_vecVelocity = pEntity->m_vecVelocity( );
	m_vecOrigin = pEntity->m_vecOrigin( );
	m_vecAbsOrigin = pEntity->GetAbsOrigin( );
	m_vecMins = pEntity->m_vecMins( );
	m_vecMaxs = pEntity->m_vecMaxs( );
	pEntity->GetAnimLayers( m_pLayers );
	pEntity->GetPoseParameters( m_pPoses );
	m_pState = pEntity->m_PlayerAnimState( ) ? pEntity->m_PlayerAnimState( ) : nullptr;
	m_flSimulationTime = pEntity->m_flSimulationTime( );
	m_flInterpTime = 0.f;
	m_flLastShotTime = pWeapon ? pWeapon->m_fLastShotTime( ) : 0.f;
	m_flDuck = pEntity->m_flDuckAmount( );
	m_flLowerBodyYawTarget = pEntity->m_flLowerBodyYawTarget( );
	m_angEyeAngles = pEntity->m_angEyeAngles( );
	m_angAbsAngles = pEntity->GetAbsAngles( );
	m_fFlags = pEntity->m_fFlags( );
	m_iEFlags = pEntity->m_iEFlags( );
	m_iEffects = pEntity->m_fEffects( );
	m_iChoked = game::TIME_TO_TICKS( pEntity->m_flSimulationTime( ) - pEntity->m_flOldSimulationTime( ) );

	m_iChoked = std::clamp( m_iChoked, 0, 17 );

	m_bValid = m_iChoked >= 0 && m_iChoked <= 17;
}

LagComp::LagRecord_t::LagRecord_t( Player* pEntity, ang_t vecLastReliableAngle ) : LagRecord_t( pEntity ) {
	this->m_angLastReliableAngle = vecLastReliableAngle;
}

bool LagComp::LagRecord_t::IsValid( float flSimulationTime, bool bValid, float flRange ) {
	if( !g_csgo.m_net || !bValid )
		return false;

	static auto sv_maxunlag = g_csgo.m_cvar->FindVar( HASH( "sv_maxunlag" ) );

	const auto flCorrect = std::clamp( g_csgo.m_net->GetLatency( INetChannel::FLOW_INCOMING )
		+ g_csgo.m_net->GetLatency( INetChannel::FLOW_OUTGOING )
		+ g_cl.m_lerp, 0.f, sv_maxunlag->GetFloat( ) );

	return fabsf( flCorrect - ( g_csgo.m_globals->m_curtime - flSimulationTime ) ) <= flRange;
}

void LagComp::LagRecord_t::BuildBones( Player* player, bool resolve) {
	const auto backup_occlusion_flags = player->m_iOcclusionFlags( );
	const auto backup_occlusion_framecount = player->m_iOcclusionFramecount( );

	g_anims.m_bEnablePVS = true;

	player->m_iOcclusionFlags( ) = 0;
	player->m_iOcclusionFramecount( ) = 0;

	//player->m_BoneAccessor( ).m_ReadableBones = player->m_BoneAccessor( ).m_WritableBones = 0;

	player->GetReadableBones() = player->GetWriteableBones() = 0;

	player->m_iMostRecentModelBoneCounter( ) = 0;
	player->m_flLastBoneSetupTime( ) = -FLT_MAX;

	player->m_fEffects() |= 8;

	if (!resolve) {
		player->SetupBones(m_pMatrix, 128, 0x7FF00, m_flSimulationTime);
	}
	else {
		const auto backup_bone = player->GetBoneArrayForWrite();
		player->GetBoneArrayForWrite() = m_pMatrix_Resolved;

		player->SetupBones(nullptr, -1, 0x7FF00, m_flSimulationTime);

		player->GetBoneArrayForWrite() = backup_bone;
	}

	player->m_iOcclusionFlags( ) = backup_occlusion_flags;

	player->m_iOcclusionFramecount( ) = backup_occlusion_framecount;

	g_anims.m_bEnablePVS = false;

	player->m_fEffects() &= ~8;
}

void LagComp::LagRecord_t::Restore( Player* player ) {
	player->m_vecVelocity( ) = m_vecVelocity;
	player->m_fFlags( ) = m_fFlags;
	player->m_iEFlags( ) = m_iEFlags;
	player->m_flDuckAmount( ) = m_flDuck;
	player->SetAnimLayers( m_pLayers );
	player->m_flLowerBodyYawTarget( ) = m_flLowerBodyYawTarget;
	player->m_vecOrigin( ) = m_vecOrigin;
	player->SetAbsOrigin( m_vecAbsOrigin );
}

void LagComp::LagRecord_t::Apply( Player* player ) {
	player->SetPoseParameters( m_pPoses ) ;
	player->m_angEyeAngles( ) = m_angEyeAngles;
	player->m_vecVelocity( ) = player->m_vecAbsVelocity( ) = m_vecVelocity;
	player->m_flLowerBodyYawTarget( ) = m_flLowerBodyYawTarget;
	player->m_flDuckAmount( ) = m_flDuck;
	player->m_fFlags( ) = m_fFlags;
	player->m_vecOrigin( ) = m_vecOrigin;
	player->SetAbsOrigin( m_vecOrigin );
	if( player->m_PlayerAnimState( ) ) {
		player->SetPlayerAnimState( m_pState );
	}
}


void LagComp::PostPlayerUpdate( ) {
	if( !g_cl.m_local ) {
		if( g_anims.m_ulAnimationInfo.size( ) ) {
			g_anims.m_ulAnimationInfo.clear( );
		}

		return;
	}

	for( auto it = g_anims.m_ulAnimationInfo.begin( ); it != g_anims.m_ulAnimationInfo.end( );) {
		const auto handle = it->first;

		Player* const player = g_csgo.m_entlist->GetClientEntityFromHandle< Player* >( handle );

		if( !player || player != it->second.m_pEntity || !player->alive( ) || !player->enemy( g_cl.m_local ) ) {
			if( player ) {
				player->m_bClientSideAnimation( ) = true;
			}
			
			it = g_anims.m_ulAnimationInfo.erase( it );
		}
		else {
			it = next( it );
		}
	}

	for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
		Player* const player = g_csgo.m_entlist->GetClientEntity< Player* >( i );
		if( !player )
			continue;

		if( !player->enemy( g_cl.m_local ) && !player->m_bIsLocalPlayer( ) )
			player->m_bClientSideAnimation( ) = true;

		if( !player->alive( ) || !player->enemy( g_cl.m_local ) )
			continue;

		if( g_anims.m_ulAnimationInfo.find(player->GetRefEHandle()) == g_anims.m_ulAnimationInfo.end( ) )
			g_anims.m_ulAnimationInfo.insert_or_assign(player->GetRefEHandle(), Animations::AnimationInfo_t( player, { } ) );
	}

	for( auto& ulAnimInfo : g_anims.m_ulAnimationInfo ) {
		Animations::AnimationInfo_t& pRecord = ulAnimInfo.second;
		Player* const pEntity = pRecord.m_pEntity;

		for( auto i = pRecord.m_pRecords.rbegin( ); i != pRecord.m_pRecords.rend( );) {
			if( g_csgo.m_globals->m_curtime - i->m_flSimulationTime > 1.2f )
				i = decltype( i ) { ulAnimInfo.second.m_pRecords.erase( next( i ).base( ) ) };
			else
				i = next( i );
		}

		if( pEntity->m_flSimulationTime( ) == pEntity->m_flOldSimulationTime( ) )
			continue;

		CCSGOPlayerAnimState* const state = pEntity->m_PlayerAnimState();

		if( pRecord.m_flLastSpawnTime != pEntity->m_flSpawnTime( ) ) {
			if( state ) {
				game::ResetAnimationState( state );
			}

			pRecord.m_flLastSpawnTime = pEntity->m_flSpawnTime( );
		}

		const auto pWeapon = pEntity->GetActiveWeapon( );

		LagComp::LagRecord_t pBackupRecord = LagComp::LagRecord_t( pEntity );
		pBackupRecord.Apply( pEntity );

		LagComp::LagRecord_t* pPreviousRecord = nullptr;

		if( !pRecord.m_pRecords.empty( ) && !pRecord.m_pRecords.front( ).m_bDormant && game::TIME_TO_TICKS( pEntity->m_flSimulationTime( ) - pRecord.m_pRecords.front( ).m_flSimulationTime ) <= 17 ) {
			pPreviousRecord = &pRecord.m_pRecords.front( );
			pRecord.m_PreviousRecord = pRecord.m_pRecords.front( );
		}

		const bool bShot = pWeapon && pPreviousRecord && pWeapon->m_fLastShotTime( ) > pPreviousRecord->m_flSimulationTime && pWeapon->m_fLastShotTime( ) <= pEntity->m_flSimulationTime( );

		if( !bShot ) {
			ulAnimInfo.second.m_vecLastReliableAngle = pEntity->m_angEyeAngles( );
		}

		auto& pCurrentRecord = pRecord.m_pRecords.emplace_front( pEntity, ulAnimInfo.second.m_vecLastReliableAngle );

		pRecord.UpdateAnimations( &pCurrentRecord, pPreviousRecord );

		pEntity->SetAnimLayers( pBackupRecord.m_pLayers );

/*		float real_poses[24];
		float backup_poses[24];
		const auto backup_abs = pEntity->GetAbsAngles();
		pEntity->GetPoseParameters(real_poses);

		//real_poses[MOVE_BLEND_CROUCH] = state->m_flDuckAmount;
		//real_poses[MOVE_BLEND_RUN] = (1.0f - state->m_flStopToFullRunningFraction) * (1.0f - state->m_flDuckAmount);
		//real_poses[MOVE_BLEND_WALK] = (state->m_flStopToFullRunningFraction) * (1.0f - state->m_flDuckAmount);

		real_poses[BODY_YAW] = 0.5f;
		pEntity->SetPoseParameters(real_poses);
		pEntity->SetAbsAngles(ang_t(0.f, state->m_flEyeYaw, 0.f));

		pCurrentRecord.BuildBones(pEntity, false); 

		pEntity->SetAbsAngles(backup_abs);
		pEntity->SetPoseParameters(backup_poses);*/

		pCurrentRecord.BuildBones(pEntity, true);

		pBackupRecord.Restore( pEntity );

		pRecord.m_LatestRecord = pCurrentRecord;
	}
}

std::optional<LagComp::LagRecord_t*> LagComp::GetLatestRecord( Player* pEntity ) {
	const auto pInfo = g_anims.m_ulAnimationInfo.find( pEntity->GetRefEHandle( ) );
	if( pInfo == g_anims.m_ulAnimationInfo.end( ) || pInfo->second.m_pRecords.empty( ) ) {
		return std::nullopt;
	}

	LagRecord_t* first_invalid = nullptr;

	for( auto it = pInfo->second.m_pRecords.begin( ); it != pInfo->second.m_pRecords.end( ); it = next( it ) ) {

		if (!first_invalid)
			first_invalid = &*it;

		if (it->IsValid(it->m_flSimulationTime, it->m_bValid)) {
			return &*it;
		}
	}

	if (first_invalid)
		return first_invalid;
	else
		return std::nullopt;
}

std::optional<LagComp::LagRecord_t*> LagComp::GetOldestRecord( Player* pEntity ) {
	const auto pInfo = g_anims.m_ulAnimationInfo.find( pEntity->GetRefEHandle( ) );
	if( pInfo == g_anims.m_ulAnimationInfo.end( ) || pInfo->second.m_pRecords.empty( ) ) {
		return std::nullopt;
	}

	for( auto it = pInfo->second.m_pRecords.rbegin( ); it != pInfo->second.m_pRecords.rend( ); it = next( it ) ) {
		if( it->IsValid( it->m_flSimulationTime, it->m_bValid ) ) {
			return &*it;
		}
	}

	return std::nullopt;
}

std::optional<std::pair<LagComp::LagRecord_t*, LagComp::LagRecord_t*>> LagComp::GetViableRecords( Player* pEntity, const float flRange ) {
	const auto pInfo = g_anims.m_ulAnimationInfo.find( pEntity->GetRefEHandle( ) );
	if( pInfo == g_anims.m_ulAnimationInfo.end( ) || pInfo->second.m_pRecords.empty( ) ) {
		return std::nullopt;
	}

	for( auto it = pInfo->second.m_pRecords.begin( ); it != pInfo->second.m_pRecords.end( ); it = next( it ) ) {
		if( it->IsValid( it->m_flSimulationTime, it->m_bValid, flRange * .2f ) && it + 1 != pInfo->second.m_pRecords.end( )
			&& !( it + 1 )->IsValid( ( it + 1 )->m_flSimulationTime, ( it + 1 )->m_bValid, flRange * .2f ) ) {
			return std::make_pair( &*( it + 1 ), &*it );
		}
	}

	return std::nullopt;
}

std::vector<LagComp::LagRecord_t*> LagComp::GetValidRecords( Player* pEntity, const float flRange ) {
	std::vector<LagRecord_t*> vecFinalRecord;

	const auto pInfo = g_anims.m_ulAnimationInfo.find( pEntity->GetRefEHandle( ) );

	if( pInfo == g_anims.m_ulAnimationInfo.end( ) || pInfo->second.m_pRecords.empty( ) )
		return vecFinalRecord;

	vecFinalRecord.reserve( static_cast< int >( std::ceil( flRange * .2f / g_csgo.m_globals->m_interval ) ) );

	for( auto it = pInfo->second.m_pRecords.begin( ); it != pInfo->second.m_pRecords.end( ); it = next( it ) ) {
		if( it->IsValid( it->m_flSimulationTime, it->m_bValid, flRange * .2f ) )
			vecFinalRecord.push_back( &*it );
	}

	return vecFinalRecord;
}