#include "../includes.h"

Movement g_movement{ };;

void Movement::JumpRelated( ) {
	if( !g_cfg[ XOR( "misc_bhop" ) ].get<bool>( ) )
		return;

	if( !g_cl.m_cmd )
		return;

	if( !g_cl.m_processing )
		return;

	static const auto sv_autobunnyhopping = g_csgo.m_cvar->FindVar( HASH( "sv_autobunnyhopping" ) );
	static const auto sv_enablebunnyhopping = g_csgo.sv_enablebunnyhopping;

	if( !sv_autobunnyhopping || !sv_enablebunnyhopping )
		return;

	if( sv_autobunnyhopping->GetInt( ) == 1 && sv_enablebunnyhopping->GetInt( ) == 1 )
		return;

	// No need to bhop there
	const auto move_type = g_cl.m_local->m_MoveType( );
	if( move_type == MOVETYPE_LADDER ||
		move_type == MOVETYPE_NOCLIP ||
		move_type == MOVETYPE_OBSERVER )
		return;

	// Is player underwater?
	//if (g_cl.m_local->m_nWaterLevel() >= 2)
	//	return;

	static auto LastJumped = false;
	static auto ShouldFake = false;

	const auto flags = g_cl.m_local->m_fFlags( );

	if( !LastJumped && ShouldFake ) {
		ShouldFake = false;
		g_cl.m_cmd->m_buttons |= IN_JUMP;
	}
	else if( g_cl.m_cmd->m_buttons & IN_JUMP ) {

		if( g_cfg[ XOR( "misc_air_duck" ) ].get<bool>( ) ) {
			g_cl.m_cmd->m_buttons |= IN_DUCK;
		}


		if( flags & FL_ONGROUND ) {
			LastJumped = true;
			ShouldFake = true;
		}
		else {
			g_cl.m_cmd->m_buttons &= ~IN_JUMP;
			LastJumped = false;
		}
	}
	else {
		LastJumped = false;
		ShouldFake = false;
	}
}

