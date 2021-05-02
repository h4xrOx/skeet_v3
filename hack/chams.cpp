#include "../includes.h"

Chams g_chams{ };;

Chams::model_type_t Chams::GetModelType( const ModelRenderInfo_t& info ) {
	// model name.
	//const char* mdl = info.m_model->m_name;

	std::string mdl{ info.m_model->m_name };

	//static auto int_from_chars = [ mdl ]( size_t index ) {
	//	return *( int* )( mdl + index );
	//};

	// little endian.
	//if( int_from_chars( 7 ) == 'paew' ) { // weap
	//	if( int_from_chars( 15 ) == 'om_v' && int_from_chars( 19 ) == 'sled' )
	//		return model_type_t::arms;
	//
	//	if( mdl[ 15 ] == 'v' )
	//		return model_type_t::view_weapon;
	//}

	//else if( int_from_chars( 7 ) == 'yalp' ) // play
	//	return model_type_t::player;

	if( mdl.find( XOR( "player" ) ) != std::string::npos && info.m_index >= 1 && info.m_index <= 64 )
		return model_type_t::player;

	return model_type_t::invalid;
}

bool Chams::IsInViewPlane( const vec3_t& world ) {
	float w;

	const VMatrix& matrix = g_csgo.m_engine->WorldToScreenMatrix( );

	w = matrix[ 3 ][ 0 ] * world.x + matrix[ 3 ][ 1 ] * world.y + matrix[ 3 ][ 2 ] * world.z + matrix[ 3 ][ 3 ];

	return w > 0.001f;
}

void Chams::SetColor( Color col, IMaterial* mat ) {
	if( mat )
		mat->ColorModulate( col );

	else
		g_csgo.m_render_view->SetColorModulation( col );
}

void Chams::SetAlpha( float alpha, IMaterial* mat ) {
	if( mat )
		mat->AlphaModulate( alpha );

	else
		g_csgo.m_render_view->SetBlend( alpha );
}

void Chams::SetupMaterial( IMaterial* mat, Color col, bool z_flag ) {
	SetColor( col );

	// mat->SetFlag( MATERIAL_VAR_HALFLAMBERT, flags );
	mat->SetFlag( MATERIAL_VAR_ZNEARER, z_flag );
	mat->SetFlag( MATERIAL_VAR_NOFOG, z_flag );
	mat->SetFlag( MATERIAL_VAR_IGNOREZ, z_flag );

	g_csgo.m_studio_render->ForcedMaterialOverride( mat );
}

void Chams::init( ) {
	// find stupid materials.
	debugambientcube = g_csgo.m_material_system->FindMaterial( XOR( "debug/debugambientcube" ), XOR( "Model textures" ) );
	debugambientcube->IncrementReferenceCount( );

	debugdrawflat = g_csgo.m_material_system->FindMaterial( XOR( "debug/debugdrawflat" ), nullptr );
	debugdrawflat->IncrementReferenceCount( );
}

bool Chams::OverridePlayer( int index ) {
	Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( index );
	if( !player )
		return false;

	// always skip the local player in DrawModelExecute.
	// this is because if we want to make the local player have less alpha
	// the static props are drawn after the players and it looks like aids.
	// therefore always process the local player in scene end.
	if( player->m_bIsLocalPlayer( ) )
		return true;

	// see if this player is an enemy to us.
	bool enemy = g_cl.m_local && player->enemy( g_cl.m_local );

	// we have chams on enemies.
	if( enemy && g_cfg[ XOR( "esp_chams_enemies" ) ].get<bool>( ) )
		return true;

	// we have chams on friendly.
	else if( !enemy && g_cfg[ XOR( "esp_chams_team" ) ].get<bool>( ) )
		return true;

	return false;
}

