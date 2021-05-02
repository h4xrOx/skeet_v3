#include "../../../includes.h"

void Hooks::LevelInitPreEntity( const char* map ) {
	float rate{ 1.f / g_csgo.m_globals->m_interval };

	// set rates when joining a server.
	g_csgo.cl_updaterate->SetValue( rate );
	g_csgo.cl_cmdrate->SetValue( rate );

	g_aimbot.reset( );
	g_visuals.m_hit_start = g_visuals.m_hit_end = g_visuals.m_hit_duration = 0.f;

	/*if (g_cl.m_fake_state_allocated) {
		//	g_csgo.m_mem_alloc->Free( g_cl.m_fake_state_allocated );

		g_cl.m_fake_state_allocated = nullptr;
	}*/

	if( g_cl.m_fake_state ) {
		g_cl.m_fake_state = nullptr;
	}

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelInitPreEntity_t >( CHLClient::LEVELINITPREENTITY )( this, map );
}

void Hooks::LevelInitPostEntity( ) {
	g_cl.OnMapload( );

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelInitPostEntity_t >( CHLClient::LEVELINITPOSTENTITY )( this );
}

void Hooks::LevelShutdown( ) {
	g_cl.m_local = nullptr;
	g_cl.m_weapon = nullptr;
	g_cl.m_processing = false;
	g_cl.m_weapon_info = nullptr;
	g_cl.m_round_end = false;

	g_aimbot.reset( );

	g_cl.m_sequences.clear( );

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelShutdown_t >( CHLClient::LEVELSHUTDOWN )( this );
}

/*int Hooks::IN_KeyEvent( int evt, int key, const char* bind ) {
	// see if this key event was fired for the drop bind.
	/*if( bind && FNV1a::get( bind ) == HASH( "drop" ) ) {
		// down.
		if( evt ) {
			g_cl.m_drop = true;
			g_cl.m_drop_query = 2;
			g_cl.print( "drop\n" );
		}

		// up.
		else
			g_cl.m_drop = false;

		// ignore the event.
		return 0;
	}

	return g_hooks.m_client.GetOldMethod< IN_KeyEvent_t >( CHLClient::INKEYEVENT )( this, evt, key, bind );
}*/

void Hooks::FrameStageNotify( Stage_t stage ) {
	if( !g_csgo.m_engine->IsInGame( ) ) {
		return g_hooks.m_client.GetOldMethod< FrameStageNotify_t >( CHLClient::FRAMESTAGENOTIFY )( this, stage );
	}

	// save stage.
	if( stage != FRAME_START )
		g_cl.m_stage = stage;

	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );

	if( stage == FRAME_RENDER_START ) {
		// update information about local player.
		g_cl.UpdateAnimations( );

		// apply local player animated angles.
		g_anims.UpdateLocalAnimations( );

		// apply local player fake angles.		
		// g_anims.UpdateFakeAnimations();

		// draw our custom beams.
		g_visuals.DrawBeams( );

		// handle our shots.
		g_shots.OnFrameStage( );
	}

	static int m_iLastCmdAck = 0;
	static float m_flNextCmdTime = 0.f;

	if( g_cl.m_local ) {
		int framstage_minus2 = stage - 2;

		if( framstage_minus2 ) {
			// do shit onetap does idk
		}
		else {
			if( g_csgo.m_cl && ( m_iLastCmdAck != g_csgo.m_cl->m_last_command_ack || m_flNextCmdTime != g_csgo.m_cl->m_flNextCmdTime ) )
			{
				if( g_inputpred.m_stored_variables.m_flVelocityModifier != g_cl.m_local->get< float >( 0xA38C ) )
				{
					*reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( g_csgo.m_prediction + 0x24 ) ) = 1;
					g_inputpred.m_stored_variables.m_flVelocityModifier = g_cl.m_local->get< float >( 0xA38C );
				}

				m_iLastCmdAck = g_csgo.m_cl->m_last_command_ack;
				m_flNextCmdTime = g_csgo.m_cl->m_flNextCmdTime;
			}
		}
	}

	// call og.
	g_hooks.m_client.GetOldMethod< FrameStageNotify_t >( CHLClient::FRAMESTAGENOTIFY )( this, stage );

	if( stage == FRAME_RENDER_START ) {
		// get tickrate.
		g_cl.m_rate = ( int )std::round( 1.f / g_csgo.m_globals->m_interval );

		static bool needs_full_update = false;

		if( g_cl.m_local ) {
			if( !g_cl.m_local->alive( ) ) {
				needs_full_update = true;
			}
			else {
				if( needs_full_update ) {
					g_skins.m_update = true;
					needs_full_update = false;
				}
			}
		}
	}

	else if( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
		// restore non-compressed netvars.
		// g_netdata.apply( );

		// fix the viewmodel sequences.
		g_inputpred.CorrectViewmodelData( );

		// run our clantag changer.
		g_cl.ClanTag( );

		g_skins.think( );
	}

	else if( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END ) {
		g_visuals.NoSmoke( );
	}

	else if( stage == FRAME_NET_UPDATE_END ) {
		// restore non-compressed netvars.
		g_netdata.apply( );

		// update all players.
		for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );
			if( !player || player->m_bIsLocalPlayer( ) )
				continue;

			AimPlayer* data = &g_aimbot.m_players[ i - 1 ];
			data->OnNetUpdate( player );

			auto hViewModel = player->m_hViewModel( );
			if( hViewModel != 0xFFFFFFFF ) {
				auto pViewModel = g_csgo.m_entlist->GetClientEntityFromHandle<Entity*>( hViewModel );
				if( pViewModel ) {
					game::UpdateAllViewmodelAddons( pViewModel );
				}
			}
		}

		g_lagcompensation.PostPlayerUpdate( );
	}
}

bool __cdecl Detours::ReportHit( Hit_t* info ) {
	// add world hitmarkers
	if( info ) {
		WorldHitmarkerData_t data;

		data.m_alpha = 1.f;
		data.m_time = g_inputpred.m_stored_variables.m_flCurtime;

		data.m_pos_x = info->m_pos_x;
		data.m_pos_y = info->m_pos_y;
		data.m_pos_z = info->m_pos_z;

		g_visuals.m_world_hitmarkers.push_back( data );
	}
	return g_detours.oReportHit( info );
}

void __cdecl Detours::CL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	if (!g_hvh.m_fake_duck) {
		if (g_cl.m_processing && g_tickbase.m_shift_data.m_should_attempt_shift && g_tickbase.m_shift_data.m_needs_recharge) {
			--g_tickbase.m_shift_data.m_needs_recharge;

			//g_tickbase.m_shift_data.m_did_shift_before = false;

			if (g_tickbase.m_shift_data.m_needs_recharge == 0) {
				g_tickbase.m_shift_data.m_should_be_ready = true;
			}

			return;
		}
	}

	return g_detours.oCL_Move(accumulated_extra_samples, bFinalTick);
}