void Movement::Strafe( ) {
	vec3_t velocity;
	float  delta, abs_delta, velocity_delta, correct;

	// don't strafe while we prolly want to jump scout..
	if( g_movement.m_slow_motion )
		return;

	// don't strafe while noclipping or on ladders..
	if( g_cl.m_local->m_MoveType( ) == MOVETYPE_NOCLIP || g_cl.m_local->m_MoveType( ) == MOVETYPE_LADDER )
		return;

	// get networked velocity ( maybe absvelocity better here? ).
	// meh, should be predicted anyway? ill see.
	velocity = g_cl.m_local->m_vecAbsVelocity( );

	// get the velocity len2d ( speed ).
	m_speed = velocity.length_2d( );

	// compute the ideal strafe angle for our velocity.
	m_ideal = ( m_speed > 0.f ) ? math::rad_to_deg( std::asin( 15.f / m_speed ) ) : 90.f;
	m_ideal2 = ( m_speed > 0.f ) ? math::rad_to_deg( std::asin( 30.f / m_speed ) ) : 90.f;

	// some additional sanity.
	math::clamp( m_ideal, 0.f, 90.f );
	math::clamp( m_ideal2, 0.f, 90.f );

	// save entity bounds ( used much in circle-strafer ).
	m_mins = g_cl.m_local->m_vecMins( );
	m_maxs = g_cl.m_local->m_vecMaxs( );

	// save our origin
	m_origin = g_cl.m_local->m_vecOrigin( );

	// disable strafing while pressing shift.
	if( ( g_cl.m_buttons & IN_SPEED ) || ( g_cl.m_flags & FL_ONGROUND ) )
		return;

	// for changing direction.
	// we want to change strafe direction every call.
	m_switch_value *= -1.f;

	// for allign strafer.
	++m_strafe_index;

	if( g_cl.m_pressing_move && g_cfg[ XOR( "misc_wasd_strafe" ) ].get<bool>( ) ) {
		// took this idea from stacker, thank u !!!!
		enum EDirections {
			FORWARDS = 0,
			BACKWARDS = 180,
			LEFT = 90,
			RIGHT = -90,
			BACK_LEFT = 135,
			BACK_RIGHT = -135
		};

		float wish_dir{ };

		// get our key presses.
		bool holding_w = g_cl.m_buttons & IN_FORWARD;
		bool holding_a = g_cl.m_buttons & IN_MOVELEFT;
		bool holding_s = g_cl.m_buttons & IN_BACK;
		bool holding_d = g_cl.m_buttons & IN_MOVERIGHT;

		// move in the appropriate direction.
		if( holding_w ) {
			//	forward left
			if( holding_a ) {
				wish_dir += ( EDirections::LEFT / 2 );
			}
			//	forward right
			else if( holding_d ) {
				wish_dir += ( EDirections::RIGHT / 2 );
			}
			//	forward
			else {
				wish_dir += EDirections::FORWARDS;
			}
		}
		else if( holding_s ) {
			//	back left
			if( holding_a ) {
				wish_dir += EDirections::BACK_LEFT;
			}
			//	back right
			else if( holding_d ) {
				wish_dir += EDirections::BACK_RIGHT;
			}
			//	back
			else {
				wish_dir += EDirections::BACKWARDS;
			}

			g_cl.m_cmd->m_forward_move = 0;
		}
		else if( holding_a ) {
			//	left
			wish_dir += EDirections::LEFT;
		}
		else if( holding_d ) {
			//	right
			wish_dir += EDirections::RIGHT;
		}
		
		g_cl.m_strafe_angles.y += math::NormalizeYaw(wish_dir);
	}

	// cancel out any forwardmove values.
	g_cl.m_cmd->m_forward_move = 0.f;

	// do allign strafer.
	if( g_input.GetKeyState( g_cfg[ XOR( "misc_a_strafe" ) ].get<int>( ) ) ) {
		float angle = std::max( m_ideal2, 4.f );

		if( angle > m_ideal2 && !( m_strafe_index % 5 ) )
			angle = m_ideal2;

		// add the computed step to the steps of the previous circle iterations.
		m_circle_yaw = math::NormalizedAngle( m_circle_yaw + angle );

		// apply data to usercmd.
		g_cl.m_strafe_angles.y = m_circle_yaw;
		g_cl.m_cmd->m_side_move = -450.f;

		return;
	}

	// do ciclestrafer
	else if( g_input.GetKeyState( g_cfg[ XOR( "misc_circle_strafe" ) ].get<int>( ) ) ) {
		// if no duck jump.
		if( !g_cfg[ XOR( "misc_air_duck" ) ].get<bool>( ) ) {
			// crouch to fit into narrow areas.
			g_cl.m_cmd->m_buttons |= IN_DUCK;
		}

		DoPrespeed( );
		return;
	}

	else if( g_input.GetKeyState( g_cfg[ XOR( "misc_z_strafe" ) ].get<int>( ) ) ) {
		float freq = ( g_cfg[ XOR( "misc_z_strafe_freq" ) ].get<float>( ) * 0.2f ) * g_csgo.m_globals->m_realtime;

		// range [ 1, 100 ], aka grenerates a factor.
		float factor = g_cfg[ XOR( "misc_z_strafe_dist" ) ].get<float>( ) * 0.5f;

		g_cl.m_strafe_angles.y += ( factor * std::sin( freq ) );
	}

	if( !g_cfg[ XOR( "misc_auto_strafe" ) ].get<bool>( ) )
		return;

	// get our viewangle change.
	delta = math::NormalizedAngle( g_cl.m_strafe_angles.y - m_old_yaw );

	// convert to absolute change.
	abs_delta = std::abs( delta );

	// save old yaw for next call.
	m_circle_yaw = m_old_yaw = g_cl.m_strafe_angles.y;

	// set strafe direction based on mouse direction change.
	if( delta > 0.f )
		g_cl.m_cmd->m_side_move = -450.f;

	else if( delta < 0.f )
		g_cl.m_cmd->m_side_move = 450.f;

	// we can accelerate more, because we strafed less then needed
	// or we got of track and need to be retracked.
	if( abs_delta <= m_ideal || abs_delta >= 30.f ) {
		// compute angle of the direction we are traveling in.
		ang_t velocity_angle;
		math::VectorAngles( velocity, velocity_angle );

		// get the delta between our direction and where we are looking at.
		velocity_delta = math::NormalizeYaw( g_cl.m_strafe_angles.y - velocity_angle.y );

		// correct our strafe amongst the path of a circle.
		correct = m_ideal;

		if( velocity_delta <= correct || m_speed <= 15.f ) {
			// not moving mouse, switch strafe every tick.
			if( -correct <= velocity_delta || m_speed <= 15.f ) {
				g_cl.m_strafe_angles.y += ( m_ideal * m_switch_value );
				g_cl.m_cmd->m_side_move = 450.f * m_switch_value;
			}

			else {
				g_cl.m_strafe_angles.y = velocity_angle.y - correct;
				g_cl.m_cmd->m_side_move = 450.f;
			}
		}

		else {
			g_cl.m_strafe_angles.y = velocity_angle.y + correct;
			g_cl.m_cmd->m_side_move = -450.f;
		}
	}
}