void Chams::RenderFakeChams( ) {
	if( !g_cl.m_processing )
		return;

	if( !g_cfg[ XOR( "esp_chams_desync_enable" ) ].get<bool>( ) )
		return;

	// get color.
	Color color = g_cfg[ XOR( "esp_chams_desync_color" ) ].get_color( );

	// was the matrix properly setup?
	if( g_cl.m_fake_matrix ) {
		// fix model rendering origin.
		for( auto& i : g_cl.m_fake_matrix ) {
			i[ 0 ][ 3 ] += g_cl.m_local->GetRenderOrigin( ).x;
			i[ 1 ][ 3 ] += g_cl.m_local->GetRenderOrigin( ).y;
			i[ 2 ][ 3 ] += g_cl.m_local->GetRenderOrigin( ).z;
		}

		// override blend.
		SetAlpha( color.a( ) / 255.f );

		// set material and color.
		SetupMaterial( g_cfg[ XOR( "esp_chams_desync_material" ) ].get<int>( ) == 0 ? g_chams.debugambientcube : g_chams.debugdrawflat, color, true );

		// backup the bone cache before we fuck with it.
		auto backup_bones = g_cl.m_local->m_BoneCache( ).m_pCachedBones;

		// replace their bone cache with our custom one.
		g_cl.m_local->m_BoneCache( ).m_pCachedBones = g_cl.m_fake_matrix;

		// manually draw the model.
		g_cl.m_local->DrawModel( );

		// reset their bone cache to the previous one.
		g_cl.m_local->m_BoneCache( ).m_pCachedBones = backup_bones;

		// restore.
		g_csgo.m_studio_render->ForcedMaterialOverride( nullptr );
		g_csgo.m_render_view->SetColorModulation( colors::white );
		g_csgo.m_render_view->SetBlend( 1.f );
	}
}

bool Chams::GenerateLerpedMatrix( int index, BoneArray* out ) {
	if( !g_cl.m_processing )
		return false;

	auto player = g_csgo.m_entlist->GetClientEntity<Player*>( index );
	if( !player )
		return false;

	const auto LastRecord = g_lagcompensation.GetViableRecords( player, 1.0f );

	if( !LastRecord.has_value( ) )
		return false;

	const auto& FirstInvalid = LastRecord.value( ).first;
	const auto& LastInvalid = LastRecord.value( ).second;

	if( FirstInvalid->m_bDormant )
		return false;

	if( LastInvalid->m_flSimulationTime - FirstInvalid->m_flSimulationTime > 0.5f )
		return false;

	const auto NextOrigin = LastInvalid->m_vecOrigin;
	const auto curtime = g_csgo.m_globals->m_curtime;

	auto flDelta = 1.f - ( curtime - LastInvalid->m_flInterpTime ) / ( LastInvalid->m_flSimulationTime - FirstInvalid->m_flSimulationTime );
	if( flDelta < 0.f || flDelta > 1.f )
		LastInvalid->m_flInterpTime = curtime;

	flDelta = 1.f - ( curtime - LastInvalid->m_flInterpTime ) / ( LastInvalid->m_flSimulationTime - FirstInvalid->m_flSimulationTime );

	const auto lerp = math::Interpolate( NextOrigin, FirstInvalid->m_vecOrigin, std::clamp( flDelta, 0.f, 1.f ) );

	BoneArray ret[ 128 ];
	memcpy( ret, FirstInvalid->m_pMatrix_Resolved, sizeof( BoneArray[ 128 ] ) );

	for( size_t i{ }; i < 128; ++i ) {
		const auto matrix_delta = math::MatrixGetOrigin( FirstInvalid->m_pMatrix_Resolved[ i ] ) - FirstInvalid->m_vecOrigin;
		math::MatrixSetOrigin( matrix_delta + lerp, ret[ i ] );
	}

	memcpy( out, ret, sizeof( BoneArray[ 128 ] ) );
	return true;
}

