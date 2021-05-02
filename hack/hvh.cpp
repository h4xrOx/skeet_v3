#include "../includes.h"

HVH g_hvh{ };;

void HVH::AntiAim( ) {
	if( !g_cfg[ XOR( "rage_aa_enabled" ) ].get<bool>( ) )
		return;

	if( !g_cl.m_cmd )
		return;

	if( !g_cl.m_processing )
		return;

	// don't fucking use anti-aim when noclipping or on ladder ?!??!
	if( ( g_cl.m_local->m_MoveType( ) == MOVETYPE_NOCLIP ||
		g_cl.m_local->m_MoveType( ) == MOVETYPE_LADDER ) && g_cl.m_pressing_move ) {
		return;
	}

	// make sure we have a valid weapon.
	auto weapon = g_cl.m_weapon;
	if( !weapon )
		return;

	// are we throwing a nade?
	// if so then fuck off don't do anti aim
	if( weapon->IsGrenade( ) )
		if( weapon->m_fThrowTime( ) > 0 )
			return;

	// don't do anti-aim if we're in use.
	if( g_cl.m_cmd->m_buttons & IN_USE )
		return;

	// don't do anti-aim if we're firing.
	if(g_cl.IsFiring(game::TICKS_TO_TIME(g_cl.m_local->m_nTickBase())) )
		return;

	// don't do anti-aim on the freeze period, if we're frozen or on round end.
	if( g_csgo.m_gamerules->m_bFreezePeriod( ) || ( g_cl.m_flags & FL_FROZEN ) || g_cl.m_round_end )
		return;

	// load settings.
	m_pitch = g_cfg[ XOR( "rage_aa_pitch" ) ].get<int>( );
	m_yaw = g_cfg[ XOR( "rage_aa_yaw" ) ].get<int>( );
	m_yaw_fake = g_cfg[ XOR( "rage_aa_yaw_fake" ) ].get<int>( );
	m_jitter_range = g_cfg[ XOR( "rage_aa_jitter_range" ) ].get<float>( );
	m_rot_range = g_cfg[ XOR( "rage_aa_rotation_range" ) ].get<float>( );
	m_rot_speed = g_cfg[ XOR( "rage_aa_rotation_speed" ) ].get<float>( );
	m_rand_update = g_cfg[ XOR( "rage_aa_random_update" ) ].get<float>( );
	m_dir = g_cfg[ XOR( "rage_aa_yaw_direction" ) ].get<int>( );
	m_base_angle = g_cfg[ XOR( "rage_aa_yaw_base" ) ].get<int>( );
	m_auto_time = g_cfg[ XOR( "rage_aa_yaw_auto_time" ) ].get<float>( );

	// update jitter.
	UpdateJitter();

	// do at target anti-aim.
	AtTarget( );

	// prepare the fake angle.
	PrepareFake( );

	// adjust pitch.
	AntiAimPitch( );

	// adjust yaw.
	AdjustYaw( );
}

void HVH::IdealPitch( ) {
	CCSGOPlayerAnimState* state = g_cl.m_local->m_PlayerAnimState( );
	if( !state )
		return;

	g_cl.m_cmd->m_view_angles.x = state->m_flMinPitch;
}

void HVH::AntiAimPitch( ) {
	bool untrusted = g_cfg[ XOR( "cheat_mode" ) ].get<int>( ) == 1;

	switch( m_pitch ) {
	case 1:
		// down.
		g_cl.m_cmd->m_view_angles.x = untrusted ? -540.f : 89.f;
		break;

	case 2:
		// up.
		g_cl.m_cmd->m_view_angles.x = untrusted ? 540.f : -89.f;
		break;

	case 3:
		// random.
		g_cl.m_cmd->m_view_angles.x = untrusted ? g_csgo.RandomFloat( -540.f, 540.f ) : g_csgo.RandomFloat( -89.f, 89.f );
		break;

	case 4:
		// ideal.
		IdealPitch( );
		break;

	default:
		break;
	}
}

