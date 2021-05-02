#include "../includes.h"

Shots g_shots{ };

void Shots::OnShotFire( Player* target, float damage, int bullets, LagComp::LagRecord_t* record, int hitbox ) {
	// setup new shot data.
	ShotRecord shot;
	shot.m_target = target;
	shot.m_record = record;
	shot.m_time = g_csgo.m_globals->m_curtime;
	shot.m_lat = g_cl.m_latency;
	shot.m_damage = damage;
	shot.m_pos = g_cl.m_shoot_pos;
	shot.m_hitbox = hitbox;
	shot.m_impacted = false;
	shot.m_confirmed = false;
	shot.m_hurt = false;
	shot.m_weapon_range = g_cl.m_weapon_info->flRange;

	// we are not shooting manually.
	// and this is the first bullet, only do this once.
	if( target && record ) {
		// increment total shots on this player.
		AimPlayer* data = &g_aimbot.m_players[ target->index( ) - 1 ];
		if( data ) {
			++data->m_shots;

			auto matrix = record->m_pMatrix_Resolved;

			if (matrix)
				shot.m_matrix = matrix;
		}
	}

	// add to tracks.
	m_shots.push_front( shot );

	// no need to keep an insane amount of shots.
	while( m_shots.size( ) > 128 )
		m_shots.pop_back( );
}

void Shots::OnImpact( IGameEvent* evt ) {
	int        attacker;
	vec3_t     pos, dir, start, end;
	float      time;

	// screw this.
	if( !evt || !g_cl.m_local )
		return;

	// get attacker, if its not us, screw it.
	attacker = g_csgo.m_engine->GetPlayerForUserID( evt->m_keys->FindKey( HASH( "userid" ) )->GetInt( ) );
	if( attacker != g_csgo.m_engine->GetLocalPlayer( ) )
		return;

	// decode impact coordinates and convert to vec3.
	pos = {
		evt->m_keys->FindKey( HASH( "x" ) )->GetFloat( ),
		evt->m_keys->FindKey( HASH( "y" ) )->GetFloat( ),
		evt->m_keys->FindKey( HASH( "z" ) )->GetFloat( )
	};

	// get prediction time at this point.
	time = g_csgo.m_globals->m_curtime;

	// add to visual impacts if we have features that rely on it enabled.
	// todo - dex; need to match shots for this to have proper Weapon_ShootPosition, don't really care to do it anymore.
	if( g_cfg[ XOR( "visuals_misc_bullet_beam" ) ].get<bool>( ) )
		m_vis_impacts.push_back( { pos, g_cl.m_local->Weapon_ShootPosition( ), g_cl.m_local->m_nTickBase( ) } );

	// we did not take a shot yet.
	if( m_shots.empty( ) )
		return;

	struct ShotMatch_t { float delta; ShotRecord* shot; };
	ShotMatch_t match;
	match.delta = std::numeric_limits< float >::max( );
	match.shot = nullptr;

	// iterate all shots.
	for( auto& s : m_shots ) {

		// this shot was already matched
		// with a 'bullet_impact' event.
		if( s.m_impacted )
			continue;

		// add the latency to the time when we shot.
		// to predict when we would receive this event.
		float predicted = s.m_time + s.m_lat;

		// get the delta between the current time
		// and the predicted arrival time of the shot.
		float delta = std::abs( time - predicted );

		// fuck this.
		if( delta > 1.f )
			continue;

		// store this shot as being the best for now.
		if( delta < match.delta ) {
			match.delta = delta;
			match.shot = &s;
		}
	}

	// no valid shotrecord was found.
	ShotRecord* shot = match.shot;
	if( !shot )
		return;

	// this shot was matched.
	shot->m_impacted = true;
	shot->m_server_pos = pos;

	// create new impact instance that we can match with a player hurt.
	ImpactRecord impact;
	impact.m_shot = shot;
	impact.m_tick = g_cl.m_local->m_nTickBase( );
	impact.m_pos = pos;

	//g_cl.print( "imp %x time: %f lat: %f dmg: %f\n", shot->m_record, shot->m_time, shot->m_lat, shot->m_damage );

	// add to track.
	m_impacts.push_front( impact );

	// no need to keep an insane amount of impacts.
	while( m_impacts.size( ) > 128 )
		m_impacts.pop_back( );
}