void Movement::DoPrespeed( ) {
	float   mod, min, max, step, strafe, time, angle;
	vec3_t  plane;

	// min and max values are based on 128 ticks.
	mod = g_csgo.m_globals->m_interval * 128.f;

	// scale min and max based on tickrate.
	min = 2.25f * mod;
	max = 5.f * mod;

	// compute ideal strafe angle for moving in a circle.
	strafe = m_ideal * 2.f;

	// clamp ideal strafe circle value to min and max step.
	math::clamp( strafe, min, max );

	// calculate time.
	time = 320.f / m_speed;

	// clamp time.
	math::clamp( time, 0.35f, 1.f );

	// init step.
	step = strafe;

	while( true ) {
		// if we will not collide with an object or we wont accelerate from such a big step anymore then stop.
		if( !WillCollide( time, step ) || max <= step )
			break;

		// if we will collide with an object with the current strafe step then increment step to prevent a collision.
		step += 0.2f;
	}

	if( step > max ) {
		// reset step.
		step = strafe;

		while( true ) {
			// if we will not collide with an object or we wont accelerate from such a big step anymore then stop.
			if( !WillCollide( time, step ) || step <= -min )
				break;

			// if we will collide with an object with the current strafe step decrement step to prevent a collision.
			step -= 0.2f;
		}

		if( step < -min ) {
			if( GetClosestPlane( plane ) ) {
				// grab the closest object normal
				// compute the angle of the normal
				// and push us away from the object.
				angle = math::rad_to_deg( std::atan2( plane.y, plane.x ) );
				step = -math::NormalizedAngle( m_circle_yaw - angle ) * 0.1f;
			}
		}

		else
			step -= 0.2f;
	}

	else
		step += 0.2f;

	// add the computed step to the steps of the previous circle iterations.
	m_circle_yaw = math::NormalizedAngle( m_circle_yaw + step );

	// apply data to usercmd.
	g_cl.m_strafe_angles.y = m_circle_yaw;
	g_cl.m_cmd->m_side_move = ( step >= 0.f ) ? -450.f : 450.f;
}

bool Movement::GetClosestPlane( vec3_t& plane ) {
	CGameTrace            trace;
	CTraceFilterWorldOnly filter;
	vec3_t                start{ m_origin };
	float                 smallest{ 1.f };
	const float		      dist{ 75.f };

	// trace around us in a circle
	for( float step{ }; step <= math::pi_2; step += ( math::pi / 10.f ) ) {
		// extend endpoint x units.
		vec3_t end = start;
		end.x += std::cos( step ) * dist;
		end.y += std::sin( step ) * dist;

		g_csgo.m_engine_trace->TraceRay( Ray( start, end, m_mins, m_maxs ), CONTENTS_SOLID, &filter, &trace );

		// we found an object closer, then the previouly found object.
		if( trace.m_fraction < smallest ) {
			// save the normal of the object.
			plane = trace.m_plane.m_normal;
			smallest = trace.m_fraction;
		}
	}

	// did we find any valid object?
	return smallest != 1.f && plane.z < 0.1f;
}

