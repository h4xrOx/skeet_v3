#include "../includes.h"

Visuals g_visuals{ };;

void Visuals::ModulateWorld( ) {
	if( !g_cl.m_local || !g_csgo.m_engine->IsInGame( ) )
		return;

	std::vector< IMaterial* > world, props;

	// iterate material handles.
	for( uint16_t h{ g_csgo.m_material_system->FirstMaterial( ) }; h != g_csgo.m_material_system->InvalidMaterial( ); h = g_csgo.m_material_system->NextMaterial( h ) ) {
		// get material from handle.
		IMaterial* mat = g_csgo.m_material_system->GetMaterial( h );
		if( !mat )
			continue;

		if( mat->IsErrorMaterial( ) )
			continue;

		// store world materials.
		if( FNV1a::get( mat->GetTextureGroupName( ) ) == HASH( "World textures" ) )
			world.push_back( mat );

		// store props.
		else if( FNV1a::get( mat->GetTextureGroupName( ) ) == HASH( "StaticProp textures" ) )
			props.push_back( mat );

	}

	// night.
	if( g_cfg[ XOR( "misc_world_night" ) ].get<bool>( ) ) {
		const float world_darkness = 1.f - ( g_cfg[ XOR( "misc_world_night_darkness" ) ].get<float>( ) / 100.f );
		const float prop_darkness = 1.3f - ( g_cfg[ XOR( "misc_world_night_darkness" ) ].get<float>( ) / 100.f );
		//const float prop_alpha = g_cfg[XOR("misc_world_prop_opacity")].get<float>() < 99.f ? g_cfg[XOR("misc_world_prop_opacity")].get<float>() * 0.01f : 1.f;

		for( const auto& w : world )
			w->ColorModulate( world_darkness, world_darkness, world_darkness );

		// IsUsingStaticPropDebugModes my nigga
		g_csgo.r_DrawSpecificStaticProp->SetValue( 0 );

		for( const auto& p : props ) {
			p->ColorModulate( prop_darkness, prop_darkness, prop_darkness );
			//p->AlphaModulate(prop_alpha);
		}

		// change skybox.
		game::SetSkybox( XOR( "sky_csgo_night02" ) );
	}

	// disable night.
	else {
		//const float prop_alpha = g_cfg[XOR("misc_world_prop_opacity")].get<float>() < 99.f ? g_cfg[XOR("misc_world_prop_opacity")].get<float>() * 0.01f : 1.f;

		for( const auto& w : world )
			w->ColorModulate( 1.f, 1.f, 1.f );

		// restore r_DrawSpecificStaticProp.
		g_csgo.r_DrawSpecificStaticProp->SetValue( -1 );

		for( const auto& p : props ) {
			p->ColorModulate( 1.f, 1.f, 1.f );
			//p->AlphaModulate(prop_alpha);
		}

		// vertigo!
		game::SetSkybox( XOR( "vertigoblue_hdr" ) );
	}
}

void Visuals::ThirdpersonThink( ) {
	ang_t                          offset;
	vec3_t                         origin, forward;
	static CTraceFilterSimple_game filter{ };
	CGameTrace                     tr;

	// for whatever reason overrideview also gets called from the main menu.
	if( !g_csgo.m_engine->IsInGame( ) )
		return;

	// check if we have a local player and he is alive.
	bool alive = g_cl.m_local && g_cl.m_local->alive( );

	// camera should be in thirdperson.
	if( m_thirdperson ) {

		// if alive and not in thirdperson already switch to thirdperson.
		if( alive && !g_csgo.m_input->m_camera_in_third_person )
			g_csgo.m_input->m_camera_in_third_person = true;

		// if dead and spectating in firstperson switch to thirdperson.
		else if( g_cl.m_local->m_iObserverMode( ) == 4 ) {

			// if in thirdperson, switch to firstperson.
			// we need to disable thirdperson to spectate properly.
			if( g_csgo.m_input->m_camera_in_third_person ) {
				g_csgo.m_input->m_camera_in_third_person = false;
				g_csgo.m_input->m_camera_offset.z = 0.f;
			}

			g_cl.m_local->m_iObserverMode( ) = 5;
		}
	}

	// camera should be in firstperson.
	else if( g_csgo.m_input->m_camera_in_third_person ) {
		g_csgo.m_input->m_camera_in_third_person = false;
		g_csgo.m_input->m_camera_offset.z = 0.f;
	}

	// if after all of this we are still in thirdperson.
	if( g_csgo.m_input->m_camera_in_third_person ) {
		// get camera angles.
		g_csgo.m_engine->GetViewAngles( offset );

		// get our viewangle's forward directional vector.
		math::AngleVectors( offset, &forward );

		// cam_idealdist convar.
		offset.z = g_cfg[ XOR( "misc_thirdperson_distance" ) ].get<float>( );

		// start pos.
		origin = g_cl.m_shoot_pos;

		// setup trace filter and trace.
		filter.SetPassEntity( g_cl.m_local );

		g_csgo.m_engine_trace->TraceRay(
			Ray( origin, origin - ( forward * offset.z ), { -16.f, -16.f, -16.f }, { 16.f, 16.f, 16.f } ),
			MASK_NPCWORLDSTATIC,
			( ITraceFilter* )&filter,
			&tr
		);

		// adapt distance to travel time.
		math::clamp( tr.m_fraction, 0.f, 1.f );
		offset.z *= tr.m_fraction;

		// override camera angles.
		g_csgo.m_input->m_camera_offset = { offset.x, offset.y, offset.z };
	}
}

void Visuals::HitmarkerScreen( ) {
	if( g_cfg[ XOR( "misc_hitmarker" ) ].get<int>( ) != 1 )
		return;

	if( g_csgo.m_globals->m_curtime > m_hit_end )
		return;

	if( m_hit_duration <= 0.f )
		return;

	float complete = ( g_csgo.m_globals->m_curtime - m_hit_start ) / m_hit_duration;
	int x = g_cl.m_width / 2,
		y = g_cl.m_height / 2,
		alpha = ( 1.f - complete ) * 240;

	constexpr int line{ 6 };

	render::line( x - line, y - line, x - ( line / 4 ), y - ( line / 4 ), { 200, 200, 200, alpha } );
	render::line( x - line, y + line, x - ( line / 4 ), y + ( line / 4 ), { 200, 200, 200, alpha } );
	render::line( x + line, y + line, x + ( line / 4 ), y + ( line / 4 ), { 200, 200, 200, alpha } );
	render::line( x + line, y - line, x + ( line / 4 ), y - ( line / 4 ), { 200, 200, 200, alpha } );
}

void Visuals::HitmarkerWorld( ) {
	if( g_cfg[ XOR( "misc_hitmarker" ) ].get<int>( ) != 2 )
		return;

	for( size_t i{ }; i < m_world_hitmarkers.size( ); ++i ) {
		WorldHitmarkerData_t& info = m_world_hitmarkers[ i ];

		// If the delta between the current time and hurt time is larger than 2 seconds then we should erase
		if( g_inputpred.m_stored_variables.m_flCurtime - info.m_time > 2.0f ) {
			info.m_alpha -= ( 1.0f / 2.0f ) * g_csgo.m_globals->m_frametime;
			info.m_alpha = std::clamp<float>( info.m_alpha, 0.0f, 1.0f );
		}

		if( info.m_alpha <= 0.0f ) {
			continue;
		}

		if( info.m_world_to_screen ) {
			constexpr int line{ 8 };

			auto draw_lines = [ & ] ( vec2_t pos_custom, Color clr ) -> void {
				D3D::Line(
					vec2_t( int( pos_custom.x - ( line - 1 ) ), int( pos_custom.y - ( line - 1 ) ) ),
					vec2_t( int( pos_custom.x - ( line / 4 ) ), int( pos_custom.y - ( line / 4 ) ) ),
					clr.OverrideAlpha( 255 * info.m_alpha ) );

				D3D::Line(
					vec2_t( int( pos_custom.x - ( line - 1 ) ), int( pos_custom.y + ( line - 1 ) ) ),
					vec2_t( int( pos_custom.x - ( line / 4 ) ), int( pos_custom.y + ( line / 4 ) ) ),
					clr.OverrideAlpha( 255 * info.m_alpha ) );

				D3D::Line(
					vec2_t( int( pos_custom.x + ( line - 1 ) ), int( pos_custom.y + ( line - 1 ) ) ),
					vec2_t( int( pos_custom.x + ( line / 4 ) ), int( pos_custom.y + ( line / 4 ) ) ),
					clr.OverrideAlpha( 255 * info.m_alpha ) );

				D3D::Line(
					vec2_t( int( pos_custom.x + ( line - 1 ) ), int( pos_custom.y - ( line - 1 ) ) ),
					vec2_t( int( pos_custom.x + ( line / 4 ) ), int( pos_custom.y - ( line / 4 ) ) ),
					clr.OverrideAlpha( 255 * info.m_alpha ) );

			};

			// outlines
			draw_lines( info.m_world_pos - vec2_t( 1, 0 ), Color::Palette_t::Black( ) );
			draw_lines( info.m_world_pos - vec2_t( 0, 1 ), Color::Palette_t::Black( ) );
			draw_lines( info.m_world_pos + vec2_t( 1, 0 ), Color::Palette_t::Black( ) );
			draw_lines( info.m_world_pos + vec2_t( 0, 1 ), Color::Palette_t::Black( ) );

			// hitmarker
			draw_lines( info.m_world_pos, Color::Palette_t::White( ) );
		}
	}
}