void Shots::OnHurt( IGameEvent* evt ) {
	int         attacker, victim, group, hp;
	float       damage;
	std::string name;

	if( !evt || !g_cl.m_local )
		return;

	attacker = g_csgo.m_engine->GetPlayerForUserID( evt->m_keys->FindKey( HASH( "attacker" ) )->GetInt( ) );
	victim = g_csgo.m_engine->GetPlayerForUserID( evt->m_keys->FindKey( HASH( "userid" ) )->GetInt( ) );

	// skip invalid player indexes.
	// should never happen? world entity could be attacker, or a nade that hits you.
	if( attacker < 1 || attacker > 64 || victim < 1 || victim > 64 )
		return;

	// we were not the attacker or we hurt ourselves.
	else if( attacker != g_csgo.m_engine->GetLocalPlayer( ) || victim == g_csgo.m_engine->GetLocalPlayer( ) )
		return;

	// get hitgroup.
	// players that get naded ( DMG_BLAST ) or stabbed seem to be put as HITGROUP_GENERIC.
	group = evt->m_keys->FindKey( HASH( "hitgroup" ) )->GetInt( );

	// invalid hitgroups ( note - dex; HITGROUP_GEAR isn't really invalid, seems to be set for hands and stuff? ).
	if( group == HITGROUP_GEAR )
		return;

	// get the player that was hurt.
	Player* target = g_csgo.m_entlist->GetClientEntity< Player* >( victim );
	if( !target )
		return;

	// get player info.
	player_info_t info;
	if( !g_csgo.m_engine->GetPlayerInfo( victim, &info ) )
		return;

	// get player name;
	name = std::string( info.m_name ).substr( 0, 24 );

	// get damage reported by the server.
	damage = ( float )evt->m_keys->FindKey( HASH( "dmg_health" ) )->GetInt( );

	// get remaining hp.
	hp = evt->m_keys->FindKey( HASH( "health" ) )->GetInt( );

	// hitmarker.
	if( g_cfg[ XOR( "misc_hitmarker" ) ].get<int>( ) ) {
		g_visuals.m_hit_duration = 1.f;
		g_visuals.m_hit_start = g_csgo.m_globals->m_curtime;
		g_visuals.m_hit_end = g_visuals.m_hit_start + g_visuals.m_hit_duration;
	}

	// hitsounds
	if( g_cfg[ XOR( "misc_hitsound" ) ].get<int>( ) ) {
		int type = g_cfg[ XOR( "misc_hitsound" ) ].get<int>( );

		switch( type ) {

		case 0:
			// none.
			break;

		case 1:
			// arena switch press.
			g_csgo.m_sound->EmitAmbientSound( XOR( "buttons/arena_switch_press_02.wav" ), ( g_cfg[ XOR( "misc_hitsound_volume" ) ].get<float>( ) / 100.f ) );
			break;

		case 2:
			// custom.

			// make sure this isn't empty.
			if( !g_cl.m_hitsounds.empty( ) ) {
				// setup the sound's directory.
				std::string sound = std::string( XOR( "C:\\" ) ).append( g_cl.m_hitsounds[ g_cfg[ XOR( "misc_custom_hitsound" ) ].get<int>( ) ] );
				if( sound.size( ) > 2 ) {
					// read the .wav file into memory.
					auto sound_bytes = util::ReadWavFileIntoMemory( sound );

					// fix this, cant memcpy cos parset_hitsound is nullptr (obviously wtf)
					// i tried std::memset but no work
					// idk i've been up the whole night and its like 3 fkn am so i cant think straight
					// thanks
					// oh also since ur reading this and im probably asleep can u pls make it so we
					// only safepoint on body(?) and head, cos rn since i forced safepoints we wont
					// shoot at legs or feet at all (fucking obviously) 
					// thank you again
					{
						// store the parsed hitsound bytes to a dummy byte.
						static byte* parsed_hitsound;

						// adjust the hitsound volume.
						if( g_cl.m_update_sound ) {
							parsed_hitsound = sound_bytes;

							Hitsounds::WavHeader_t header;
							header.ParseWavHeader( parsed_hitsound );
							g_hitsounds.AdjustHitsoundVolume( header, ( g_cfg[ XOR( "misc_hitsound_volume" ) ].get<float>( ) / 200.f ) );

							g_cl.m_update_sound = false;
						}

						// play the sound.
						if( parsed_hitsound ) {
							PlaySoundA( LPCSTR( parsed_hitsound ), NULL, SND_ASYNC | SND_MEMORY );
						}
					}
				}
			}

			break;
		}
	}

	// print this shit.
	if( g_cfg[ XOR( "misc_log_damage" ) ].get<bool>( ) ) {
		std::string out = tfm::format( XOR( "hit %s in the %s for %i [ %i remaining ] [ tick: %i ]" ), name, m_groups[ group ], ( int )damage, hp, g_csgo.m_globals->m_tick_count % 100 );
		g_notify.add( out );
	}

	if( group == HITGROUP_GENERIC )
		return;

	// if we hit a player, mark vis impacts.
	if( !m_vis_impacts.empty( ) ) {
		for( auto& i : m_vis_impacts ) {
			if( i.m_tickbase == g_cl.m_local->m_nTickBase( ) )
				i.m_hit_player = true;
		}
	}

	struct ShotMatch_t { float delta; ShotRecord* shot; };
	ShotMatch_t match;
	match.delta = std::numeric_limits< float >::max();
	match.shot = nullptr;

	// iterate all shots.
	for (auto& s : m_shots) {

		// this shot was already matched
		// with a 'player_hurt' event.
		if (s.m_hurt)
			continue;

		// add the latency to the time when we shot.
		// to predict when we would receive this event.
		float predicted = s.m_time + s.m_lat;

		// get the delta between the current time
		// and the predicted arrival time of the shot.
		float delta = std::abs(g_csgo.m_globals->m_curtime - predicted);

		// fuck this.
		if (delta > 1.f)
			continue;

		// store this shot as being the best for now.
		if (delta < match.delta) {
			match.delta = delta;
			match.shot = &s;
		}
	}

	// no valid shotrecord was found.
	ShotRecord* shot = match.shot;
	if (!shot)
		return;

	shot->m_hurt = true;
}