void HVH::PrepareFake( ) {
	if (g_cl.m_ground) {
		if ((g_cfg[XOR("rage_aa_real_around_fake_standing")].get<bool>() && g_cl.m_speed < 3.5f ) || (g_cfg[XOR("rage_aa_real_around_fake_moving")].get<bool>() && g_cl.m_speed > 3.5f && !g_movement.m_slow_motion) || (g_cfg[XOR("rage_aa_real_around_fake_slow_motion")].get<bool>() && g_movement.m_slow_motion)) {
			m_use_real_around_fake = true;
		}
		else {
			m_use_real_around_fake = false;
		}
	}
	else {
		if (g_cfg[XOR("rage_aa_real_around_fake_air")].get<bool>()) {
			m_use_real_around_fake = true;
		}
		else {
			m_use_real_around_fake = false;
		}
	}

	// move in the sent commands.
	if( !g_csgo.m_cl->m_choked_commands ) {
		// make sure we don't move if we are wanting to move manually.
		if( !g_cl.m_pressing_move ) {
			static bool negate = false;
			const float velocity = g_cl.m_local->m_flDuckAmount() ? 3.25f : 1.01f;

			// set sidemove to make sure lby is always being updated.
			// only do this if we aren't pressing any move keys, or
			// else we won't be able to move and stuff wont go nicely.

			g_cl.m_cmd->m_side_move = negate ? velocity : -velocity;

			// move in the negative direction next tick.
			negate = !negate;
		}
	}

	// set lby angle in the choked commands.
	else if( !*g_cl.m_packet ) {
		// we can just set our angles to 180, since
		// the server will end up clamping our shit anyways.
		if (!m_use_real_around_fake) {
			g_cl.m_cmd->m_view_angles.y += 180.f;
		}
		else {
			float yaw = 120.f;
			if (g_cfg[XOR("rage_aa_real_around_fake_jitter")].get<bool>()) {
				if (g_cl.m_local && g_csgo.m_cl) {
					yaw = m_jitter_update ? -yaw : yaw;
				}
			}

			g_cl.m_cmd->m_view_angles.y += g_cl.m_negate_desync ? -yaw : yaw;
		}
	}
}

float HVH::ChooseReal( ) {
	static float flReturnValue = 0.f;


	switch (m_dir) {
	case 0:
		m_direction = 180.f;
		break;
	case 1:
		m_direction = 90.f;
		break;
	case 2:
		m_direction = -90.f;
		break;

	default:
		m_direction = 0.f;
	}

	// if we have a yaw antaim.
	if( m_yaw > 0 ) {
		// do real anti-aim.
		switch( m_yaw ) {

			// backwards.
		case 1:
			flReturnValue = m_direction;
			break;

			  // random.
		case 2:
			// check update time.
			if( g_csgo.m_globals->m_curtime >= m_next_random_update ) {

				// set new random angle.
				m_random_angle = g_csgo.RandomFloat( -180.f, 180.f );

				// set next update time
				m_next_random_update = g_csgo.m_globals->m_curtime + m_rand_update;
			}

			// apply angle.
			flReturnValue = m_random_angle;
			break;

		default:
			break;
		}

		if (g_cfg[XOR("rage_aa_jitter_type")].get<int>() > 0) {
			// get the range from the menu.
			float range = m_jitter_range / 2.f;

			// offset.
			if (g_cfg[XOR("rage_aa_jitter_type")].get<int>() == 1) {
				flReturnValue += m_jitter_update ? -range : range;
			}
			// random.
			else {
				// set angle.
				flReturnValue += g_csgo.RandomFloat(-range, range);
			}
		}

		if (g_cfg[XOR("rage_aa_rotation_enabled")].get<bool>()) {
			// apply spin.
			flReturnValue = (flReturnValue - m_rot_range / 2.f) + std::fmod(g_csgo.m_globals->m_curtime * (m_rot_speed * 20.f), m_rot_range);
		}
	}

	return flReturnValue;
}

void HVH::AdjustYaw( ) {
	float wish_real_angle = ChooseReal( );
	float custom_fake = std::clamp<float>( g_cfg[ XOR( "rage_aa_custom_fake" ) ].get<float>( ), -60.f, 60.f );
	float wish_desync = g_cl.m_negate_desync ? custom_fake : -custom_fake;

	// fake around real.
	if (!m_use_real_around_fake) {
		// do real anti-aim.
		if (*g_cl.m_packet) {
			g_cl.m_cmd->m_view_angles.y += wish_real_angle;
		}
		// do fake anti-aim.
		else {
			g_cl.m_cmd->m_view_angles.y += wish_real_angle + wish_desync;
		}
	}
	// real around fake.
	else {
		if (*g_cl.m_packet) {
			g_cl.m_cmd->m_view_angles.y += wish_real_angle;
		}
		else {
			g_cl.m_cmd->m_view_angles.y += wish_real_angle;
		}
	}
}
	