bool Movement::WillCollide( float time, float change ) {
	struct PredictionData_t {
		vec3_t start;
		vec3_t end;
		vec3_t velocity;
		float  direction;
		bool   ground;
		float  predicted;
	};

	PredictionData_t      data;
	CGameTrace            trace;
	CTraceFilterWorldOnly filter;

	// set base data.
	data.ground = g_cl.m_flags & FL_ONGROUND;
	data.start = m_origin;
	data.end = m_origin;
	data.velocity = g_cl.m_local->m_vecVelocity( );
	data.direction = math::rad_to_deg( std::atan2( data.velocity.y, data.velocity.x ) );

	for( data.predicted = 0.f; data.predicted < time; data.predicted += g_csgo.m_globals->m_interval ) {
		// predict movement direction by adding the direction change.
		// make sure to normalize it, in case we go over the -180/180 turning point.
		data.direction = math::NormalizedAngle( data.direction + change );

		// pythagoras.
		float hyp = data.velocity.length_2d( );

		// adjust velocity for new direction.
		data.velocity.x = std::cos( math::deg_to_rad( data.direction ) ) * hyp;
		data.velocity.y = std::sin( math::deg_to_rad( data.direction ) ) * hyp;

		// assume we bhop, set upwards impulse.
		if( data.ground )
			data.velocity.z = g_csgo.sv_jump_impulse->GetFloat( );

		else
			data.velocity.z -= g_csgo.sv_gravity->GetFloat( ) * g_csgo.m_globals->m_interval;

		// we adjusted the velocity for our new direction.
		// see if we can move in this direction, predict our new origin if we were to travel at this velocity.
		data.end += ( data.velocity * g_csgo.m_globals->m_interval );

		// trace
		g_csgo.m_engine_trace->TraceRay( Ray( data.start, data.end, m_mins, m_maxs ), MASK_PLAYERSOLID, &filter, &trace );

		// check if we hit any objects.
		if( trace.m_fraction != 1.f && trace.m_plane.m_normal.z <= 0.9f )
			return true;
		if( trace.m_startsolid || trace.m_allsolid )
			return true;

		// adjust start and end point.
		data.start = data.end = trace.m_endpos;

		// move endpoint 2 units down, and re-trace.
		// do this to check if we are on th floor.
		g_csgo.m_engine_trace->TraceRay( Ray( data.start, data.end - vec3_t{ 0.f, 0.f, 2.f }, m_mins, m_maxs ), MASK_PLAYERSOLID, &filter, &trace );

		// see if we moved the player into the ground for the next iteration.
		data.ground = trace.hit( ) && trace.m_plane.m_normal.z > 0.7f;
	}

	// the entire loop has ran
	// we did not hit shit.
	return false;
}