void Shots::OnFire(IGameEvent* evt)
{
	int attacker;
	 
	// screw this.
	if (!evt || !g_cl.m_local)
		return;

	// get attacker, if its not us, screw it.
	attacker = g_csgo.m_engine->GetPlayerForUserID(evt->m_keys->FindKey(HASH("userid"))->GetInt());
	if (attacker != g_csgo.m_engine->GetLocalPlayer())
		return;

	struct ShotMatch_t { float delta; ShotRecord* shot; };
	ShotMatch_t match;
	match.delta = std::numeric_limits< float >::max();
	match.shot = nullptr;

	// iterate all shots.
	for (auto& s : m_shots) {

		// this shot was already matched
		// with a 'weapon_fire' event.
		if (s.m_confirmed)
			continue;

		// add the latency to the time when we shot.
		// to predict when we would receive this event.
		float predicted = s.m_time + s.m_lat;

		// get the delta between the current time
		// and the predicted arrival time of the shot.
		float delta = std::abs(g_csgo.m_globals->m_curtime - predicted);

		// fuck this.
		if (delta > 1.f)
			continue;

		// store this shot as being the best for now.
		if (delta < match.delta) {
			match.delta = delta;
			match.shot = &s;
		}
	}

	// no valid shotrecord was found.
	ShotRecord* shot = match.shot;
	if (!shot)
		return;

	shot->m_confirmed = true;
}