void HVH::AtTarget( ) {
	if( !m_base_angle )
		return;
	
	float  best_fov{ std::numeric_limits< float >::max( ) };
	float  best_dist{ std::numeric_limits< float >::max( ) };
	float  fov, dist;
	Player* target, * best_target{ nullptr };

	for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
		target = g_csgo.m_entlist->GetClientEntity< Player* >( i );

		if( !g_aimbot.IsValidTarget( target ) )
			continue;

		if( target->dormant( ) )
			continue;

		// 'away crosshair'.
		if( m_base_angle == 1 ) {

			// check if a player was closer to our crosshair.
			fov = math::GetFOV( g_cl.m_view_angles, g_cl.m_shoot_pos, target->WorldSpaceCenter( ) );
			if( fov < best_fov ) {
				best_fov = fov;
				best_target = target;
			}
		}

		// 'away distance'.
		else if( m_base_angle == 2 ) {

			// check if a player was closer to us.
			dist = ( target->m_vecOrigin( ) - g_cl.m_local->m_vecOrigin( ) ).length_sqr( );
			if( dist < best_dist ) {
				best_dist = dist;
				best_target = target;
			}
		}
	}

	if( best_target ) {
		// todo - dex; calculate only the yaw needed for this (if we're not going to use the x component that is).
		ang_t angle;
		math::VectorAngles( best_target->m_vecOrigin( ) - g_cl.m_local->m_vecOrigin( ), angle );
		g_cl.m_cmd->m_view_angles.y = angle.y;
	}
}

void HVH::UpdateJitter()
{
	if (!g_cl.m_processing)
		return;

	const auto jitterSpeed = 1 + g_csgo.m_cl->m_choked_commands;

	static int lastTick = 0;
	static auto returnValue = 0.f;
	if (lastTick + jitterSpeed < g_cl.m_local->m_nTickBase() || lastTick > g_cl.m_local->m_nTickBase())
	{
		lastTick = g_cl.m_local->m_nTickBase();

		m_jitter_update = !m_jitter_update;
	}
}