void Movement::FixMove( CUserCmd* cmd, ang_t wish_angles ) {
	vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	math::AngleVectors( wish_angles, &view_fwd, &view_right, &view_up );
	math::AngleVectors( cmd->m_view_angles, &cmd_fwd, &cmd_right, &cmd_up );

	const auto v8 = sqrtf( ( view_fwd.x * view_fwd.x ) + ( view_fwd.y * view_fwd.y ) );
	const auto v10 = sqrtf( ( view_right.x * view_right.x ) + ( view_right.y * view_right.y ) );
	const auto v12 = sqrtf( view_up.z * view_up.z );

	const vec3_t norm_view_fwd( ( 1.f / v8 ) * view_fwd.x, ( 1.f / v8 ) * view_fwd.y, 0.f );
	const vec3_t norm_view_right( ( 1.f / v10 ) * view_right.x, ( 1.f / v10 ) * view_right.y, 0.f );
	const vec3_t norm_view_up( 0.f, 0.f, ( 1.f / v12 ) * view_up.z );

	const auto v14 = sqrtf( ( cmd_fwd.x * cmd_fwd.x ) + ( cmd_fwd.y * cmd_fwd.y ) );
	const auto v16 = sqrtf( ( cmd_right.x * cmd_right.x ) + ( cmd_right.y * cmd_right.y ) );
	const auto v18 = sqrtf( cmd_up.z * cmd_up.z );

	const vec3_t norm_cmd_fwd( ( 1.f / v14 ) * cmd_fwd.x, ( 1.f / v14 ) * cmd_fwd.y, 0.f );
	const vec3_t norm_cmd_right( ( 1.f / v16 ) * cmd_right.x, ( 1.f / v16 ) * cmd_right.y, 0.f );
	const vec3_t norm_cmd_up( 0.f, 0.f, ( 1.f / v18 ) * cmd_up.z );

	const auto v22 = norm_view_fwd.x * cmd->m_forward_move;
	const auto v26 = norm_view_fwd.y * cmd->m_forward_move;
	const auto v28 = norm_view_fwd.z * cmd->m_forward_move;
	const auto v24 = norm_view_right.x * cmd->m_side_move;
	const auto v23 = norm_view_right.y * cmd->m_side_move;
	const auto v25 = norm_view_right.z * cmd->m_side_move;
	const auto v30 = norm_view_up.x * cmd->m_up_move;
	const auto v27 = norm_view_up.z * cmd->m_up_move;
	const auto v29 = norm_view_up.y * cmd->m_up_move;

	cmd->m_forward_move = ( ( ( ( norm_cmd_fwd.x * v24 ) + ( norm_cmd_fwd.y * v23 ) ) + ( norm_cmd_fwd.z * v25 ) )
		+ ( ( ( norm_cmd_fwd.x * v22 ) + ( norm_cmd_fwd.y * v26 ) ) + ( norm_cmd_fwd.z * v28 ) ) )
		+ ( ( ( norm_cmd_fwd.y * v30 ) + ( norm_cmd_fwd.x * v29 ) ) + ( norm_cmd_fwd.z * v27 ) );
	cmd->m_side_move = ( ( ( ( norm_cmd_right.x * v24 ) + ( norm_cmd_right.y * v23 ) ) + ( norm_cmd_right.z * v25 ) )
		+ ( ( ( norm_cmd_right.x * v22 ) + ( norm_cmd_right.y * v26 ) ) + ( norm_cmd_right.z * v28 ) ) )
		+ ( ( ( norm_cmd_right.x * v29 ) + ( norm_cmd_right.y * v30 ) ) + ( norm_cmd_right.z * v27 ) );
	cmd->m_up_move = ( ( ( ( norm_cmd_up.x * v23 ) + ( norm_cmd_up.y * v24 ) ) + ( norm_cmd_up.z * v25 ) )
		+ ( ( ( norm_cmd_up.x * v26 ) + ( norm_cmd_up.y * v22 ) ) + ( norm_cmd_up.z * v28 ) ) )
		+ ( ( ( norm_cmd_up.x * v30 ) + ( norm_cmd_up.y * v29 ) ) + ( norm_cmd_up.z * v27 ) );

	wish_angles = cmd->m_view_angles;

	if( g_cl.m_local->m_MoveType( ) != MOVETYPE_LADDER )
		cmd->m_buttons &= ~( IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT );
}

void Movement::AutoPeek( ) {
	// set to invert if we press the button.
	if( g_input.GetKeyState( g_cfg[ XOR( "misc_auto_peek" ) ].get<int>( ) ) && !m_slow_motion ) {
		if( g_cl.m_old_shot )
			m_invert = true;

		vec3_t move{ g_cl.m_cmd->m_forward_move, g_cl.m_cmd->m_side_move, 0.f };

		if( m_invert ) {
			move *= -1.f;
			g_cl.m_cmd->m_forward_move = move.x;
			g_cl.m_cmd->m_side_move = move.y;
		}
	}

	else m_invert = false;

}

void Movement::AutoStop()
{
	if (g_cl.m_ground && g_cfg[XOR("rage_aimbot_auto_stop")].get<bool>() && !m_slow_motion) {
		//if (g_cfg[XOR("rage_aimbot_auto_stop_type_full")].get<bool>()) {
		if (g_aimbot.m_stop) {
			if (g_cl.m_speed > 15.f) {
				// convert velocity to angular momentum.
				ang_t angle;
				math::VectorAngles(g_cl.m_local->m_vecVelocity(), angle);

				// get our current speed of travel.
				float speed = g_cl.m_local->m_vecVelocity().length();

				// fix direction by factoring in where we are looking.
				angle.y = g_cl.m_view_angles.y - angle.y;

				// convert corrected angle back to a direction.
				vec3_t direction;
				math::AngleVectors(angle, &direction);

				vec3_t stop = direction * -speed;

				g_cl.m_cmd->m_forward_move = stop.x;
				g_cl.m_cmd->m_side_move = stop.y;
			}
			else {
				g_cl.m_cmd->m_forward_move = 0;
				g_cl.m_cmd->m_side_move = 0;
			}

			g_aimbot.m_stop = false;
		}
	}
}