void Shots::OnFrameStage( )
{
	if( !g_cl.m_processing || m_shots.empty( ) ) {
		if( !m_shots.empty( ) )
			m_shots.clear( );

		return;
	}

	for( auto it = m_shots.begin( ); it != m_shots.end( );) {
		if( it->m_time + 1.f < g_csgo.m_globals->m_curtime )
			it = m_shots.erase( it );
		else
			it = next( it );
	}

	for( auto it = m_shots.begin( ); it != m_shots.end( );) {
		if( it->m_impacted && !it->m_hurt && it->m_confirmed ) {
			// not in nospread mode, see if the shot missed due to spread.
			Player* target = it->m_target;
			if( !target ) {
				it = m_shots.erase( it );
				continue;
			}

			// not gonna bother anymore.
			if( !target->alive( ) ) {
				it = m_shots.erase( it );
				continue;
			}

			AimPlayer* data = &g_aimbot.m_players[ target->index( ) - 1 ];
			if( !data ) {
				it = m_shots.erase( it );
				continue;
			}

			// this record was deleted already.
			if( !it->m_record->m_pMatrix_Resolved ) {
				g_notify.add( XOR( "shot missed due to invalid target" ) );
				it = m_shots.erase( it );
				continue;
			}

			// write historical matrix of the time that we shot
			// into the games bone cache, so we can trace against it.
			//it->m_record->cache( );

			// start position of trace is where we took the shot.
			vec3_t start = it->m_pos;

			// the impact pos contains the spread from the server
			// which is generated with the server seed, so this is where the bullet
			// actually went, compute the direction of this from where the shot landed
			// and from where we actually took the shot.
			vec3_t dir = ( it->m_server_pos - start ).normalized( );

			// get end pos by extending direction forward.
			// todo; to do this properly should save the weapon range at the moment of the shot, cba..
			vec3_t end = start + dir * start.dist_to( it->m_record->m_vecOrigin ) * 6.6f;

			if( g_cfg[ XOR( "cheat_mode" ) ].get<int>( ) == 0 ) {
				if( !g_aimbot.CanHit( start, end, it->m_record, it->m_hitbox, true, it->m_matrix ) ) {
					g_notify.add( XOR( "shot missed due to spread" ) );

					//g_csgo.m_debug_overlay->AddLineOverlay(start, end, 255, 255, 255, false, 3.f);

					it = m_shots.erase( it );

					continue;
				}
			}

			// let's not increment this if this is a shot record.
			if( !it->m_record->m_bDidShot )
				++data->m_missed_shots;

			g_notify.add( XOR( "shot missed due to bad resolve" ) );

			// we processed this shot, let's delete it.
			it = m_shots.erase( it );
		}
		else {
			it = next( it );
		}
	}
}

bool Sphere::intersectsRay(const Ray& ray)
{
	float T1, T2;
	if (!IntersectInfiniteRayWithSphere(ray.m_start, ray.m_delta, m_center, m_radius, &T1, &T2))
		return false;

	return T2 >= 0.0f && T1 <= 1.0f;
}

bool Sphere::intersectsRay(const Ray& ray, vec3_t& vecIntersection)
{
	float T1, T2;
	if (!IntersectInfiniteRayWithSphere(ray.m_start, ray.m_delta, m_center, m_radius, &T1, &T2))
		return false;

	if (T1 > 1.0f || T2 < 0.0f)
		return false;

	// Clamp it!
	if (T1 < 0.0f)
		T1 = 0.0f;
	if (T2 > 1.0f)
		T2 = 1.0f;

	vecIntersection = ray.m_start + ray.m_delta * T1;

	return true;
}

bool Sphere::IntersectInfiniteRayWithSphere(const vec3_t& vecRayOrigin, const vec3_t& vecRayDelta, const vec3_t& vecSphereCenter, float flRadius, float* pT1, float* pT2)
{
	vec3_t vecSphereToRay = (vecRayOrigin - vecSphereCenter);

	float a = vecRayDelta.dot(vecRayDelta);

	// This would occur in the case of a zero-length ray
	if (a == 0.0f) {
		*pT1 = *pT2 = 0.0f;
		return vecSphereToRay.length_sqr() <= flRadius * flRadius;
	}

	float b = 2 * vecSphereToRay.dot(vecRayDelta);
	float c = vecSphereToRay.dot(vecSphereToRay) - flRadius * flRadius;
	float flDiscrim = b * b - 4 * a * c;
	if (flDiscrim < 0.0f)
		return false;

	flDiscrim = sqrt(flDiscrim);
	float oo2a = 0.5f / a;
	*pT1 = (-b - flDiscrim) * oo2a;
	*pT2 = (-b + flDiscrim) * oo2a;
	return true;
}

void Shots::SetupCapsule(const vec3_t& vecMin, const vec3_t& vecMax, float flRadius, int hitgroup, std::vector<Sphere>& m_cSpheres)
{
	auto vecDelta = (vecMax - vecMin).normalized();
	auto vecCenter = vecMin;

	Sphere sphere = Sphere{ vecMin, flRadius, hitgroup };
	m_cSpheres.push_back(sphere);

	for (size_t i = 1; i < std::floor(vecMin.dist_to(vecMax)); ++i)
	{
		Sphere sphere_ = Sphere{ vecMin + vecDelta * static_cast<float>(i), flRadius, hitgroup };
		m_cSpheres.push_back(Sphere{ sphere_ });
	}
}