void HVH::SendPacket( ) {
	// fake-lag enabled.
	if( g_cfg[ XOR( "rage_fakelag_enabled" ) ].get<bool>( ) && !g_csgo.m_gamerules->m_bFreezePeriod( ) && !( g_cl.m_flags & FL_FROZEN ) ) {
		// limit of lag.
		int limit = std::clamp<int>( ( int )g_cfg[ XOR( "rage_fakelag_limit" ) ].get<float>( ), 2, g_cl.m_max_lag );
		int mode = g_cfg[ XOR( "rage_fakelag_type" ) ].get<int>( );

		// reset this here everytime..
		g_cl.m_should_lag = false;

		// get current origin.
		vec3_t cur = g_cl.m_local->m_vecOrigin( );

		// get prevoius origin.
		vec3_t prev = g_cl.m_net_pos.empty( ) ? g_cl.m_local->m_vecOrigin( ) : g_cl.m_net_pos.front( ).m_pos;

		// delta between the current origin and the last sent origin.
		float delta = ( cur - prev ).length_sqr( );

		// stand.
		if( delta < 0.1 ) {
			g_cl.m_should_lag = true;

			if (!g_cfg[XOR("rage_fakelag_standing_enabled")].get<bool>())
				limit = 2;
		} 

		// move.
		if( g_cfg[ XOR( "rage_fakelag_move_enabled" ) ].get<bool>( ) && delta > 0.1f && g_cl.m_speed > 0.1f && !m_fake_duck ) {
			g_cl.m_should_lag = true;
		}

		// air.
		else if( g_cfg[ XOR( "rage_fakelag_air_enabled" ) ].get<bool>( ) && !m_fake_duck && ( ( g_cl.m_buttons & IN_JUMP ) || !( g_cl.m_flags & FL_ONGROUND ) ) ) {
			g_cl.m_should_lag = true;
		}

		// crouch.
		else if( g_cfg[ XOR( "rage_fakelag_crouch_enabled" ) ].get<bool>( ) && g_cl.m_local->m_bDucking( ) && !m_fake_duck ) {
			g_cl.m_should_lag = true;
		}

		// fake duck.
		else if( m_fake_duck ) {
			g_cl.m_should_lag = true;
			limit = 14;
			mode = 0;
		}

		// commenting in gives the 'p2c effect' where it turns on fakelag between shots, though cba adjusting the current recharging..
		if( g_tickbase.m_shift_data.m_should_attempt_shift && ((!g_tickbase.m_shift_data.m_should_be_ready && g_tickbase.m_shift_data.m_prepare_recharge) || g_tickbase.m_shift_data.m_needs_recharge || g_tickbase.m_shift_data.m_should_be_ready) && !m_fake_duck ) {
			g_cl.m_should_lag = true;
			limit = 2;
			mode = 0;
		}

		// talking.
		if( g_csgo.m_engine->IsVoiceRecording( ) ) {
			g_cl.m_should_lag = true;
			limit = 2;
			mode = 0;
		}

		g_cl.m_wanted_choke = limit;
		 
		if(g_cl.m_should_lag) {
			// max.
			if( mode == 0 )
				*g_cl.m_packet = false;

			// break.
			else if( mode == 1 && delta <= 4096.f )
				*g_cl.m_packet = false;

			// random.
			else if( mode == 2 ) {
				// compute new factor.
				if( g_cl.m_lag >= m_random_lag )
					m_random_lag = g_csgo.RandomInt( 2, limit );

				// factor not met, keep choking. 
				else *g_cl.m_packet = false;
			}

			// break step.
			else if( mode == 3 ) {
				// normal break.
				if( m_step_switch ) {
					if( delta <= 4096.f )
						*g_cl.m_packet = false;
				}

				// max.
				else *g_cl.m_packet = false;
			}

			if (g_cl.m_lag >= limit) {
				*g_cl.m_packet = true;
			}
		}
	} else {
		g_cl.m_should_lag = false;
		g_cl.m_wanted_choke = 0;
	}

	if( g_cfg[ XOR( "rage_fakelag_on_land" ) ].get<bool>( ) ) {
		vec3_t                start = g_cl.m_local->m_vecOrigin( ), end = start, vel = g_cl.m_local->m_vecVelocity( );
		CTraceFilterWorldOnly filter;
		CGameTrace            trace;

		// gravity.
		vel.z -= ( g_csgo.sv_gravity->GetFloat( ) * g_csgo.m_globals->m_interval );

		// extrapolate.
		end += ( vel * g_csgo.m_globals->m_interval );

		// move down.
		end.z -= 2.f;

		g_csgo.m_engine_trace->TraceRay( Ray( start, end ), MASK_SOLID, &filter, &trace );

		// check if landed.
		if( trace.m_fraction != 1.f && trace.m_plane.m_normal.z > 0.7f && !( g_cl.m_flags & FL_ONGROUND ) )
			*g_cl.m_packet = true;
	}

	// do not lag while shooting.
	if( !g_hvh.m_fake_duck && g_cfg[ XOR( "cheat_mode" ) ].get<int>( ) == 0 ) {
		if( g_cl.m_old_shot )
			*g_cl.m_packet = true;
	}

	// we somehow reached the maximum amount of lag.
	// we cannot lag anymore and we also cannot shoot anymore since we cant silent aim.
	if( g_cl.m_lag >= g_cl.m_max_lag ) {
		// set bSendPacket to true.
		*g_cl.m_packet = true;
	}
}

void HVH::FakeDuck( )
{
	if( !g_csgo.m_cl || !g_cl.m_cmd )
		return;

	// ensure infinite duck.
	g_cl.m_cmd->m_buttons |= IN_BULLRUSH;

	if( !g_cl.m_processing || !m_fake_duck )
		return;

	// unduck if we are choking 7 or less ticks.
	if( g_csgo.m_cl->m_choked_commands <= 7 ) {
		g_cl.m_cmd->m_buttons &= ~IN_DUCK;
	}
	// duck if we are choking more than 7 ticks.
	else {
		g_cl.m_cmd->m_buttons |= IN_DUCK;
	}
}