void Chams::RenderHistoryChams( int index ) {
	AimPlayer* data;

	Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( index );
	if( !player )
		return;

	if( !g_aimbot.IsValidTarget( player ) )
		return;

	bool enemy = g_cl.m_local && player->enemy( g_cl.m_local );
	if( enemy ) {
		data = &g_aimbot.m_players[ index - 1 ];
		if( !data )
			return;

		// get color.
		Color color = g_cfg[ XOR( "esp_chams_enemy_backtrack_color" ) ].get_color( );

		// override blend.
		SetAlpha( color.a( ) / 255.f );

		// set material and color.
		SetupMaterial( debugdrawflat, color, true );

		// was the matrix properly setup?
		BoneArray arr[ 128 ];
		if( Chams::GenerateLerpedMatrix( index, arr ) ) {
			// backup the bone cache before we fuck with it.
			auto backup_bones = player->m_BoneCache( ).m_pCachedBones;

			// replace their bone cache with our custom one.
			player->m_BoneCache( ).m_pCachedBones = arr;

			// manually draw the model.
			player->DrawModel( );

			// reset their bone cache to the previous one.
			player->m_BoneCache( ).m_pCachedBones = backup_bones;
		}
	}
}

void Chams::RenderResolveChams( int index ) {
	AimPlayer* data;

	Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( index );
	if( !player || !g_cl.m_processing )
		return;

	if( !g_aimbot.IsValidTarget( player ) )
		return;

	bool enemy = g_cl.m_local && player->enemy( g_cl.m_local );
	if( enemy ) {
		data = &g_aimbot.m_players[ index - 1 ];
		if( !data )
			return;

		const auto latest = g_lagcompensation.GetLatestRecord(player);

		if (!latest.has_value())
			return;

		auto record = latest.value();

		// get color.
		Color color = g_cfg[ XOR( "esp_chams_enemy_resolve_color" ) ].get_color( );

		// override blend.
		SetAlpha( color.a( ) / 255.f );

		// set material and color.
		SetupMaterial( debugdrawflat, color, true );

		auto matrix = record->m_pMatrix;

		// was the matrix properly setup?
		if(matrix) {
			// backup the bone cache before we fuck with it.
			auto backup_bones = player->m_BoneCache( ).m_pCachedBones;

			// replace their bone cache with our custom one.
			player->m_BoneCache( ).m_pCachedBones = matrix;

			// manually draw the model.
			player->DrawModel( );

			// reset their bone cache to the previous one.
			player->m_BoneCache( ).m_pCachedBones = backup_bones;
		}
	}
}

bool Chams::DrawModel( uintptr_t ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone ) {
	// store and validate model type.
	model_type_t type = GetModelType( info );
	if( type == model_type_t::invalid )
		return true;

	// is a valid player.
	if( type == model_type_t::player ) {
		// do not cancel out our own calls from SceneEnd
		// also do not cancel out calls from the glow.
		if( !m_running && !g_csgo.m_studio_render->m_pForcedMaterial && OverridePlayer( info.m_index ) )
			return false;
	}

	return true;
}

void Chams::SceneEnd( ) {
	// store and sort ents by distance.
	if( SortPlayers( ) ) {
		// iterate each player and render them.
		for( const auto& p : m_players )
			RenderPlayer( p );
	}

	// restore.
	g_csgo.m_studio_render->ForcedMaterialOverride( nullptr );
	g_csgo.m_render_view->SetColorModulation( colors::white );
	g_csgo.m_render_view->SetBlend( 1.f );
}