/*thanks onetap.com*/
void Movement::ClampMovementSpeed(float speed) {
	float final_speed = speed;

	if (!g_cl.m_cmd || !g_cl.m_processing)
		return;

	g_cl.m_cmd->m_buttons |= IN_SPEED;

	float squirt = std::sqrtf((g_cl.m_cmd->m_forward_move * g_cl.m_cmd->m_forward_move) + (g_cl.m_cmd->m_side_move * g_cl.m_cmd->m_side_move));

	if (squirt > speed) {
		float squirt2 = std::sqrtf((g_cl.m_cmd->m_forward_move * g_cl.m_cmd->m_forward_move) + (g_cl.m_cmd->m_side_move * g_cl.m_cmd->m_side_move));

		float cock1 = g_cl.m_cmd->m_forward_move / squirt2;
		float cock2 = g_cl.m_cmd->m_side_move / squirt2;

		auto Velocity = g_cl.m_local->m_vecVelocity().length_2d();

		if (final_speed + 1.0 <= Velocity) {
			g_cl.m_cmd->m_forward_move = 0;
			g_cl.m_cmd->m_side_move = 0;
		}
		else {
			g_cl.m_cmd->m_forward_move = cock1 * final_speed;
			g_cl.m_cmd->m_side_move = cock2 * final_speed;
		}
	}
}

void Movement::FakeWalk( ) {
	vec3_t velocity{ g_cl.m_local->m_vecVelocity( ) };
	int    ticks{ }, max{ 7 };

	if( !m_slow_motion )
		return;

	if( !g_cl.m_local->GetGroundEntity( ) )
		return;

	if( g_cfg[ XOR( "movement_fakewalk_type" ) ].get<int>( ) == 1 ) {
		//if( g_cl.m_weapon && g_cl.m_weapon_info )
	//	max = std::floor( 11 * ( g_cl.m_weapon_info->m_max_player_speed / 250.f ) );

	// reference:
	// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/gamemovement.cpp#L1612

	// calculate friction.
		float friction = g_csgo.sv_friction->GetFloat( ) * g_cl.m_local->m_surfaceFriction( );

		for( ; ticks <= g_cl.m_max_lag; ++ticks ) {
			// calculate speed.
			float speed = velocity.length( );

			// if too slow return.
			if( speed <= 0.1f )
				break;

			// bleed off some speed, but if we have less than the bleed, threshold, bleed the threshold amount.
			float control = std::max( speed, g_csgo.sv_stopspeed->GetFloat( ) );

			// calculate the drop amount.
			float drop = control * friction * g_csgo.m_globals->m_interval;

			// scale the velocity.
			float newspeed = std::max( 0.f, speed - drop );

			if( newspeed != speed ) {
				// determine proportion of old speed we are using.
				newspeed /= speed;

				// adjust velocity according to proportion.
				velocity *= newspeed;
			}
		}

		// zero forwardmove and sidemove.
		if( ticks > ( max - g_cl.m_lag ) || !g_cl.m_lag )
			g_cl.m_cmd->m_forward_move = g_cl.m_cmd->m_side_move = 0.f;

		// set bSendPacket.
		*g_cl.m_packet = !( g_cl.m_lag < max );
	}
	else {
		if( g_cl.m_weapon_info ) {
			// get the max possible speed whilest we are still accurate.
			float flMaxSpeed = g_cl.m_local->m_bIsScoped( ) > 0 ? g_cl.m_weapon_info->flMaxPlayerSpeedAlt : g_cl.m_weapon_info->flMaxPlayerSpeed;
			float flDesiredSpeed = ( flMaxSpeed * 0.33000001);

			ClampMovementSpeed(flDesiredSpeed);
		}
	}
}