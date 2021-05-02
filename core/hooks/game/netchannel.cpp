#include "../../../includes.h"

int Hooks::SendDatagram( void* data ) {
	if( !g_csgo.m_net )
		return g_hooks.m_net_channel.GetOldMethod< SendDatagram_t >( INetChannel::SENDDATAGRAM )( this, data );

	int backup1 = g_csgo.m_net->m_in_rel_state;
	int backup2 = g_csgo.m_net->m_in_seq;

	if( g_cfg[ XOR( "rage_extended_backtrack" ) ].get<bool>( ) ) {
		// the target latency.
		float correct = std::max( 0.f, ( 200.f / 1000.f ) - g_cl.m_latency - g_cl.m_lerp );

		g_csgo.m_net->m_in_seq += 2 * ( 64 - 3 ) - static_cast< uint32_t >( ( 64 - 3 ) * correct );
	}

	int ret = g_hooks.m_net_channel.GetOldMethod< SendDatagram_t >( INetChannel::SENDDATAGRAM )( this, data );

	g_csgo.m_net->m_in_rel_state = backup1;
	g_csgo.m_net->m_in_seq = backup2;

	return ret;
}

bool Hooks::SendNetMsg( INetMessage& msg, bool bForceReliable, bool bVoice )
{
	if( !g_csgo.m_net )
		return g_hooks.m_net_channel.GetOldMethod<SendNetMsg_t>( INetChannel::SENDNETMSG )( this, msg, bForceReliable, bVoice );

	if( g_cfg[ XOR( "misc_bypass_sv_pure" ) ].get<bool>( ) ) {
		if( msg.GetType( ) == 14 ) // Return and don't send messsage if its FileCRCCheck
			return false;
	}

	if( msg.GetGroup( ) == 9 ) { // Fix lag when transmitting voice and fakelagging
		bVoice = true;
	}

	return g_hooks.m_net_channel.GetOldMethod<SendNetMsg_t>( INetChannel::SENDNETMSG )( this, msg, bForceReliable, bVoice );
}

void Hooks::ProcessPacket( void* packet, bool header ) {
	if( !g_csgo.m_net )
		return g_hooks.m_net_channel.GetOldMethod< ProcessPacket_t >( INetChannel::PROCESSPACKET )( this, packet, header );

	g_hooks.m_net_channel.GetOldMethod< ProcessPacket_t >( INetChannel::PROCESSPACKET )( this, packet, header );

	g_cl.UpdateIncomingSequences( );

	// get this from CL_FireEvents string "Failed to execute event for classId" in engine.dll
	for( CEventInfo* it{ g_csgo.m_cl->m_events }; it != nullptr; it = it->m_next ) {
		if( !it->m_class_id )
			continue;

		// set all delays to instant.
		it->m_fire_delay = 0.f;
	}

	// game events are actually fired in OnRenderStart which is WAY later after they are received
	// effective delay by lerp time, now we call them right after theyre received (all receive proxies are invoked without delay).
	//g_cl.m_allow_fire_events = true;
	g_csgo.m_engine->FireEvents( );
}