void Chams::RenderPlayer( Player* player ) {
	// prevent recruisive model cancelation.
	m_running = true;

	// restore.
	g_csgo.m_studio_render->ForcedMaterialOverride( nullptr );
	g_csgo.m_render_view->SetColorModulation( colors::white );
	g_csgo.m_render_view->SetBlend( 1.f );

	// this is the local player.
	// we always draw the local player manually in drawmodel.
	if( player->m_bIsLocalPlayer( ) ) {
		// render the fake model.
		// RenderFakeChams( );

		if( g_cfg[ XOR( "esp_chams_local_blend_if_scoped" ) ].get<bool>( ) && player->m_bIsScoped( ) )
			SetAlpha( 1.f - ( g_cfg[ XOR( "esp_chams_local_blend_amount" ) ].get<float>( ) / 100.f ) );

		else if( g_cfg[ XOR( "esp_chams_local_enable" ) ].get<bool>( ) ) {

			Color color = g_cfg[ XOR( "esp_chams_local_color" ) ].get_color( );

			SetAlpha( color.a( ) / 255.f );

			// set material and color.
			SetupMaterial( g_cfg[ XOR( "esp_chams_local_material" ) ].get<int>( ) == 0 ? debugambientcube : debugdrawflat, color, false );
		}

		// manually draw the model.
		player->DrawModel( );
	}
	else {
		// check if is an enemy.
		bool enemy = g_cl.m_local && player->enemy( g_cl.m_local );

		if( enemy && g_cfg[ XOR( "esp_chams_enemy_backtrack" ) ].get<bool>( ) ) {
			RenderHistoryChams( player->index( ) );

			g_csgo.m_studio_render->ForcedMaterialOverride( nullptr );
			g_csgo.m_render_view->SetColorModulation( colors::white );
			g_csgo.m_render_view->SetBlend( 1.f );
		}	
		
		if( enemy && g_cfg[ XOR( "esp_chams_enemies" ) ].get<bool>( ) ) {
			if( g_cfg[ XOR( "esp_chams_enemies_invis" ) ].get<bool>( ) ) {

				Color color = g_cfg[ XOR( "esp_chams_enemies_invis_color" ) ].get_color( );

				SetAlpha( color.a( ) / 255.f );
				SetupMaterial( g_cfg[ XOR( "esp_chams_enemies_invis_material" ) ].get<int>( ) == 0 ? debugambientcube : debugdrawflat, color, true );

				player->DrawModel( ); 
			}

			Color color = g_cfg[ XOR( "esp_chams_enemies_color" ) ].get_color( );

			SetAlpha( color.a( ) / 255.f );
			SetupMaterial( g_cfg[ XOR( "esp_chams_enemies_material" ) ].get<int>( ) == 0 ? debugambientcube : debugdrawflat, color, false );

			player->DrawModel( );
		}

		else if( !enemy && g_cfg[ XOR( "esp_chams_team" ) ].get<bool>( ) ) {
			if( g_cfg[ XOR( "esp_chams_team_invis" ) ].get<bool>( ) ) {

				Color color = g_cfg[ XOR( "esp_chams_team_invis_color" ) ].get_color( );

				SetAlpha( color.a( ) / 255.f );
				SetupMaterial( g_cfg[ XOR( "esp_chams_team_invis_material" ) ].get<int>( ) == 0 ? debugambientcube : debugdrawflat, color, true );

				player->DrawModel( );
			}

			Color color = g_cfg[ XOR( "esp_chams_team_color" ) ].get_color( );

			SetAlpha( color.a( ) / 255.f );
			SetupMaterial( g_cfg[ XOR( "esp_chams_team_material" ) ].get<int>( ) == 0 ? debugambientcube : debugdrawflat, color, false );

			player->DrawModel( );
		}
	}

	m_running = false;
}

bool Chams::SortPlayers( ) {
	// lambda-callback for std::sort.
	// to sort the players based on distance to the local-player.
	static auto distance_predicate = [ ] ( Entity* a, Entity* b ) {
		vec3_t local = g_cl.m_local->GetAbsOrigin( );

		// note - dex; using squared length to save out on sqrt calls, we don't care about it anyway.
		float len1 = ( a->GetAbsOrigin( ) - local ).length_sqr( );
		float len2 = ( b->GetAbsOrigin( ) - local ).length_sqr( );

		return len1 < len2;
	};

	// reset player container.
	m_players.clear( );

	// find all players that should be rendered.
	for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
		// get player ptr by idx.
		Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );

		// validate.
		if( !player || !player->IsPlayer( ) || !player->alive( ) || player->dormant( ) )
			continue;

		// do not draw players occluded by view plane.
		if( !IsInViewPlane( player->WorldSpaceCenter( ) ) )
			continue;

		// this player was not skipped to draw later.
		// so do not add it to our render list.
		if( !OverridePlayer( i ) )
			continue;

		m_players.push_back( player );
	}

	// any players?
	if( m_players.empty( ) )
		return false;

	// sorting fixes the weird weapon on back flickers.
	// and all the other problems regarding Z-layering in this shit game.
	std::sort( m_players.begin( ), m_players.end( ), distance_predicate );

	return true;
}