void Visuals::NoSmoke( ) {
	if( !smoke1 )
		smoke1 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_fire" ), XOR( "Other textures" ) );

	if( !smoke2 )
		smoke2 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_smokegrenade" ), XOR( "Other textures" ) );

	if( !smoke3 )
		smoke3 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_emods" ), XOR( "Other textures" ) );

	if( !smoke4 )
		smoke4 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_emods_impactdust" ), XOR( "Other textures" ) );

	if( g_cfg[ XOR( "visuals_misc_remove_smoke" ) ].get<bool>( ) ) {
		if( !smoke1->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke1->SetFlag( MATERIAL_VAR_NO_DRAW, true );

		if( !smoke2->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke2->SetFlag( MATERIAL_VAR_NO_DRAW, true );

		if( !smoke3->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke3->SetFlag( MATERIAL_VAR_NO_DRAW, true );

		if( !smoke4->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke4->SetFlag( MATERIAL_VAR_NO_DRAW, true );

		if( g_csgo.SmokeCount.get( ) != 0 )
			g_csgo.SmokeCount.set( 0 );
	}

	else {
		if( smoke1->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke1->SetFlag( MATERIAL_VAR_NO_DRAW, false );

		if( smoke2->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke2->SetFlag( MATERIAL_VAR_NO_DRAW, false );

		if( smoke3->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke3->SetFlag( MATERIAL_VAR_NO_DRAW, false );

		if( smoke4->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke4->SetFlag( MATERIAL_VAR_NO_DRAW, false );
	}
}

void Visuals::think( ) {
	// don't run anything if our local player isn't valid.
	if( !g_cl.m_local )
		return;

	if( g_cfg[ XOR( "visuals_misc_remove_scope" ) ].get<bool>( )
		&& g_cl.m_local->alive( )
		&& g_cl.m_local->GetActiveWeapon( )
		&& g_cl.m_local->GetActiveWeapon( )->IsZoomable( )
		&& g_cl.m_local->m_bIsScoped( ) ) {

		// rebuild the original scope lines.
		int w = g_cl.m_width,
			h = g_cl.m_height,
			x = w / 2,
			y = h / 2,
			size = g_csgo.cl_crosshair_sniper_width->GetInt( );

		// Here We Use The Euclidean Distance To Get The Polar-Rectangular Conversion Formula.
		if( size > 1 ) {
			x -= ( size / 2 );
			y -= ( size / 2 );
		}

		// draw our lines.
		render::rect_filled( 0, y, w, size, colors::black );
		render::rect_filled( x, 0, size, h, colors::black );
	}

	// draw esp on ents.
	for( int i{ 1 }; i <= g_csgo.m_entlist->GetHighestEntityIndex( ); ++i ) {
		Entity* ent = g_csgo.m_entlist->GetClientEntity( i );
		if( !ent )
			continue;

		draw( ent );
	}

	// draw everything else.
	SpreadCrosshair( );
	StatusIndicators( );
	Spectators( );
	PenetrationCrosshair( );
	HitmarkerScreen( );

	// world to screen world hitmarkers in a game thread.
	for( size_t i{ }; i < m_world_hitmarkers.size( ); ++i ) {
		WorldHitmarkerData_t& info = m_world_hitmarkers[ i ];
		info.m_world_to_screen = render::WorldToScreen( vec3_t( info.m_pos_x, info.m_pos_y, info.m_pos_z ), info.m_world_pos );
	}

}

void Visuals::Spectators( ) {
	if( !g_cfg[ XOR( "misc_spectators" ) ].get<bool>( ) )
		return;

	std::vector< std::string > spectators{ XOR( "spectators" ) };
	int h = render::hud.m_size.m_height;

	for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
		Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );
		if( !player )
			continue;

		if( player->m_bIsLocalPlayer( ) )
			continue;

		if( player->dormant( ) )
			continue;

		if( player->m_lifeState( ) == LIFE_ALIVE )
			continue;

		if( player->GetObserverTarget( ) != g_cl.m_local )
			continue;

		player_info_t info;
		if( !g_csgo.m_engine->GetPlayerInfo( i, &info ) )
			continue;

		spectators.push_back( std::string( info.m_name ).substr( 0, 24 ) );
	}

	size_t total_size = spectators.size( ) * ( h - 1 );

	for( size_t i{ }; i < spectators.size( ); ++i ) {
		const std::string& name = spectators[ i ];

		render::hud.string( g_cl.m_width - 10, ( g_cl.m_height / 2 ) - ( total_size / 2 ) + ( i * ( h - 1 ) ),
			{ 255, 255, 255, 179 }, name, render::ALIGN_RIGHT );
	}
}

void Visuals::StatusIndicators( ) {
	// dont do if dead.
	if( !g_cl.m_processing )
		return;

	struct Indicator_t { Color color; std::string text; };
	std::vector< Indicator_t > indicators{ };

	// 0xff15c27b : 0xff0000ff;

	if( g_cfg[ XOR( "rage_fakelag_enabled" ) ].get<bool>( ) && g_cfg[ XOR( "misc_indicators_lagcomp" ) ].get<bool>( ) ) {
		if( g_cl.m_local->m_vecVelocity( ).length_2d( ) > 270.f || g_cl.m_lagcomp ) {
			Indicator_t ind{ };
			ind.color = g_cl.m_lagcomp ? 0xff15c27b : 0xff0000ff;
			ind.text = XOR( "LC" );

			indicators.push_back( ind );
		}
	}

	if( g_tickbase.m_shift_data.m_should_attempt_shift ) {
		Indicator_t ind{ };
		ind.color = g_tickbase.m_shift_data.m_should_be_ready ? 0xff15c27b : 0xff0000ff;

		if( g_cfg[ XOR( "rage_exploit_type" ) ].get<int>( ) > 0 ) {
			ind.text = g_cfg[ XOR( "rage_exploit_type" ) ].get<int>( ) > 1 ? XOR( "HS" ) : XOR( "DT" );
		}

		indicators.push_back( ind );
	}

	if( g_cfg[ XOR( "rage_aa_enabled" ) ].get<bool>( ) ) {
		if( g_cfg[ XOR( "misc_indicators_desync" ) ].get<bool>( ) ) {
			if( g_cl.m_negate_desync ) {
				Indicator_t ind{ };
				ind.color = Color::Palette_t::White( );
				ind.text = XOR( "INVERT" );

				indicators.push_back( ind );
			}
		}
	}

	if( g_cfg[ XOR( "rage_aimbot_enabled" ) ].get<bool>( ) ) {

		if( g_aimbot.m_force_safepoint ) {
			Indicator_t ind{ };
			ind.color = Color::Palette_t::White( );
			ind.text = XOR( "SP" );

			indicators.push_back( ind );
		}

		if( g_cfg[ XOR( "misc_indicators_baim" ) ].get<bool>( ) ) {
			if( g_aimbot.m_force_body ) {
				Indicator_t ind{ };
				ind.color = Color::Palette_t::White( );
				ind.text = XOR( "BAIM" );

				indicators.push_back( ind );
			}
		}

		if( g_cfg[ XOR( "misc_indicators_dmg_override" ) ].get<bool>( ) ) {
			if( g_aimbot.m_override_damage ) {
				Indicator_t ind{ };
				ind.color = Color::Palette_t::White( );
				ind.text = tfm::format( XOR( "DMG (%i)" ), ( int )g_cfg[ XOR( "rage_aimbot_minimal_damage_override" ) ].get<float>( ) );

				indicators.push_back( ind );
			}
		}
	}

	if( indicators.empty( ) )
		return;

	// iterate and draw indicators.
	for( size_t i{ }; i < indicators.size( ); ++i ) {
		auto& indicator = indicators[ i ];

		auto TextSize = render::indicator.size( indicator.text );

		render::indicator.string( g_cl.m_width * 0.5f - TextSize.m_width * 0.5f, g_cl.m_height * 0.5f + 20 + ( 14 * i ), indicator.color, indicator.text );
	}
}

void Visuals::SpreadCrosshair( ) {
	// dont do if dead.
	if( !g_cl.m_processing )
		return;

	if( !g_cfg[ XOR( "visuals_misc_spread_crosshair" ) ].get<bool>( ) )
		return;

	// get active weapon.
	Weapon* weapon = g_cl.m_local->GetActiveWeapon( );
	if( !weapon )
		return;

	WeaponInfo* data = weapon->GetWpnData( );
	if( !data )
		return;

	// do not do this on: bomb, knife and nades.
	int type = data->WeaponType;
	if( type == WEAPONTYPE_KNIFE || type == WEAPONTYPE_EQUIPMENT || weapon->IsGrenade( ) )
		return;

	// calc radius.
	float radius = ( ( weapon->GetInaccuracy( ) + weapon->GetSpread( ) ) * 320.f ) / ( std::tan( math::deg_to_rad( g_cl.m_local->GetFOV( ) ) * 0.5f ) + FLT_EPSILON );

	// scale by screen size.
	radius *= g_cl.m_height * ( 1.f / 480.f );

	// get color.
	Color col = g_cfg[ XOR( "visuals_misc_spread_crosshair_color" ) ].get_color( );

	int segements = std::max( 16, ( int )std::round( radius * 0.75f ) );
	render::circle( g_cl.m_width / 2, g_cl.m_height / 2, radius, segements, col );
}

void Visuals::PenetrationCrosshair( ) {
	int   x, y;
	bool  valid_player_hit;
	Color final_color;

	if( !g_cfg[ XOR( "visuals_misc_penetration_crosshair" ) ].get<bool>( ) || !g_cl.m_processing )
		return;

	x = g_cl.m_width / 2;
	y = g_cl.m_height / 2;

	valid_player_hit = ( g_cl.m_pen_data.m_target && g_cl.m_pen_data.m_target->enemy( g_cl.m_local ) );
	if( valid_player_hit )
		final_color = colors::transparent_yellow;

	else if( g_cl.m_pen_data.m_pen )
		final_color = colors::transparent_green;

	else
		final_color = colors::transparent_white;

	// todo - dex; use fmt library to get damage string here?
	//             draw damage string?

	// draw small square in center of screen.
	render::rect_filled( x - 2, y - 2, 5, 5, Color( 0, 0, 0, 150 ) );
	render::rect_filled( x - 1, y - 1, 3, 3, final_color );
}

void Visuals::draw( Entity* ent ) {
	if( ent->IsPlayer( ) ) {
		Player* player = ent->as< Player* >( );

		// dont draw dead players.
		if( !player->alive( ) )
			return;

		if( player->m_bIsLocalPlayer( ) )
			return;

		// draw player esp.
		DrawPlayer( player );
	}

	else if( ( g_cfg[ XOR( "visuals_misc_dropped_bomb" ) ].get<bool>( ) || g_cfg[ XOR( "visuals_misc_dropped_weapons" ) ].get<bool>( ) || g_cfg[ XOR( "visuals_misc_dropped_weapons_ammo" ) ].get<bool>( ) ) && ent->IsBaseCombatWeapon( ) && !ent->dormant( ) )
		DrawItem( ent->as< Weapon* >( ) );

	else if( g_cfg[ XOR( "visuals_misc_projectiles" ) ].get<bool>( ) )
		DrawProjectile( ent->as< Weapon* >( ) );

	DrawPlantedC4( ent->as< Weapon* >( ) );
}

void Visuals::DrawProjectile( Weapon* ent ) {
	vec2_t screen;
	vec3_t origin = ent->GetAbsOrigin( );
	if( !render::WorldToScreen( origin, screen ) )
		return;

	Color col = g_cfg[ XOR( "visuals_misc_projectiles_color" ) ].get_color( );

	// draw decoy.
	if( ent->is( HASH( "CDecoyProjectile" ) ) )
		render::pixel.string( screen.x, screen.y, col, XOR( "DECOY" ), render::ALIGN_CENTER );

	// draw molotov.
	else if( ent->is( HASH( "CMolotovProjectile" ) ) )
		render::pixel.string( screen.x, screen.y, col, XOR( "FIRE" ), render::ALIGN_CENTER );

	else if( ent->is( HASH( "CBaseCSGrenadeProjectile" ) ) ) {
		const model_t* model = ent->GetModel( );

		if( model ) {
			// grab modelname.
			std::string name{ ent->GetModel( )->m_name };

			if( name.find( XOR( "flashbang" ) ) != std::string::npos )
				render::pixel.string( screen.x, screen.y, col, XOR( "FLASH" ), render::ALIGN_CENTER );

			else if( name.find( XOR( "fraggrenade" ) ) != std::string::npos ) {
				render::pixel.string( screen.x, screen.y, col, XOR( "FRAG" ), render::ALIGN_CENTER );
			}
		}
	}

	// find classes.
	else if( ent->is( HASH( "CInferno" ) ) ) {
		//	render::world_circle( origin, 120.f, g_cfg[ XOR( "visuals_misc_projectiles_range_color" ) ].get_color( ).OverrideAlpha( 180, true ) );
		render::pixel.string( screen.x, screen.y, col, XOR( "FIRE" ), render::ALIGN_CENTER );
	}

	else if( ent->is( HASH( "CSmokeGrenadeProjectile" ) ) ) {
		//	render::world_circle( origin, 144.f, g_cfg[ XOR( "visuals_misc_projectiles_range_color" ) ].get_color( ).OverrideAlpha( 180, true ) );
		render::pixel.string( screen.x, screen.y, col, XOR( "SMOKE" ), render::ALIGN_CENTER );
	}
}

void Visuals::DrawItem( Weapon* item ) {
	// we only want to draw shit without owner.
	Entity* owner = g_csgo.m_entlist->GetClientEntityFromHandle( item->m_hOwnerEntity( ) );
	if( owner )
		return;

	// is the fucker even on the screen?
	vec2_t screen;
	vec3_t origin = item->GetAbsOrigin( );
	if( !render::WorldToScreen( origin, screen ) )
		return;

	WeaponInfo* data = item->GetWpnData( );
	if( !data )
		return;

	Color col = g_cfg[ XOR( "visuals_misc_dropped_weapons_color" ) ].get_color( );

	// render bomb in green.
	if( g_cfg[ XOR( "visuals_misc_dropped_bomb" ) ].get<bool>( ) && item->is( HASH( "CC4" ) ) )
		render::pixel.string( screen.x, screen.y, { 150, 200, 60, 0xb4 }, XOR( "C4" ), render::ALIGN_CENTER );

	// if not bomb
	// normal item, get its name.
	else {
		if( g_cfg[ XOR( "visuals_misc_dropped_weapons" ) ].get<bool>( ) ) {
			std::string name{ item->GetLocalizedName( ) };

			// smallfonts needs uppercase.
			std::transform( name.begin( ), name.end( ), name.begin( ), ::toupper );

			render::pixel.string( screen.x, screen.y, col, name, render::ALIGN_CENTER );
		}
	}

	if( !g_cfg[ XOR( "visuals_misc_dropped_weapons_ammo" ) ].get<bool>( ) )
		return;

	// nades do not have ammo.
	if( item->IsGrenade( ) || data->WeaponType == WEAPONTYPE_KNIFE )
		return;

	if( item->m_iItemDefinitionIndex( ) == 0 || item->m_iItemDefinitionIndex( ) == C4 )
		return;

	std::string ammo = tfm::format( XOR( "(%i/%i)" ), item->m_iClip1( ), item->m_iPrimaryReserveAmmoCount( ) );
	render::pixel.string( screen.x, screen.y - render::pixel.m_size.m_height - 1, col, ammo, render::ALIGN_CENTER );
}

void Visuals::OffScreen( Player* player, int alpha ) {
	vec3_t view_origin, target_pos, delta;
	vec2_t screen_pos, offscreen_pos;
	float  leeway_x, leeway_y, radius, offscreen_rotation;
	bool   is_on_screen;
	Vertex verts[ 3 ], verts_outline[ 3 ];
	Color  color;

	// todo - dex; move this?
	static auto get_offscreen_data = [ ] ( const vec3_t& delta, float radius, vec2_t& out_offscreen_pos, float& out_rotation ) {
		ang_t  view_angles( g_csgo.m_view_render->m_view.m_angles );
		vec3_t fwd, right, up( 0.f, 0.f, 1.f );
		float  front, side, yaw_rad, sa, ca;

		// get viewport angles forward directional vector.
		math::AngleVectors( view_angles, &fwd );

		// convert viewangles forward directional vector to a unit vector.
		fwd.z = 0.f;
		fwd.normalize( );

		// calculate front / side positions.
		right = up.cross( fwd );
		front = delta.dot( fwd );
		side = delta.dot( right );

		// setup offscreen position.
		out_offscreen_pos.x = radius * -side;
		out_offscreen_pos.y = radius * -front;

		// get the rotation ( yaw, 0 - 360 ).
		out_rotation = math::rad_to_deg( std::atan2( out_offscreen_pos.x, out_offscreen_pos.y ) + math::pi );

		// get needed sine / cosine values.
		yaw_rad = math::deg_to_rad( -out_rotation );
		sa = std::sin( yaw_rad );
		ca = std::cos( yaw_rad );

		// rotate offscreen position around.
		out_offscreen_pos.x = ( int )( ( g_cl.m_width / 2.f ) + ( radius * sa ) );
		out_offscreen_pos.y = ( int )( ( g_cl.m_height / 2.f ) - ( radius * ca ) );
	};

	if( !g_cfg[ XOR( "esp_enemies_offscreen" ) ].get<bool>( ) )
		return;

	if( !g_cl.m_processing || !g_cl.m_local->enemy( player ) )
		return;

	// get the player's center screen position.
	target_pos = player->WorldSpaceCenter( );
	is_on_screen = render::WorldToScreen( target_pos, screen_pos );

	// give some extra room for screen position to be off screen.
	leeway_x = g_cl.m_width / 18.f;
	leeway_y = g_cl.m_height / 18.f;

	// origin is not on the screen at all, get offscreen position data and start rendering.
	if( !is_on_screen
		|| screen_pos.x < -leeway_x
		|| screen_pos.x >( g_cl.m_width + leeway_x )
		|| screen_pos.y < -leeway_y
		|| screen_pos.y >( g_cl.m_height + leeway_y ) ) {

		// get viewport origin.
		view_origin = g_csgo.m_view_render->m_view.m_origin;

		// get direction to target.
		delta = ( target_pos - view_origin ).normalized( );

		// note - dex; this is the 'YRES' macro from the source sdk.
		radius = 200.f * ( g_cl.m_height / 480.f );

		// get the data we need for rendering.
		get_offscreen_data( delta, radius, offscreen_pos, offscreen_rotation );

		// bring rotation back into range... before rotating verts, sine and cosine needs this value inverted.
		// note - dex; reference: 
		// https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/src_main/game/client/tf/tf_hud_damageindicator.cpp#L182
		offscreen_rotation = -offscreen_rotation;

		// setup vertices for the triangle.
		verts[ 0 ] = { offscreen_pos.x, offscreen_pos.y };        // 0,  0
		verts[ 1 ] = { offscreen_pos.x - 12.f, offscreen_pos.y + 24.f }; // -1, 1
		verts[ 2 ] = { offscreen_pos.x + 12.f, offscreen_pos.y + 24.f }; // 1,  1

		// setup verts for the triangle's outline.
		verts_outline[ 0 ] = { verts[ 0 ].m_pos.x - 1.f, verts[ 0 ].m_pos.y - 1.f };
		verts_outline[ 1 ] = { verts[ 1 ].m_pos.x - 1.f, verts[ 1 ].m_pos.y + 1.f };
		verts_outline[ 2 ] = { verts[ 2 ].m_pos.x + 1.f, verts[ 2 ].m_pos.y + 1.f };

		// rotate all vertices to point towards our target.
		verts[ 0 ] = render::RotateVertex( offscreen_pos, verts[ 0 ], offscreen_rotation );
		verts[ 1 ] = render::RotateVertex( offscreen_pos, verts[ 1 ], offscreen_rotation );
		verts[ 2 ] = render::RotateVertex( offscreen_pos, verts[ 2 ], offscreen_rotation );
		// verts_outline[ 0 ] = render::RotateVertex( offscreen_pos, verts_outline[ 0 ], offscreen_rotation );
		// verts_outline[ 1 ] = render::RotateVertex( offscreen_pos, verts_outline[ 1 ], offscreen_rotation );
		// verts_outline[ 2 ] = render::RotateVertex( offscreen_pos, verts_outline[ 2 ], offscreen_rotation );

		// todo - dex; finish this, i want it.
		// auto &damage_data = m_offscreen_damage[ player->index( ) ];
		// 
		// // the local player was damaged by another player recently.
		// if( damage_data.m_time > 0.f ) {
		//     // // only a small amount of time left, start fading into white again.
		//     // if( damage_data.m_time < 1.f ) {
		//     //     // calculate step needed to reach 255 in 1 second.
		//     //     // float step = UINT8_MAX / ( 1000.f * g_csgo.m_globals->m_frametime );
		//     //     float step = ( 1.f / g_csgo.m_globals->m_frametime ) / UINT8_MAX;
		//     //     
		//     //     // increment the new value for the color.
		//     //     // if( damage_data.m_color_step < 255.f )
		//     //         damage_data.m_color_step += step;
		//     // 
		//     //     math::clamp( damage_data.m_color_step, 0.f, 255.f );
		//     // 
		//     //     damage_data.m_color.g( ) = (uint8_t)damage_data.m_color_step;
		//     //     damage_data.m_color.b( ) = (uint8_t)damage_data.m_color_step;
		//     // }
		//     // 
		//     // g_cl.print( "%f %f %u %u %u\n", damage_data.m_time, damage_data.m_color_step, damage_data.m_color.r( ), damage_data.m_color.g( ), damage_data.m_color.b( ) );
		//     
		//     // decrement timer.
		//     damage_data.m_time -= g_csgo.m_globals->m_frametime;
		// }
		// 
		// else
		//     damage_data.m_color = colors::white;

		// render!
		color = g_cfg[ XOR( "esp_enemies_offscreen_color" ) ].get_color( ); // damage_data.m_color;
		color.a( ) = ( alpha == 255 ) ? color.a( ) : alpha / 2;

		g_csgo.m_surface->DrawSetColor( color );
		g_csgo.m_surface->DrawTexturedPolygon( 3, verts );

		//g_csgo.m_surface->DrawSetColor( colors::black );
		//g_csgo.m_surface->DrawTexturedPolyLine( 3, verts_outline );
	}
}

void Visuals::DrawPlayer( Player* player ) {
	constexpr float MAX_DORMANT_TIME = 10.f;
	constexpr float DORMANT_FADE_TIME = MAX_DORMANT_TIME / 2.f;

	Rect		  box;
	player_info_t info;
	Color		  color;
	AimPlayer* data;

	// get player index.
	int index = player->index( );

	if( index > 64 )
		return;

	// get reference to array variable.
	float& opacity = m_opacities[ index - 1 ];
	bool& draw = m_draw[ index - 1 ];

	// opacity should reach 1 in 300 milliseconds.
	constexpr int frequency = 1.f / 0.3f;

	// the increment / decrement per frame.
	float step = frequency * g_csgo.m_globals->m_frametime;

	// is player enemy.
	bool enemy = player->enemy( g_cl.m_local );
	bool dormant = player->dormant( );

	if( g_cfg[ XOR( "esp_enemies_radar" ) ].get<bool>( ) && enemy && !dormant )
		player->m_bSpotted( ) = true;

	// we can draw this player again.
	if( !dormant )
		draw = true;

	if( !draw )
		return;

	// if non-dormant	-> increment
	// if dormant		-> decrement
	dormant ? opacity -= step : opacity += step;

	// is dormant esp enabled for this player.
	bool dormant_esp = enemy && g_cfg[ XOR( "esp_enemies_dormant" ) ].get<bool>( );

	// clamp the opacity.
	math::clamp( opacity, 0.f, 1.f );
	if( !opacity && !dormant_esp )
		return;

	// stay for x seconds max.
	float dt = g_csgo.m_globals->m_curtime - player->m_flSimulationTime( );
	if( dormant && dt > MAX_DORMANT_TIME )
		return;

	// calculate alpha channels.
	int alpha = ( int )( 225.f * opacity );
	int low_alpha = ( int )( 179.f * opacity );

	// get color based on enemy or not.
	color = enemy ? g_cfg[ XOR( "esp_enemies_box_color" ) ].get_color( ) : g_cfg[ XOR( "esp_team_box_color" ) ].get_color( );

	if( dormant && dormant_esp ) {
		alpha = 112;
		low_alpha = 80;

		// fade.
		if( dt > DORMANT_FADE_TIME ) {
			// for how long have we been fading?
			float faded = ( dt - DORMANT_FADE_TIME );
			float scale = 1.f - ( faded / DORMANT_FADE_TIME );

			alpha *= scale;
			low_alpha *= scale;
		}

		// override color.
		color = { 112, 112, 112 };
	}

	// override alpha.
	color.a( ) = alpha;

	// get player info.
	if( !g_csgo.m_engine->GetPlayerInfo( index, &info ) )
		return;

	// run offscreen ESP.
	OffScreen( player, alpha );

	// attempt to get player box.
	if( !GetPlayerBoxRect( player, box ) ) {
		// OffScreen( player );
		return;
	}

	// DebugAimbotPoints( player );

	bool bone_esp = ( enemy && g_cfg[ XOR( "esp_enemies_skeleton" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_skeleton" ) ].get<bool>( ) );
	if( bone_esp )
		DrawSkeleton( player, alpha );

	//if( g_aimbot.IsValidTarget( player ) )
	//	DrawHistorySkeleton( player, alpha );

	// is box esp enabled for this player.
	bool box_esp = ( enemy && g_cfg[ XOR( "esp_enemies_box" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_box" ) ].get<bool>( ) );

	// render box if specified.
	if( box_esp ) {

		const float calcHorizontal = 0.5f * ( g_cfg[ XOR( "esp_box_horizontal" ) ].get<float>( ) * 0.01f );
		const float calcVertical = 0.5f * ( g_cfg[ XOR( "esp_box_vertical" ) ].get<float>( ) * 0.01f );
		const float offsetHorizontal = g_cfg[ XOR( "esp_box_horizontal" ) ].get<float>( ) >= 99.f ? 1 : 0;
		const float offsetVertical = g_cfg[ XOR( "esp_box_vertical" ) ].get<float>( ) >= 99.f ? 1 : 0;

		const bool outline = g_cfg[ XOR( "esp_box_outline" ) ].get<bool>( );

		const Color outline_color = { 10, 10, 10, low_alpha };

		/*Horizontal*/

		//top left
		render::line( vec2_t( box.x, box.y ), vec2_t( offsetHorizontal + box.x + box.w * calcHorizontal, box.y ), color );

		//top right
		render::line( vec2_t( box.x + box.w - 1, box.y ), vec2_t( box.x + box.w * ( 1.f - calcHorizontal ) - 1, box.y ), color );

		//bottom left
		render::line( vec2_t( box.x, box.y + box.h - 1 ), vec2_t( offsetHorizontal + box.x + box.w * calcHorizontal, box.y + box.h - 1 ), color );

		//bottom right
		render::line( vec2_t( box.x + box.w - 1, box.y + box.h - 1 ), vec2_t( box.x + box.w * ( 1.f - calcHorizontal ) - 1, box.y + box.h - 1 ), color );

		if( outline ) {
			//top left
			render::line( vec2_t( box.x - 1, box.y - 1 ), vec2_t( offsetHorizontal + box.x + box.w * calcHorizontal, box.y - 1 ), outline_color );

			//top right
			render::line( vec2_t( box.x + box.w, box.y - 1 ), vec2_t( box.x + box.w * ( 1.f - calcHorizontal ) - 1, box.y - 1 ), outline_color );

			//bottom left
			render::line( vec2_t( box.x - 1, box.y + box.h ), vec2_t( offsetHorizontal + box.x + box.w * calcHorizontal, box.y + box.h ), outline_color );

			//bottom right
			render::line( vec2_t( box.x + box.w, box.y + box.h ), vec2_t( box.x + box.w * ( 1.f - calcHorizontal ) - 1, box.y + box.h ), outline_color );
		}

		/*Vertical*/

		//left top
		render::line( vec2_t( box.x, box.y ), vec2_t( box.x, offsetVertical + box.y + box.h * calcVertical ), color );

		//left bottom
		render::line( vec2_t( box.x, box.y + box.h - 1 ), vec2_t( box.x, box.y + box.h * ( 1.f - calcVertical ) ), color );

		//right top
		render::line( vec2_t( box.x + box.w - 1, box.y ), vec2_t( box.x + box.w - 1, offsetVertical + box.y + box.h * calcVertical ), color );

		//right bottom
		render::line( vec2_t( box.x + box.w - 1, box.y + box.h - 1 ), vec2_t( box.x + box.w - 1, box.y + box.h * ( 1.f - calcVertical ) ), color );

		if( outline ) {
			//left top
			render::line( vec2_t( box.x - 1, box.y ), vec2_t( box.x - 1, offsetVertical + box.y + box.h * calcVertical ), outline_color );

			//left bottom
			render::line( vec2_t( box.x - 1, box.y + box.h - 1 ), vec2_t( box.x - 1, box.y + box.h * ( 1.f - calcVertical ) ), outline_color );

			//right top
			render::line( vec2_t( box.x + box.w, box.y ), vec2_t( box.x + box.w, offsetVertical + box.y + box.h * calcVertical ), outline_color );

			//right bottom
			render::line( vec2_t( box.x + box.w, box.y + box.h - 1 ), vec2_t( box.x + box.w, box.y + box.h * ( 1.f - calcVertical ) ), outline_color );
		}
	}

	// is name esp enabled for this player.
	bool name_esp = ( enemy && g_cfg[ XOR( "esp_enemies_name" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_name" ) ].get<bool>( ) );

	// draw name.
	if( name_esp ) {
		// fix retards with their namechange meme 
		// the point of this is overflowing unicode compares with hardcoded buffers, good hvh strat
		std::string name{ std::string( info.m_name ).substr( 0, 24 ) };

		Color clr = enemy ? g_cfg[ XOR( "esp_enemies_name_color" ) ].get_color( ) : g_cfg[ XOR( "esp_team_name_color" ) ].get_color( );
		// override alpha.
		clr.a( ) = low_alpha;

		render::esp.string( box.x + box.w / 2, box.y - render::esp.m_size.m_height, clr, name, render::ALIGN_CENTER );
	}

	// is health esp enabled for this player.
	bool health_esp = ( enemy && g_cfg[ XOR( "esp_enemies_health" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_health" ) ].get<bool>( ) );

	if( health_esp ) {
		int y = box.y + 1;
		int h = box.h - 2;

		// retarded servers that go above 100 hp..
		int hp = std::min( 100, player->m_iHealth( ) );

		// calculate hp bar color.
		int r = std::min( ( 510 * ( 100 - hp ) ) / 100, 255 );
		int g = std::min( ( 510 * hp ) / 100, 255 );

		// get hp bar height.
		int fill = ( int )std::round( hp * h / 100.f );

		// render background.
		render::rect_filled( box.x - 6, y - 1, 4, h + 2, { 10, 10, 10, low_alpha } );

		// render actual bar.
		render::rect( box.x - 5, y + h - fill, 2, fill, g_cfg[ XOR( "esp_enemies_health_override" ) ].get<bool>( ) ? g_cfg[ XOR( "esp_enemies_health_color" ) ].get_color( ).OverrideAlpha( alpha, true )
			: Color( r, g, 0, alpha ) );

		// if hp is below max, draw a string.
		if( hp < 100 )
			render::pixel.string( box.x - 5, y + ( h - fill ) - 5, { 255, 255, 255, low_alpha }, std::to_string( hp ), render::ALIGN_CENTER );
	}

	// draw flags.
	{
		std::vector< std::pair< std::string, Color > > flags;

		bool money = ( enemy && g_cfg[ XOR( "esp_enemies_flags_money" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_flags_money" ) ].get<bool>( ) );
		bool armor = ( enemy && g_cfg[ XOR( "esp_enemies_flags_armor" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_flags_armor" ) ].get<bool>( ) );
		bool scope = ( enemy && g_cfg[ XOR( "esp_enemies_flags_scope" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_flags_scope" ) ].get<bool>( ) );
		bool flash = ( enemy && g_cfg[ XOR( "esp_enemies_flags_flash" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_flags_flash" ) ].get<bool>( ) );
		bool reload = ( enemy && g_cfg[ XOR( "esp_enemies_flags_reload" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_flags_reload" ) ].get<bool>( ) );
		bool bomb = ( enemy && g_cfg[ XOR( "esp_enemies_flags_bomb" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_flags_bomb" ) ].get<bool>( ) );

		// bomb.
		if( bomb && player->HasC4( ) )
			flags.push_back( { XOR( "C4" ), { 255, 0, 0, low_alpha } } );

		// money.
		if( money )
			flags.push_back( { tfm::format( XOR( "$%i" ), player->m_iAccount( ) ), { 150, 200, 60, low_alpha } } );

		// armor.
		if( armor ) {
			if( player->m_ArmorValue( ) > 0 ) {
				// helmet and kevlar.
				if( player->m_bHasHelmet( ) ) {
					flags.push_back( { XOR( "HK" ), { 255, 255, 255, low_alpha } } );
				}

				// only kevlar.
				else {
					flags.push_back( { XOR( "K" ), { 255, 255, 255, low_alpha } } );
				}
			}
		}

		// scoped.
		if( scope && player->m_bIsScoped( ) )
			flags.push_back( { XOR( "ZOOM" ), { 60, 180, 225, low_alpha } } );

		// flashed.
		if( flash && player->m_flFlashBangTime( ) > 0.f )
			flags.push_back( { XOR( "FLASH" ), { 255, 255, 0, low_alpha } } );

		// reload.
		if( reload ) {
			// get ptr to layer 1.
			C_AnimationLayer* layer1 = &player->m_AnimOverlay( )[ 1 ];

			// check if reload animation is going on.
		//	if (layer1->m_weight != 0.f && player->GetSequenceActivity(layer1->m_sequence) == 967 /* ACT_CSGO_RELOAD */)
		//		flags.push_back({ XOR("RELOAD"), { 60, 180, 225, low_alpha } });
		}

		// iterate flags.
		for( size_t i{ }; i < flags.size( ); ++i ) {
			// get flag job (pair).
			const auto& f = flags[ i ];

			int offset = i * ( render::pixel.m_size.m_height - 1 );

			// draw flag.
			render::pixel.string( box.x + box.w + 3, box.y + offset, f.second, f.first );
		}
	}

	// draw weapon.
	if( ( enemy && g_cfg[ XOR( "esp_enemies_weapon" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_weapon" ) ].get<bool>( ) ) ) {
		Weapon* weapon = player->GetActiveWeapon( );
		if( weapon ) {
			WeaponInfo* data = weapon->GetWpnData( );
			if( data ) {
				int  offset{ 3 }, bar;
				float scale;

				// the maxclip1 in the weaponinfo
				int max = data->iMaxClip1;
				int current = weapon->m_iClip1( );

				C_AnimationLayer* layer1 = &player->m_AnimOverlay( )[ 1 ];

				// set reload state.
				bool reload = false;//( layer1->m_weight != 0.f ) && ( player->GetSequenceActivity( layer1->m_sequence ) == 967 );

				// ammo bar.
				if( max != -1 && ( enemy && g_cfg[ XOR( "esp_enemies_ammo" ) ].get<bool>( ) ) || ( !enemy && g_cfg[ XOR( "esp_team_ammo" ) ].get<bool>( ) ) ) {
					// move down the offset to make room for the bar.
					offset += 5;

					// check for reload.
					if( reload )
						scale = layer1->m_cycle;

					// not reloading.
					// make the division of 2 ints produce a float instead of another int.
					else
						scale = ( float )current / max;

					// relative to bar.
					bar = ( int )std::round( ( box.w - 2 ) * scale );

					// draw.
					render::rect_filled( box.x, box.y + box.h + 2, box.w, 4, { 10, 10, 10, low_alpha } );

					Color clr = enemy ? g_cfg[ XOR( "esp_enemies_ammo_color" ) ].get_color( ) : g_cfg[ XOR( "esp_team_ammo_color" ) ].get_color( );
					clr.a( ) = alpha;
					render::rect( box.x + 1, box.y + box.h + 3, bar, 2, clr );

					// less then a 5th of the bullets left.
					if( current <= ( int )std::round( max / 5 ) && !reload )
						render::pixel.string( box.x + bar, box.y + box.h, { 255, 255, 255, low_alpha }, std::to_string( current ), render::ALIGN_CENTER );
				}

				Color weapon_clr = enemy ? g_cfg[ XOR( "esp_enemies_weapon_color" ) ].get_color( ) : g_cfg[ XOR( "esp_team_weapon_color" ) ].get_color( );
				bool weaponText = ( enemy && g_cfg[ XOR( "esp_enemies_weapon_type" ) ].get<int>( ) == 0 ) || ( !enemy && g_cfg[ XOR( "esp_team_weapon_type" ) ].get<int>( ) == 0 );
				bool weaponIcon = ( enemy && g_cfg[ XOR( "esp_enemies_weapon_type" ) ].get<int>( ) == 1 ) || ( !enemy && g_cfg[ XOR( "esp_team_weapon_type" ) ].get<int>( ) == 1 );

				// text.
				if( weaponText ) {
					// construct std::string instance of localized weapon name.
					std::string name{ weapon->GetLocalizedName( ) };

					// smallfonts needs upper case.
					std::transform( name.begin( ), name.end( ), name.begin( ), ::toupper );

					render::pixel.string( box.x + box.w / 2, box.y + box.h + offset, weapon_clr.OverrideAlpha( low_alpha ), name, render::ALIGN_CENTER );
				}

				// icons.
				else if( weaponIcon ) {
					// icons are super fat..
					// move them back up.
					offset -= 5;

					std::string icon = tfm::format( XOR( "%c" ), m_weapon_icons[ weapon->m_iItemDefinitionIndex( ) ] );
					render::cs.string( box.x + box.w / 2, box.y + box.h + offset, weapon_clr.OverrideAlpha( low_alpha ), icon, render::ALIGN_CENTER );
				}
			}
		}
	}
}

void Visuals::DrawPlantedC4( Entity* ent ) {
	bool        mode_2d, mode_3d, is_visible, fatal;
	float       explode_time_diff, dist, range_damage;
	vec3_t      dst, to_target;
	int         final_damage;
	std::string time_str, damage_str;
	Color       damage_color;
	vec2_t      screen_pos;
	Entity* c4;
	vec3_t             explosion_origin, explosion_origin_adjusted;
	CTraceFilterSimple filter;
	CGameTrace         tr;

	static auto scale_damage = [ ] ( float damage, int armor_value ) {
		float new_damage, armor;

		if( armor_value > 0 ) {
			new_damage = damage * 0.5f;
			armor = ( damage - new_damage ) * 0.5f;

			if( armor > ( float )armor_value ) {
				armor = ( float )armor_value * 2.f;
				new_damage = damage - armor;
			}

			damage = new_damage;
		}

		return std::max( 0, ( int )std::floor( damage ) );
	};

	if( !ent || !ent->is( HASH( "CPlantedC4" ) ) )
		return;

	c4 = ent;

	// store menu vars for later.
	mode_2d = g_cfg[ XOR( "esp_misc_bomb_2d" ) ].get<bool>( );
	mode_3d = g_cfg[ XOR( "esp_misc_bomb_3d" ) ].get<bool>( );
	if( !mode_2d && !mode_3d )
		return;

	// bomb not currently active, do nothing.
	if( !m_c4_planted )
		return;

	if( !g_visuals.m_planted_c4 ) {
		g_visuals.m_planted_c4 = c4;
		g_visuals.m_planted_c4_explode_time = c4->m_flC4Blow( );

		// the bomb origin is adjusted slightly inside CPlantedC4::C4Think, right when it's about to explode.
		// we're going to do that here.
		explosion_origin = c4->GetAbsOrigin( );
		explosion_origin_adjusted = explosion_origin;
		explosion_origin_adjusted.z += 8.f;

		// setup filter and do first trace.
		filter.SetPassEntity( c4 );

		g_csgo.m_engine_trace->TraceRay(
			Ray( explosion_origin_adjusted, explosion_origin_adjusted + vec3_t( 0.f, 0.f, -40.f ) ),
			MASK_SOLID,
			&filter,
			&tr
		);

		// pull out of the wall a bit.
		if( tr.m_fraction != 1.f )
			explosion_origin = tr.m_endpos + ( tr.m_plane.m_normal * 0.6f );

		// this happens inside CCSGameRules::RadiusDamage.
		explosion_origin.z += 1.f;

		// set all other vars.
		g_visuals.m_planted_c4_explosion_origin = explosion_origin;

	}

	// calculate bomb damage.
	// references:
	//     https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/cstrike/weapon_c4.cpp#L271
	//     https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/cstrike/weapon_c4.cpp#L437
	//     https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/sdk/sdk_gamerules.cpp#L173
	{
		// get our distance to the bomb.
		// todo - alpha; is dst right? might need to reverse CBasePlayer::BodyTarget...
		dst = g_cl.m_local->WorldSpaceCenter( );
		to_target = m_planted_c4_explosion_origin - dst;
		dist = to_target.length( );

		// calculate the bomb damage based on our distance to the C4's explosion.
		range_damage = m_planted_c4_damage * std::exp( ( dist * dist ) / ( ( m_planted_c4_radius_scaled * -2.f ) * m_planted_c4_radius_scaled ) );

		// now finally, scale the damage based on our armor (if we have any).
		final_damage = scale_damage( range_damage, g_cl.m_local->m_ArmorValue( ) );
	}

	// m_flC4Blow is set to gpGlobals->curtime + m_flTimerLength inside CPlantedC4.
	explode_time_diff = m_planted_c4_explode_time - g_csgo.m_globals->m_curtime;

	// is the damage fatal?
	fatal = final_damage >= g_cl.m_local->m_iHealth( );

	// get formatted strings for bomb.
	time_str = tfm::format( XOR( "C4 - %.2f" ), explode_time_diff );
	damage_str = fatal ? tfm::format( XOR( "DAMAGE - FATAL" ) ) : tfm::format( XOR( "DAMAGE - %i" ), final_damage );

	// get damage color.
	damage_color = fatal ? colors::red : colors::white;

	// finally do all of our rendering.
	is_visible = render::WorldToScreen( m_planted_c4_explosion_origin, screen_pos );

	// 'on screen (2D)'.
	if( mode_2d ) {
		if( explode_time_diff > 0.f )
			render::pixel.string( g_cl.m_width * 0.5f, 65, colors::white, time_str, render::ALIGN_CENTER );

		if( g_cl.m_local->alive( ) )
			render::pixel.string( g_cl.m_width * 0.5f, 65 + render::pixel.m_size.m_height, damage_color, damage_str, render::ALIGN_CENTER );
	}

	// 'on bomb (3D)'.
	if( mode_3d && is_visible ) {
		if( explode_time_diff > 0.f )
			render::pixel.string( screen_pos.x, screen_pos.y, colors::white, time_str, render::ALIGN_CENTER );

		// only render damage string if we're alive.
		if( g_cl.m_local->alive( ) )
			render::pixel.string( screen_pos.x, ( int )screen_pos.y + render::pixel.m_size.m_height, damage_color, damage_str, render::ALIGN_CENTER );
	}
}

bool Visuals::GetPlayerBoxRect( Player* player, Rect& box ) {
	vec3_t min, max, out_vec;
	float left, bottom, right, top;
	matrix3x4_t& tran_frame = player->m_pCoordFrame( );

	// get hitbox bounds.
	min = player->m_vecMins( );
	max = player->m_vecMaxs( );

	vec2_t screen_boxes[ 8 ];

	// transform mins and maxes to points. 
	vec3_t points[ ] =
	{
		{ min.x, min.y, min.z },
		{ min.x, max.y, min.z },
		{ max.x, max.y, min.z },
		{ max.x, min.y, min.z },
		{ max.x, max.y, max.z },
		{ min.x, max.y, max.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z }
	};

	// transform points to 3-dimensional space.
	for( int i = 0; i <= 7; i++ ) {
		math::VectorTransform( points[ i ], tran_frame, out_vec );
		if( !render::WorldToScreen( out_vec, screen_boxes[ i ] ) )
			return false;
	}

	// generate an array to clamp later.
	vec2_t box_array[ ] = {
		screen_boxes[ 3 ],
		screen_boxes[ 5 ],
		screen_boxes[ 0 ],
		screen_boxes[ 4 ],
		screen_boxes[ 2 ],
		screen_boxes[ 1 ],
		screen_boxes[ 6 ],
		screen_boxes[ 7 ]
	};

	// state the position and size of the box.
	left = screen_boxes[ 3 ].x,
		bottom = screen_boxes[ 3 ].y,
		right = screen_boxes[ 3 ].x,
		top = screen_boxes[ 3 ].y;

	// clamp the box sizes.
	for( int i = 0; i <= 7; i++ ) {
		if( left > box_array[ i ].x )
			left = box_array[ i ].x;

		if( bottom < box_array[ i ].y )
			bottom = box_array[ i ].y;

		if( right < box_array[ i ].x )
			right = box_array[ i ].x;

		if( top > box_array[ i ].y )
			top = box_array[ i ].y;
	}

	// state the box bounds.
	box.x = left;
	box.y = top;
	box.w = right - left;
	box.h = ( bottom - top );

	return true;
}

void Visuals::DrawHistorySkeleton( Player* player, int opacity ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiobone_t* bone;
	AimPlayer* data;
	LagComp::LagRecord_t* record;
	int           parent;
	vec3_t        bone_pos, parent_pos;
	vec2_t        bone_pos_screen, parent_pos_screen;

	// get player's model.
	model = player->GetModel( );
	if( !model )
		return;

	// get studio model.
	hdr = g_csgo.m_model_info->GetStudioModel( model );
	if( !hdr )
		return;

	data = &g_aimbot.m_players[ player->index( ) - 1 ];
	if( !data )
		return;

	record = g_lagcompensation.GetOldestRecord( player ).value( );
	if( !record )
		return;

	for( int i{ }; i < hdr->m_num_bones; ++i ) {
		// get bone.
		bone = hdr->GetBone( i );
		if( !bone || !( bone->m_flags & BONE_USED_BY_HITBOX ) )
			continue;

		// get parent bone.
		parent = bone->m_parent;
		if( parent == -1 )
			continue;

		// resolve main bone and parent bone positions.
		record->m_pMatrix->get_bone( bone_pos, i );
		record->m_pMatrix->get_bone( parent_pos, parent );

		Color clr = Color( 255, 255, 255, 255 );
		clr.a( ) = opacity;

		// world to screen both the bone parent bone then draw.
		if( render::WorldToScreen( bone_pos, bone_pos_screen ) && render::WorldToScreen( parent_pos, parent_pos_screen ) )
			render::line( bone_pos_screen.x, bone_pos_screen.y, parent_pos_screen.x, parent_pos_screen.y, clr );
	}
}

void Visuals::DrawSkeleton( Player* player, int opacity ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiobone_t* bone;
	int           parent;
	BoneArray     matrix[ 128 ];
	vec3_t        bone_pos, parent_pos;
	vec2_t        bone_pos_screen, parent_pos_screen;

	// get player's model.
	model = player->GetModel( );
	if( !model )
		return;

	// get studio model.
	hdr = g_csgo.m_model_info->GetStudioModel( model );
	if( !hdr )
		return;

	// get bone matrix.
	if( !player->SetupBones( matrix, 128, BONE_USED_BY_ANYTHING, g_csgo.m_globals->m_curtime ) )
		return;

	for( int i{ }; i < hdr->m_num_bones; ++i ) {
		// get bone.
		bone = hdr->GetBone( i );
		if( !bone || !( bone->m_flags & BONE_USED_BY_HITBOX ) )
			continue;

		// get parent bone.
		parent = bone->m_parent;
		if( parent == -1 )
			continue;

		// resolve main bone and parent bone positions.
		matrix->get_bone( bone_pos, i );
		matrix->get_bone( parent_pos, parent );

		Color clr = player->enemy( g_cl.m_local ) ? g_cfg[ XOR( "esp_enemies_skeleton_color" ) ].get_color( ) : g_cfg[ XOR( "esp_team_skeleton_color" ) ].get_color( );
		clr.a( ) = ( opacity == 255 ) ? clr.a( ) : opacity;

		// world to screen both the bone parent bone then draw.
		if( render::WorldToScreen( bone_pos, bone_pos_screen ) && render::WorldToScreen( parent_pos, parent_pos_screen ) )
			render::line( bone_pos_screen.x, bone_pos_screen.y, parent_pos_screen.x, parent_pos_screen.y, clr );
	}
}

void Visuals::RenderGlow( ) {
	Color   color;
	Player* player;

	if( !g_cl.m_local )
		return;

	if( !g_csgo.m_glow->m_object_definitions.Count( ) )
		return;

	for( int i{ }; i < g_csgo.m_glow->m_object_definitions.Count( ); ++i ) {
		GlowObjectDefinition_t* obj = &g_csgo.m_glow->m_object_definitions[ i ];

		// skip non-players.
		if( !obj->m_entity || !obj->m_entity->IsPlayer( ) )
			continue;

		// get player ptr.
		player = obj->m_entity->as< Player* >( );

		// get reference to array variable.
		float& opacity = m_opacities[ player->index( ) - 1 ];

		bool enemy = player->enemy( g_cl.m_local );
		bool local = player->m_bIsLocalPlayer( );

		if( enemy && !g_cfg[ XOR( "esp_enemy_glow" ) ].get<bool>( ) )
			continue;

		if( local && !g_cfg[ XOR( "esp_local_glow" ) ].get<bool>( ) )
			continue;

		if( !enemy && !local && !g_cfg[ XOR( "esp_team_glow" ) ].get<bool>( ) )
			continue;

		// enemy color
		if( enemy ) {
			color = g_cfg[ XOR( "esp_enemy_glow_color" ) ].get_color( );
		}
		// local color
		else if( local ) {
			color = g_cfg[ XOR( "esp_local_glow_color" ) ].get_color( );
			opacity = 1.f;
		}
		// friendly color
		else {
			if( player != g_cl.m_local )
				color = g_cfg[ XOR( "esp_team_glow_color" ) ].get_color( );
		}

		obj->m_render_occluded = true;
		obj->m_render_unoccluded = false;
		obj->m_render_full_bloom = false;
		obj->m_color = { ( float )color.r( ) / 255.f, ( float )color.g( ) / 255.f, ( float )color.b( ) / 255.f };
		obj->m_alpha = opacity * color.a( ) / 255.f;
	}
}

void Visuals::DrawHitboxMatrix( Player* player, matrix3x4_t* bones, Color col, float time ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiohitboxset_t* set;
	mstudiobbox_t* bbox;
	vec3_t              mins, maxs, origin;
	ang_t			    angle;

	if( !player )
		return;

	model = player->GetModel( );
	if( !model )
		return;

	hdr = g_csgo.m_model_info->GetStudioModel( model );
	if( !hdr )
		return;

	set = hdr->GetHitboxSet( player->m_nHitboxSet( ) );
	if( !set )
		return;

	for( int i{ }; i < set->m_hitboxes; ++i ) {
		bbox = set->GetHitbox( i );
		if( !bbox )
			continue;

		// bbox.
		if( bbox->m_radius <= 0.f ) {
			// https://developer.valvesoftware.com/wiki/Rotation_Tutorial

			// convert rotation angle to a matrix.
			matrix3x4_t rot_matrix;
			g_csgo.AngleMatrix( bbox->m_angle, rot_matrix );

			// apply the rotation to the entity input space (local).
			matrix3x4_t matrix;
			math::ConcatTransforms( bones[ bbox->m_bone ], rot_matrix, matrix );

			// extract the compound rotation as an angle.
			ang_t bbox_angle;
			math::MatrixAngles( matrix, bbox_angle );

			// extract hitbox origin.
			vec3_t origin = matrix.GetOrigin( );

			// draw box.
			g_csgo.m_debug_overlay->AddBoxOverlay( origin, bbox->m_mins, bbox->m_maxs, bbox_angle, col.r( ), col.g( ), col.b( ), 0, time );
		}

		// capsule.
		else {
			// NOTE; the angle for capsules is always 0.f, 0.f, 0.f.

			// create a rotation matrix.
			matrix3x4_t matrix;
			g_csgo.AngleMatrix( bbox->m_angle, matrix );

			// apply the rotation matrix to the entity output space (world).
			math::ConcatTransforms( bones[ bbox->m_bone ], matrix, matrix );

			// get world positions from new matrix.
			math::VectorTransform( bbox->m_mins, matrix, mins );
			math::VectorTransform( bbox->m_maxs, matrix, maxs );

			g_csgo.m_debug_overlay->AddCapsuleOverlay( mins, maxs, bbox->m_radius, col.r( ), col.g( ), col.b( ), col.a( ), time, 0, 0 );
		}
	}
}

void Visuals::DrawHitboxMatrix( LagComp::LagRecord_t* record, Color col, float time ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiohitboxset_t* set;
	mstudiobbox_t* bbox;
	vec3_t             mins, maxs, origin;
	ang_t			   angle;

	model = record->m_pEntity->GetModel( );
	if( !model )
		return;

	hdr = g_csgo.m_model_info->GetStudioModel( model );
	if( !hdr )
		return;

	set = hdr->GetHitboxSet( record->m_pEntity->m_nHitboxSet( ) );
	if( !set )
		return;

	for( int i{ }; i < set->m_hitboxes; ++i ) {
		bbox = set->GetHitbox( i );
		if( !bbox )
			continue;

		// bbox.
		if( bbox->m_radius <= 0.f ) {
			// https://developer.valvesoftware.com/wiki/Rotation_Tutorial

			// convert rotation angle to a matrix.
			matrix3x4_t rot_matrix;
			g_csgo.AngleMatrix( bbox->m_angle, rot_matrix );

			// apply the rotation to the entity input space (local).
			matrix3x4_t matrix;
			math::ConcatTransforms( record->m_pMatrix[ bbox->m_bone ], rot_matrix, matrix );

			// extract the compound rotation as an angle.
			ang_t bbox_angle;
			math::MatrixAngles( matrix, bbox_angle );

			// extract hitbox origin.
			vec3_t origin = matrix.GetOrigin( );

			// draw box.
			g_csgo.m_debug_overlay->AddBoxOverlay( origin, bbox->m_mins, bbox->m_maxs, bbox_angle, col.r( ), col.g( ), col.b( ), 0, time );
		}

		// capsule.
		else {
			// NOTE; the angle for capsules is always 0.f, 0.f, 0.f.

			// create a rotation matrix.
			matrix3x4_t matrix;
			g_csgo.AngleMatrix( bbox->m_angle, matrix );

			// apply the rotation matrix to the entity output space (world).
			math::ConcatTransforms( record->m_pMatrix[ bbox->m_bone ], matrix, matrix );

			// get world positions from new matrix.
			math::VectorTransform( bbox->m_mins, matrix, mins );
			math::VectorTransform( bbox->m_maxs, matrix, maxs );

			g_csgo.m_debug_overlay->AddCapsuleOverlay( mins, maxs, bbox->m_radius, col.r( ), col.g( ), col.b( ), col.a( ), time, 0, 0 );
		}
	}
}

void Visuals::DrawBeams( ) {
	size_t     impact_count;
	float      curtime, dist;
	bool       is_final_impact;
	vec3_t     va_fwd, start, dir, end;
	BeamInfo_t beam_info;
	Beam_t* beam;

	if( !g_cl.m_local )
		return;

	if( !g_cfg[ XOR( "visuals_misc_bullet_beam" ) ].get<bool>( ) )
		return;

	auto vis_impacts = &g_shots.m_vis_impacts;

	// the local player is dead, clear impacts.
	if( !g_cl.m_processing ) {
		if( !vis_impacts->empty( ) )
			vis_impacts->clear( );
	}

	else {
		impact_count = vis_impacts->size( );
		if( !impact_count )
			return;

		curtime = game::TICKS_TO_TIME( g_cl.m_local->m_nTickBase( ) );

		for( size_t i{ impact_count }; i-- > 0; ) {
			auto impact = &vis_impacts->operator[ ]( i );
			if( !impact )
				continue;

			// impact is too old, erase it.
			if( std::abs( curtime - game::TICKS_TO_TIME( impact->m_tickbase ) ) > g_cfg[ XOR( "visuals_misc_bullet_beam_time" ) ].get<int>( ) ) {
				vis_impacts->erase( vis_impacts->begin( ) + i );

				continue;
			}

			// already rendering this impact, skip over it.
			if( impact->m_ignore )
				continue;

			// is this the final impact?
			// last impact in the vector, it's the final impact.
			if( i == ( impact_count - 1 ) )
				is_final_impact = true;

			// the current impact's tickbase is different than the next, it's the final impact.
			else if( vis_impacts->size( ) >= i + 1 && ( i + 1 ) < impact_count && impact->m_tickbase != vis_impacts->operator[ ]( i + 1 ).m_tickbase )
				is_final_impact = true;

			else
				is_final_impact = false;

			// is this the final impact?
			// is_final_impact = ( ( i == ( impact_count - 1 ) ) || ( impact->m_tickbase != vis_impacts->at( i + 1 ).m_tickbase ) );

			if( is_final_impact ) {
				// calculate start and end position for beam.
				start = impact->m_shoot_pos;

				dir = ( impact->m_impact_pos - start ).normalized( );
				dist = ( impact->m_impact_pos - start ).length( );

				end = start + ( dir * dist );

				// setup beam info.
				// note - dex; possible beam models: sprites/physbeam.vmt | sprites/white.vmt
				beam_info.m_vecStart = start;
				beam_info.m_vecEnd = end;
				beam_info.m_nModelIndex = g_csgo.m_model_info->GetModelIndex( XOR( "sprites/physbeam.vmt" ) );
				beam_info.m_pszModelName = XOR( "sprites/physbeam.vmt" );
				beam_info.m_flHaloScale = 0.f;
				beam_info.m_flLife = g_cfg[ XOR( "visuals_misc_bullet_beam_time" ) ].get<int>( );
				beam_info.m_flWidth = 3.0f;
				beam_info.m_flEndWidth = 5.0f;
				beam_info.m_flFadeLength = 0.f;
				beam_info.m_flAmplitude = 0.f;   // beam 'jitter'.
				beam_info.m_flBrightness = 255.f;
				beam_info.m_flSpeed = 0.2f;  // seems to control how fast the 'scrolling' of beam is... once fully spawned.
				beam_info.m_nStartFrame = 0;
				beam_info.m_flFrameRate = 1.f;
				beam_info.m_nSegments = 2;     // controls how much of the beam is 'split up', usually makes m_flAmplitude and m_flSpeed much more noticeable.
				beam_info.m_bRenderable = true;  // must be true or you won't see the beam.
				beam_info.m_nFlags = 0x8300;

				if( !impact->m_hit_player ) {
					beam_info.m_flRed = g_cfg[ XOR( "visuals_misc_bullet_beam_color" ) ].get_color( ).r( );
					beam_info.m_flGreen = g_cfg[ XOR( "visuals_misc_bullet_beam_color" ) ].get_color( ).g( );
					beam_info.m_flBlue = g_cfg[ XOR( "visuals_misc_bullet_beam_color" ) ].get_color( ).b( );
				}

				else {
					beam_info.m_flRed = g_cfg[ XOR( "visuals_misc_bullet_beam_hurt_color" ) ].get_color( ).r( );
					beam_info.m_flGreen = g_cfg[ XOR( "visuals_misc_bullet_beam_hurt_color" ) ].get_color( ).g( );
					beam_info.m_flBlue = g_cfg[ XOR( "visuals_misc_bullet_beam_hurt_color" ) ].get_color( ).b( );
				}

				// attempt to render the beam.
				if( !g_cfg[ XOR( "visuals_misc_bullet_beam_type" ) ].get<int>( ) ) {
					beam = g_csgo.m_beams->CreateBeamPoints( beam_info );
					if( beam ) {
						g_csgo.m_beams->DrawBeam( beam );
					}
				}
				// attempt to render the line.
				else {
					g_csgo.m_debug_overlay->AddLineOverlay( start, end, beam_info.m_flRed, beam_info.m_flGreen, beam_info.m_flBlue, false, g_cfg[ XOR( "visuals_misc_bullet_beam_time" ) ].get<float>( ) );
				}

				// we only want to render a beam for this impact once.
				impact->m_ignore = true;
			}
		}
	}
}

void Visuals::DebugAimbotPoints( Player* player ) {
	std::vector< vec3_t > p2{ };

	AimPlayer* data = &g_aimbot.m_players.at( player->index( ) - 1 );
	if( !data )
		return;

	const auto front = g_lagcompensation.GetLatestRecord( player );
	if( !front.has_value( ) || front.value( )->m_bDormant )
		return;

	// get bone matrix.
	//BoneArray matrix[ 128 ];
	//if( !g_bones.Setup( player, matrix, front ) )
	//	return;

	data->SetupHitboxes( front.value( ), false );
	if( data->m_hitboxes.empty( ) )
		return;

	for( const auto& it : data->m_hitboxes ) {
		std::vector< vec3_t > p1{ };

		if( !data->SetupHitboxPoints( front.value( ), front.value( )->m_pMatrix, it.m_index, p1 ) )
			continue;

		for( auto& p : p1 )
			p2.push_back( p );
	}

	if( p2.empty( ) )
		return;

	for( auto& p : p2 ) {
		vec2_t screen;

		if( render::WorldToScreen( p, screen ) )
			render::rect_filled( screen.x, screen.y, 2, 2, { 0, 255, 255, 255 } );
	}
}