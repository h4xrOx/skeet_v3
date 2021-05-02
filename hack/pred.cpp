#include "../includes.h"
#include "pred.h"

EnginePrediction g_inputpred{ };;

void EnginePrediction::UpdatePrediction( ) {
	bool        valid{ g_csgo.m_cl->m_delta_tick > 0 };

	if( m_stored_variables.m_flVelocityModifier < 1.0 ) {
		*reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( g_csgo.m_prediction + 0x24 ) ) = 1;
	}

	// render start was not called.
	if( g_cl.m_stage == FRAME_NET_UPDATE_END ) {
		/*outgoing_command = g_csgo.m_cl->m_last_outgoing_command + g_csgo.m_cl->m_choked_commands;

		// this must be done before update ( update will mark the unpredicted commands as predicted ).
		for( int i{}; ; ++i ) {
			current_command = g_csgo.m_cl->m_last_command_ack + i;

			// caught up / invalid.
			if( current_command > outgoing_command || i >= MULTIPLAYER_BACKUP )
				break;

			// get command.
			cmd = g_csgo.m_input->GetUserCmd( current_command );
			if( !cmd )
				break;

			// cmd hasn't been predicted.
			// m_nTickBase is incremented inside RunCommand ( which is called frame by frame, we are running tick by tick here ) and prediction hasn't run yet,
			// so we must fix tickbase by incrementing it ourselves on non-predicted commands.
			if( !cmd->m_predicted )
				++g_cl.m_local->m_nTickBase( );
		}*/

		// EDIT; from what ive seen RunCommand is called when u call Prediction::Update
		// so the above code is not fucking needed.

		int start = g_csgo.m_cl->m_last_command_ack;
		int stop = g_csgo.m_cl->m_last_outgoing_command + g_csgo.m_cl->m_choked_commands;

		// call CPrediction::Update.
		g_csgo.m_prediction->Update( g_csgo.m_cl->m_delta_tick, valid, start, stop );
	}
}

void EnginePrediction::Predict( ) {
	static CMoveData data{ };

	g_csgo.m_prediction->m_in_prediction = true;

	// CPrediction::StartCommand
	g_cl.m_local->m_pCurrentCommand( ) = g_cl.m_cmd;
	g_cl.m_local->m_PlayerCommand( ) = *g_cl.m_cmd;

	*g_csgo.m_nPredictionRandomSeed = g_cl.m_cmd->m_random_seed;
	g_csgo.m_pPredictionPlayer = g_cl.m_local;

	// backup globals.
	m_stored_variables.m_flCurtime = g_csgo.m_globals->m_curtime;
	m_stored_variables.m_flFrametime = g_csgo.m_globals->m_frametime;

	// CPrediction::RunCommand

	// set globals appropriately.
	g_csgo.m_globals->m_curtime = game::TICKS_TO_TIME( g_cl.m_local->m_nTickBase( ) );
	g_csgo.m_globals->m_frametime = g_csgo.m_prediction->m_engine_paused ? 0.f : g_csgo.m_globals->m_interval;

	// set target player ( host ).
	g_csgo.m_move_helper->SetHost( g_cl.m_local );
	g_csgo.m_game_movement->StartTrackPredictionErrors( g_cl.m_local );

	// setup input.
	g_csgo.m_prediction->SetupMove( g_cl.m_local, g_cl.m_cmd, g_csgo.m_move_helper, &data );

	// run movement.
	g_csgo.m_game_movement->ProcessMovement( g_cl.m_local, &data );
	g_csgo.m_prediction->FinishMove( g_cl.m_local, g_cl.m_cmd, &data );
	g_csgo.m_game_movement->FinishTrackPredictionErrors( g_cl.m_local );

	// reset target player ( host ).
	g_csgo.m_move_helper->SetHost( nullptr );
}

void EnginePrediction::Restore( ) {
	if (!g_cl.m_cmd || !g_cl.m_local)
		return;

	g_csgo.m_prediction->m_in_prediction = false;

	*g_csgo.m_nPredictionRandomSeed = -1;
	g_csgo.m_pPredictionPlayer = nullptr;

	// restore globals.
	g_csgo.m_globals->m_curtime = m_stored_variables.m_flCurtime;
	g_csgo.m_globals->m_frametime = m_stored_variables.m_flFrametime;
}

void EnginePrediction::CorrectViewmodelData( ) {
	if( !g_cl.m_processing ) {
		return;
	}

	if( g_cl.m_local->m_hViewModel( ) != 0xFFFFFFFF ) {
		Entity* const pViewModel = g_csgo.m_entlist->GetClientEntityFromHandle( g_cl.m_local->m_hViewModel( ) );

		if( !pViewModel ) {
			return;
		}

		pViewModel->m_flAnimTime( ) = StoredViewmodel.m_flViewmodelAnimTime;
		pViewModel->m_flCycle( ) = StoredViewmodel.m_flViewmodelCycle;
	}
}

void EnginePrediction::UpdateViewmodelData( ) {
	if( !g_cl.m_processing ) {
		return;
	}

	if( g_cl.m_local->m_hViewModel( ) != 0xFFFFFFFF ) {
		Entity* const pViewModel = g_csgo.m_entlist->GetClientEntityFromHandle( g_cl.m_local->m_hViewModel( ) );

		if( !pViewModel ) {
			return;
		}

		StoredViewmodel.m_flViewmodelCycle = pViewModel->m_flCycle( );
		StoredViewmodel.m_flViewmodelAnimTime = pViewModel->m_flAnimTime( );
	}
}