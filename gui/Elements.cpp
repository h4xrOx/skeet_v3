#include <filesystem>
#include "Elements.h"
#include "Framework/GUI.h"


namespace Menu {
	void DrawMenu( ) {
		static bool once = false;
		if( !once ) {
			g_cfg[ XOR( "menu_color" ) ].set_color( Color( 132, 117, 230 ) );
			g_cfg[ XOR( "rage_aa_rotation_range" ) ].set<int>( 1 );

			once = true;
		}

		if( GUI::Form::BeginWindow( XOR( "Pandora" ) ) ) {

			if( GUI::Form::BeginTab( XOR( "A" ), XOR( "Aimbot" ) ) ) {
				if( GUI::Form::BeginSubTab( XOR( "X" ), XOR( "Aimbot" ) ) ) {
					GUI::Group::BeginGroup( XOR( "General" ), vec2_t( 50, 100 ) ); {
						GUI::Controls::Checkbox( XOR( "Enable aimbot" ), XOR( "rage_aimbot_enabled" ) );

						// MAXIMUM FIELD-OF-VIEW LIMIT LM,FAOFAOFOAFOAFOAOFA
						GUI::Controls::Checkbox( XOR( "Angle limit" ), XOR( "rage_aimbot_limit_fov" ) );
						if( g_cfg[ XOR( "rage_aimbot_limit_fov" ) ].get<bool>( ) )
							GUI::Controls::Slider( XOR( "Maximum FOV" ), XOR( "rage_aimbot_limit_fov_amount" ), 0.f, 180.f, XOR( "%.0f" ) );

						GUI::Controls::Dropdown( XOR( "Target selection" ), { XOR( "Closest distance" ), XOR( "Nearest to crosshair" ), XOR( "Highest damage" ), XOR( "Lowest health" ), XOR( "Lowest lag" ), XOR( "Best height" ) }, XOR( "rage_aimbot_target_selection" ) );

						GUI::Controls::Checkbox( XOR( "Silent aim" ), XOR( "rage_aimbot_silent_aim" ) );
						GUI::Controls::Checkbox( XOR( "Auto fire" ), XOR( "rage_aimbot_autofire" ) );

						GUI::Controls::Dropdown(XOR("Prefered hitbox"), { XOR("Head"), XOR("Body"), XOR("Stomach") }, XOR("rage_aimbot_prefered_hitbox"));

						std::vector<MultiItem_t> hitboxes = {
							{ XOR( "Head" ), XOR( "rage_aimbot_hitbox_head" ) },
							{ XOR( "Neck" ), XOR( "rage_aimbot_hitbox_neck" ) },
							{ XOR( "Chest" ), XOR( "rage_aimbot_hitbox_chest" ) },
							{ XOR( "Arms" ), XOR( "rage_aimbot_hitbox_arms" ) },
							{ XOR( "Stomach" ), XOR( "rage_aimbot_hitbox_stomach" ) },
							{ XOR( "Legs" ), XOR( "rage_aimbot_hitbox_legs" ) },
							{ XOR( "Feet" ), XOR( "rage_aimbot_hitbox_feet" ) },
						};

						std::vector<MultiItem_t> multipoint = {
							{ XOR( "Head" ), XOR( "rage_aimbot_multipoint_head" ) },
							{ XOR( "Chest" ), XOR( "rage_aimbot_multipoint_chest" ) },
							{ XOR( "Stomach" ), XOR( "rage_aimbot_multipoint_stomach" ) },
							{ XOR( "Legs" ), XOR( "rage_aimbot_multipoint_legs" ) },
							{ XOR( "Feet" ), XOR( "rage_aimbot_multipoint_feet" ) },
						};

						GUI::Controls::MultiDropdown( XOR( "Hitboxes" ), hitboxes );
						GUI::Controls::MultiDropdown( XOR( "Multipoints" ), multipoint );

						GUI::Controls::Checkbox(XOR("Ignore limbs when moving"), XOR("rage_aimbot_ignore_limbs"));

						GUI::Controls::Slider( XOR( "Multipoint scale" ), XOR( "rage_aimbot_multipoint_scale" ), 0.f, 100.f, XOR( "%.0f" ) );
						GUI::Controls::Slider( XOR( "Stomach scale" ), XOR( "rage_aimbot_stomach_scale" ), 0.f, 100.f, XOR( "%.0f" ) );

						GUI::Controls::Checkbox(XOR("Prefer safepoint"), XOR("rage_aimbot_prefer_safepoint"));

						GUI::Controls::Label(XOR("Force safepoint"));
						GUI::Controls::Hotkey(XOR("Safepoint"), XOR("rage_safepoint_key"));

						GUI::Controls::Checkbox( XOR( "Hitchance" ), XOR( "rage_aimbot_hitchance" ) );
						if( g_cfg[ XOR( "rage_aimbot_hitchance" ) ].get<bool>( ) )
							GUI::Controls::Slider( XOR( "Minimal hitchance" ), XOR( "rage_aimbot_hitchance_amount" ), 1.f, 100.f, XOR( "%.0f" ) );

						GUI::Controls::Slider( XOR( "Minimal damage" ), XOR( "rage_aimbot_minimal_damage" ), 1.f, 100.f, XOR( "%.0f" ) );
						GUI::Controls::Checkbox( XOR( "Scale damage on health" ), XOR( "rage_aimbot_minimal_damage_hp" ) );

						GUI::Controls::Label( XOR( "Minimal damage override" ) );
						GUI::Controls::Hotkey( XOR( "Minimal damage override key" ), XOR( "rage_aimbot_minimal_damage_override_key" ) );
						GUI::Controls::Slider( XOR( "Damage override amount" ), XOR( "rage_aimbot_minimal_damage_override" ), 1.f, 100.f, XOR( "%.0f" ) );
						GUI::Controls::Checkbox( XOR( "Override penetration damage" ), XOR( "rage_aimbot_minimal_damage_override_pen" ) );

						GUI::Controls::Checkbox( XOR( "Wall penetration" ), XOR( "rage_aimbot_penetration" ) );

						if( g_cfg[ XOR( "rage_aimbot_penetration" ) ].get<bool>( ) ) {
							GUI::Controls::Slider( XOR( "Minimal penetration damage" ), XOR( "rage_aimbot_penetration_minimal_damage" ), 0.f, 100.f, XOR( "%.0f" ) );
							GUI::Controls::Checkbox( XOR( "Scale penetration damage on health" ), XOR( "rage_aimbot_penetration_minimal_damage_hp" ) );
						}

						GUI::Controls::Checkbox( XOR( "Aimbot with knife" ), XOR( "misc_knifebot" ) );

					}GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Accuracy" ), vec2_t( 50, 100 ) ); {
						std::vector<MultiItem_t> baim_prefer = {
							{ XOR( "Always" ), XOR( "rage_aimbot_baim_prefer_always" ) },
							{ XOR( "Lethal" ), XOR( "rage_aimbot_baim_prefer_lethal" ) },
							{ XOR( "Lethal x2" ), XOR( "rage_aimbot_baim_prefer_lethalx2" ) },
							{ XOR( "In-Air" ), XOR( "rage_aimbot_baim_prefer_air" ) },
						};

						std::vector<MultiItem_t> baim_always = {
							{ XOR( "Always" ), XOR( "rage_aimbot_baim_always" ) },
							{ XOR( "Below health" ), XOR( "rage_aimbot_baim_always_health" ) },
							{ XOR( "In-Air" ), XOR( "rage_aimbot_baim_always_air" ) },
						};

						//	std::vector<MultiItem_t> auto_stop = {
						//		{ XOR("Slow motion"), XOR("rage_aimbot_auto_stop_walk") },
						//		{ XOR("Full"), XOR("rage_aimbot_auto_stop_type_full") },
						//	};

						GUI::Controls::Dropdown( XOR( "Automatic scope" ), { XOR( "None" ), XOR( "Always" ), XOR( "Hitchance fail" ) }, XOR( "rage_aimbot_auto_scope" ) );
						GUI::Controls::Checkbox( XOR( "Automatic stop" ), XOR( "rage_aimbot_auto_stop" ) );
						//	if (g_cfg[XOR("rage_aimbot_auto_stop")].get<bool>()) {
						//		GUI::Controls::MultiDropdown(XOR("Automatic stop options"), auto_stop);
						//	}

							//GUI::Controls::Checkbox( XOR( "Extended backtrack" ), XOR( "rage_extended_backtrack" ) );
						GUI::Controls::Checkbox( XOR( "Remove recoil" ), XOR( "rage_aimbot_norecoil" ) );

						if( g_cfg[ XOR( "cheat_mode" ) ].get<int>( ) == 1 )
							GUI::Controls::Checkbox( XOR( "Remove spread" ), XOR( "rage_aimbot_nospread" ) );

						GUI::Controls::MultiDropdown( XOR( "Prefer body-aim conditions" ), baim_prefer );

						GUI::Controls::MultiDropdown( XOR( "Force body-aim conditions" ), baim_always );
						if( g_cfg[ XOR( "rage_aimbot_baim_always_health" ) ].get<bool>( ) )
							GUI::Controls::Slider( XOR( "Below health amount" ), XOR( "rage_aimbot_baim_always_health_amount" ), 0.f, 100.f, XOR( "%.0f" ) );
						
						GUI::Controls::Label( XOR( "Force body-aim key" ) );
						GUI::Controls::Hotkey( XOR( "Force body-aim" ), XOR( "rage_aimbot_baim_key" ) );

						GUI::Controls::Dropdown( XOR( "Exploit" ), { XOR( "None" ), XOR( "Double tap" ), XOR("Hide shot") }, XOR( "rage_exploit_type" ) );
						if( g_cfg[ XOR( "rage_exploit_type" ) ].get<int>( ) > 0 ) {
							GUI::Controls::Label( XOR( "Exploit key" ) );
							GUI::Controls::Hotkey( XOR( "Exploit##key" ), XOR( "rage_exploit_key" ) );

							if (g_cfg[XOR("rage_exploit_type")].get<int>() == 2)
								GUI::Controls::Checkbox(XOR("Wait for charge"), XOR("rage_exploit_charged"));
		
							//GUI::Controls::Label( XOR( "Manual recharge key" ) );
							//GUI::Controls::Hotkey( XOR( "Manual recharge" ), XOR( "rage_exploit_recharge" ), false );
						}

					}GUI::Group::EndGroup( );
				}
				if( GUI::Form::BeginSubTab( XOR( "X" ), XOR( "Anti-aim" ) ) ) {
					GUI::Group::BeginGroup( XOR( "Real anti-aim" ), vec2_t( 50, 100 ) ); {
						GUI::Controls::Checkbox( XOR( "Enable##anti-aim" ), XOR( "rage_aa_enabled" ) );

						GUI::Controls::Dropdown( XOR( "Pitch" ), { XOR( "None" ), XOR( "Down" ), XOR( "Up" ), XOR( "Random" ), XOR( "Ideal" ) }, XOR( "rage_aa_pitch" ) );

						GUI::Controls::Dropdown( XOR( "Static angle" ), { XOR( "None" ), XOR( "Away crosshair" ), XOR( "Away distance" ) }, XOR( "rage_aa_yaw_base" ) );
						GUI::Controls::Dropdown( XOR( "Yaw#real" ), { XOR( "None" ), XOR( "Direction" ), XOR( "Random" ) }, XOR( "rage_aa_yaw" ) );

						auto yaw = g_cfg[ XOR( "rage_aa_yaw" ) ].get<int>( );
						// yaw types.
						if( yaw > 0 ) {
							if( yaw == 1 )
								GUI::Controls::Dropdown( XOR( "Direction" ), { XOR( "Backwards" ), XOR( "Left" ), XOR( "Right" ) }, XOR( "rage_aa_yaw_direction" ) );

							// random.
							if( yaw == 2 ) {
								GUI::Controls::Slider( XOR( "Random update" ), XOR( "rage_aa_random_update" ), 0.f, 1.f, XOR( "%.1fs" ) );
							}

							GUI::Controls::Dropdown( XOR( "Jitter type" ), { XOR( "None" ), XOR( "Offset" ), XOR( "Random" ) }, XOR( "rage_aa_jitter_type" ) );
							GUI::Controls::Slider( XOR( "Jitter range" ), XOR( "rage_aa_jitter_range" ), 0, 361.f, XOR( "%.0f" ) );

							GUI::Controls::Checkbox( XOR( "Rotation" ), XOR( "rage_aa_rotation_enabled" ) );

							if( g_cfg[ XOR( "rage_aa_rotation_enabled" ) ].get<bool>( ) ) {
								GUI::Controls::Slider( XOR( "Rotate range" ), XOR( "rage_aa_rotation_range" ), 1.f, 361.f, XOR( "%.0f" ) );
								GUI::Controls::Slider( XOR( "Rotate speed" ), XOR( "rage_aa_rotation_speed" ), 0.f, 100.f, XOR( "%.0f" ) );
							}
						}

					}GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Fake anti-aim" ), vec2_t( 50, 50 ) ); {
						std::vector<MultiItem_t> real_around_fake = {
							{ XOR( "Standing" ), XOR( "rage_aa_real_around_fake_standing" ) },
							{ XOR( "Moving" ), XOR( "rage_aa_real_around_fake_moving" ) },
							{ XOR( "In-Air" ), XOR( "rage_aa_real_around_fake_air" ) },
							{ XOR( "Slow motion" ), XOR( "rage_aa_real_around_fake_slow_motion" ) },
						};

						GUI::Controls::Dropdown( XOR( "Yaw#fake" ), { XOR( "Off" ), XOR( "Static" ) }, XOR( "rage_aa_yaw_fake" ) );
						GUI::Controls::MultiDropdown( XOR( "Real around fake conditions" ), real_around_fake );
						GUI::Controls::Checkbox( XOR( "Real around fake jitter" ), XOR( "rage_aa_real_around_fake_jitter" ) );

						GUI::Controls::Slider( XOR( "Fake amount" ), XOR( "rage_aa_custom_fake" ), -60.f, 60.f, XOR( "%.0f%%" ) );

						GUI::Controls::Label( XOR( "Invert" ) );
						GUI::Controls::Hotkey( XOR( "Negate key" ), XOR( "rage_aa_negate_key" ) );

					}GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Fake-lag" ), vec2_t( 50, 50 ) ); {
						GUI::Controls::Checkbox( XOR( "Enable##fake-lag" ), XOR( "rage_fakelag_enabled" ) );

						std::vector<MultiItem_t> fakelag_activation = {
							{ XOR( "Standing" ), XOR( "rage_fakelag_standing_enabled" ) },
							{ XOR( "Moving" ), XOR( "rage_fakelag_move_enabled" ) },
							{ XOR( "In air" ), XOR( "rage_fakelag_air_enabled" ) },
							{ XOR( "Crouch" ), XOR( "rage_fakelag_crouch_enabled" ) },
						};

						GUI::Controls::MultiDropdown( XOR( "Fake-lag activation" ), fakelag_activation );

						GUI::Controls::Dropdown( XOR( "Fake-lag mode" ), { XOR( "Maximum" ), XOR( "Break" ), XOR( "Random" ), XOR( "Break step" ) }, XOR( "rage_fakelag_type" ) );

						GUI::Controls::Slider( XOR( "Fake-lag limit" ), XOR( "rage_fakelag_limit" ), 2, 15, XOR( "%.0f%%" ) );

						GUI::Controls::Checkbox( XOR( "Avoid fakelag on-land" ), XOR( "rage_fakelag_on_land" ) );

					}GUI::Group::EndGroup( );

				}
			}
			if( GUI::Form::BeginTab( XOR( "B" ), XOR( "Visuals" ) ) ) {
				if( GUI::Form::BeginSubTab( XOR( "X" ), XOR( "General" ) ) ) {
					GUI::Group::BeginGroup( XOR( "Player ESP" ), vec2_t( 50, 100 ) ); {

						GUI::Controls::Dropdown( XOR( "Group" ), { XOR( "Enemy" ), XOR( "Team" ) }, XOR( "menu_esp_group" ) );

						std::vector<MultiItem_t> Flags_Enemy = {
							{ XOR( "Money" ), XOR( "esp_enemies_flags_money" ) },
							{ XOR( "Armor" ), XOR( "esp_enemies_flags_armor" ) },
							{ XOR( "Scoped" ), XOR( "esp_enemies_flags_scope" ) },
							//	{XOR("Flashed"), XOR("esp_enemies_flags_flash")},
							//	{XOR("Reloading"), XOR("esp_enemies_flags_reload")},
							{ XOR( "Bomb" ), XOR( "esp_enemies_flags_bomb" ) },
						};

						std::vector<MultiItem_t> Flags_Team = {
							{ XOR( "Money" ), XOR( "esp_team_flags_money" ) },
							{ XOR( "Armor" ), XOR( "esp_team_flags_armor" ) },
							{ XOR( "Scoped" ), XOR( "esp_team_flags_scope" ) },
							//	{XOR("Flashed"), XOR("esp_team_flags_flash")},
							//	{XOR("Reloading"), XOR("esp_team_flags_reload")},
							{ XOR( "Bomb" ), XOR( "esp_team_flags_bomb" ) },
						};

						GUI::Controls::Slider( XOR( "Box horizontal" ), XOR( "esp_box_horizontal" ), 0.f, 100.f );
						GUI::Controls::Slider( XOR( "Box vertical" ), XOR( "esp_box_vertical" ), 0.f, 100.f );
						GUI::Controls::Checkbox( XOR( "Box outline" ), XOR( "esp_box_outline" ) );

						switch( g_cfg[ XOR( "menu_esp_group" ) ].get<int>( ) ) {
						case 0:

							GUI::Controls::Checkbox( XOR( "Dormant" ), XOR( "esp_enemies_dormant" ) );

							//	if (g_cfg[XOR("esp_enemies_offscreen")].get<bool>()) {
							//		GUI::Controls::Slider(XOR("Offscreen radius"), XOR("esp_offscreen_radius"), 0.f, 1000.f);
							//	}

							GUI::Controls::Checkbox( XOR( "Box#Enemy" ), XOR( "esp_enemies_box" ) );
							GUI::Controls::ColorPicker( XOR( "Box color#Enemy" ), XOR( "esp_enemies_box_color" ) );

							GUI::Controls::Checkbox( XOR( "Offscreen" ), XOR( "esp_enemies_offscreen" ) );
							GUI::Controls::ColorPicker( XOR( "Offscreen color" ), XOR( "esp_enemies_offscreen_color" ) );

							GUI::Controls::Checkbox( XOR( "Name#Enemy" ), XOR( "esp_enemies_name" ) );
							GUI::Controls::ColorPicker( XOR( "Name color#Enemy" ), XOR( "esp_enemies_name_color" ) );

							GUI::Controls::Checkbox( XOR( "Health#Enemy" ), XOR( "esp_enemies_health" ) );
							GUI::Controls::Checkbox( XOR( "Health color override#Enemy" ), XOR( "esp_enemies_health_override" ) );
							GUI::Controls::ColorPicker( XOR( "Health color#Enemy" ), XOR( "esp_enemies_health_color" ) );

							GUI::Controls::MultiDropdown( XOR( "Flags#Enemy" ), Flags_Enemy );

							GUI::Controls::Checkbox( XOR( "Weapon#Enemy" ), XOR( "esp_enemies_weapon" ) );
							GUI::Controls::ColorPicker( XOR( "Weapon color#Enemy" ), XOR( "esp_enemies_weapon_color" ) );

							GUI::Controls::Dropdown( XOR( "Weapon type#Enemy" ), { XOR( "Text" ), XOR( "Icons" ) }, XOR( "esp_enemies_weapon_type" ) );

							GUI::Controls::Checkbox( XOR( "Ammo#Enemy" ), XOR( "esp_enemies_ammo" ) );
							GUI::Controls::ColorPicker( XOR( "Ammo color#Enemy" ), XOR( "esp_enemies_ammo_color" ) );

							GUI::Controls::Checkbox( XOR( "Skeleton#Enemy" ), XOR( "esp_enemies_skeleton" ) );
							GUI::Controls::ColorPicker( XOR( "Skeleton color#Enemy" ), XOR( "esp_enemies_skeleton_color" ) );

							break;
						case 1:

							GUI::Controls::Checkbox( XOR( "Box#Team" ), XOR( "esp_team_box" ) );
							GUI::Controls::ColorPicker( XOR( "Box color#Team" ), XOR( "esp_team_box_color" ) );

							GUI::Controls::Checkbox( XOR( "Name#Team" ), XOR( "esp_team_name" ) );
							GUI::Controls::ColorPicker( XOR( "Name color#Team" ), XOR( "esp_team_name_color" ) );

							GUI::Controls::Checkbox( XOR( "Health#Team" ), XOR( "esp_team_health" ) );
							GUI::Controls::MultiDropdown( XOR( "Flags#Team" ), Flags_Team );

							GUI::Controls::Checkbox( XOR( "Weapon#Team" ), XOR( "esp_team_weapon" ) );
							GUI::Controls::ColorPicker( XOR( "Weapon color#Team" ), XOR( "esp_team_weapon_color" ) );

							GUI::Controls::Dropdown( XOR( "Weapon type#Team" ), { XOR( "Text" ), XOR( "Icons" ) }, XOR( "esp_team_weapon_type" ) );

							GUI::Controls::Checkbox( XOR( "Ammo#Team" ), XOR( "esp_team_ammo" ) );
							GUI::Controls::ColorPicker( XOR( "Ammo color#Team" ), XOR( "esp_team_ammo_color" ) );

							GUI::Controls::Checkbox( XOR( "Skeleton#Team" ), XOR( "esp_team_skeleton" ) );
							GUI::Controls::ColorPicker( XOR( "Skeleton color#Team" ), XOR( "esp_team_skeleton_color" ) );

							break;
						}


					}GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Other ESP" ), vec2_t( 50, 50 ) ); {

						std::vector<MultiItem_t> Bomb = {
							{ XOR( "Dropped" ), XOR( "visuals_misc_dropped_bomb" ) },
							{ XOR( "On screen (2D)" ), XOR( "esp_misc_bomb_2d" ) },
							{ XOR( "On bomb (3D)" ), XOR( "esp_misc_bomb_3d" ) },
						};

						std::vector<MultiItem_t> DroppedWeapon = {
							{ XOR( "Text" ), XOR( "visuals_misc_dropped_weapons" ) },
							{ XOR( "Ammo text" ), XOR( "visuals_misc_dropped_weapons_ammo" ) },
						};

						GUI::Controls::Checkbox( XOR( "Radar" ), XOR( "esp_enemies_radar" ) );

						GUI::Controls::Checkbox( XOR( "Force crosshair" ), XOR( "misc_force_crosshair" ) );
						GUI::Controls::Checkbox( XOR( "Penetration crosshair" ), XOR( "visuals_misc_penetration_crosshair" ) );

						GUI::Controls::MultiDropdown( XOR( "Dropped weapon" ), DroppedWeapon );
						GUI::Controls::ColorPicker( XOR( "Dropped weapon color" ), XOR( "visuals_misc_dropped_weapons_color" ) );

						GUI::Controls::Checkbox( XOR( "Grenades" ), XOR( "visuals_misc_projectiles" ) );
						GUI::Controls::ColorPicker( XOR( "Grenades color" ), XOR( "visuals_misc_projectiles_color" ) );

						GUI::Controls::MultiDropdown( XOR( "Bomb ESP" ), Bomb );

					}GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Effects" ), vec2_t( 50, 50 ) ); {

						GUI::Controls::Checkbox( XOR( "Remove scope overlay" ), XOR( "visuals_misc_remove_scope" ) );
						GUI::Controls::Checkbox( XOR( "Remove visual recoil" ), XOR( "visuals_misc_remove_visual_recoil" ) );

						GUI::Controls::Checkbox( XOR( "Remove smoke effects" ), XOR( "visuals_misc_remove_smoke" ) );
						GUI::Controls::Checkbox( XOR( "Remove flash effects" ), XOR( "visuals_misc_remove_flash" ) );
						GUI::Controls::Checkbox( XOR( "Remove fog effects" ), XOR( "visuals_misc_remove_fog" ) );

						GUI::Controls::Checkbox( XOR( "Remove teammate rendering" ), XOR( "visuals_misc_no_draw_team" ) );

						GUI::Controls::Checkbox( XOR( "Grenade trajectory" ), XOR( "visuals_misc_grenade_prediction" ) );
						GUI::Controls::ColorPicker( XOR( "Grenade trajectory color" ), XOR( "visuals_misc_grenade_prediction_color1" ) );

						std::vector<MultiItem_t> World = {
							{ XOR( "Night mode" ), XOR( "misc_world_night" ) },
							{ XOR( "Fullbright" ), XOR( "misc_world_fullbright" ) },
						};

						GUI::Controls::MultiDropdown( XOR( "Brightness adjustements" ), World );

						if( g_cfg[ XOR( "misc_world_night" ) ].get<bool>( ) ) {
							GUI::Controls::Slider( XOR( "Night mode darkness" ), XOR( "misc_world_night_darkness" ), 0.f, 100.f, XOR( "%.0f%%" ) );
						}

						//	GUI::Controls::Slider(XOR("Prop opacity"), XOR("misc_world_prop_opacity"), 0.f, 100.f, XOR("%.0f%%"));

						GUI::Controls::Label( XOR( "Third person" ) );
						GUI::Controls::Hotkey( XOR( "Third person" ), XOR( "misc_thirdperson_key" ) );
						GUI::Controls::Slider( XOR( "Third person distance" ), XOR( "misc_thirdperson_distance" ), 0.f, 200.f );

						GUI::Controls::Checkbox( XOR( "Bullet tracers" ), XOR( "visuals_misc_bullet_beam" ) );

						if( g_cfg[ XOR( "visuals_misc_bullet_beam" ) ].get<bool>( ) ) {
							GUI::Controls::Dropdown( XOR( "Bullet tracers type" ), { XOR( "Beam" ), XOR( "Line" ) }, XOR( "visuals_misc_bullet_beam_type" ) );

							GUI::Controls::Slider( XOR( "Bullet tracers duration" ), XOR( "visuals_misc_bullet_beam_time" ), 1.f, 15.f, XOR( "%.0fs" ) );

							GUI::Controls::Label( XOR( "Bullet tracers color" ) );
							GUI::Controls::ColorPicker( XOR( "Bullet tracers colorr" ), XOR( "visuals_misc_bullet_beam_color" ) );
							GUI::Controls::Label( XOR( "Bullet tracers hurt color" ) );
							GUI::Controls::ColorPicker( XOR( "Bullet tracers hurt colorr" ), XOR( "visuals_misc_bullet_beam_hurt_color" ) );
						}

					}GUI::Group::EndGroup( );
				}

				if( GUI::Form::BeginSubTab( XOR( "X" ), XOR( "Player models" ) ) ) {
					GUI::Group::BeginGroup( XOR( "Player ESP" ), vec2_t( 50, 100 ) ); {

						GUI::Controls::Dropdown( XOR( "Group" ), { XOR( "Local" ), XOR( "Enemy" ), XOR( "Team" ) }, XOR( "menu_esp_group_chams" ) );

						std::vector<std::string> Materials = {
							{ XOR( "Default" ) },
							{ XOR( "Flat" ) }
						};

						switch( g_cfg[ XOR( "menu_esp_group_chams" ) ].get<int>( ) ) {
						case 0:

							GUI::Controls::Checkbox( XOR( "Glow#Local" ), XOR( "esp_local_glow" ) );
							GUI::Controls::ColorPicker( XOR( "Glow color#Local" ), XOR( "esp_local_glow_color" ) );

							GUI::Controls::Checkbox( XOR( "Colored model#Local" ), XOR( "esp_chams_local_enable" ) );
							GUI::Controls::ColorPicker( XOR( "Colored model color#Local" ), XOR( "esp_chams_local_color" ) );

							if( g_cfg[ XOR( "esp_chams_local_enable" ) ].get<bool>( ) ) {
								GUI::Controls::Dropdown( XOR( "Colored model material#Local" ), Materials, XOR( "esp_chams_local_material" ) );
							}

							GUI::Controls::Checkbox( XOR( "Blend if scoped" ), XOR( "esp_chams_local_blend_if_scoped" ) );
							GUI::Controls::Slider( XOR( "Blend amount" ), XOR( "esp_chams_local_blend_amount" ), 0.f, 100.f, XOR( "%.0f%%" ) );

							// GUI::Controls::Checkbox( XOR( "Fake model#Local" ), XOR( "esp_chams_desync_enable" ) );
							// GUI::Controls::ColorPicker( XOR( "Fake model color#Local" ), XOR( "esp_chams_desync_color" ) );

							// if( g_cfg[ XOR( "esp_chams_desync_enable" ) ].get<bool>( ) ) {
							//	GUI::Controls::Dropdown( XOR( "Fake model material#Local" ), Materials, XOR( "esp_chams_desync_material" ) );
							// }
							break;
						case 1:

							GUI::Controls::Checkbox( XOR( "Glow#Enemy" ), XOR( "esp_enemy_glow" ) );
							GUI::Controls::ColorPicker( XOR( "Glow color#Enemy" ), XOR( "esp_enemy_glow_color" ) );

							GUI::Controls::Checkbox( XOR( "Colored model visible#Enemy" ), XOR( "esp_chams_enemies" ) );
							GUI::Controls::ColorPicker( XOR( "Colored model visible color#Enemy" ), XOR( "esp_chams_enemies_color" ) );

							if( g_cfg[ XOR( "esp_chams_enemies" ) ].get<bool>( ) ) {
								GUI::Controls::Dropdown( XOR( "Colored model visible material#Enemy" ), Materials, XOR( "esp_chams_enemies_material" ) );

								GUI::Controls::Checkbox( XOR( "Colored model invisible#Enemy" ), XOR( "esp_chams_enemies_invis" ) );
								GUI::Controls::ColorPicker( XOR( "Colored model invisible color#Enemy" ), XOR( "esp_chams_enemies_invis_color" ) );

								if( g_cfg[ XOR( "esp_chams_enemies_invis" ) ].get<bool>( ) ) {
									GUI::Controls::Dropdown( XOR( "Colored model invisible material#Enemy" ), Materials, XOR( "esp_chams_enemies_invis_material" ) );
								}
							}

							GUI::Controls::Checkbox( XOR( "History chams #Enemy" ), XOR( "esp_chams_enemy_backtrack" ) );
							GUI::Controls::ColorPicker( XOR( "History chams color#Enemy" ), XOR( "esp_chams_enemy_backtrack_color" ) );

							//GUI::Controls::Checkbox( XOR( "Resolve chams #Enemy" ), XOR( "esp_chams_enemy_resolve" ) );
							//GUI::Controls::ColorPicker( XOR( "Resolve chams color#Enemy" ), XOR( "esp_chams_enemy_resolve_color" ) );

							break;
						case 2:

							GUI::Controls::Checkbox( XOR( "Glow#Team" ), XOR( "esp_team_glow" ) );
							GUI::Controls::ColorPicker( XOR( "Glow color#Team" ), XOR( "esp_team_glow_color" ) );

							GUI::Controls::Checkbox( XOR( "Colored model visible#Team" ), XOR( "esp_chams_team" ) );
							GUI::Controls::ColorPicker( XOR( "Colored model visible color#Team" ), XOR( "esp_chams_team_color" ) );

							if( g_cfg[ XOR( "esp_chams_team" ) ].get<bool>( ) ) {
								GUI::Controls::Dropdown( XOR( "Colored model visible material#Team" ), Materials, XOR( "esp_chams_team_material" ) );

								GUI::Controls::Checkbox( XOR( "Colored model invisible#Team" ), XOR( "esp_chams_team_invis" ) );
								GUI::Controls::ColorPicker( XOR( "Colored model invisible color#Team" ), XOR( "esp_chams_team_invis_color" ) );

								if( g_cfg[ XOR( "esp_chams_team_invis" ) ].get<bool>( ) ) {
									GUI::Controls::Dropdown( XOR( "Colored model invisible material#Team" ), Materials, XOR( "esp_chams_team_invis_material" ) );
								}
							}

							break;
						}


					}GUI::Group::EndGroup( );
				}
			}
			if( GUI::Form::BeginTab( XOR( "C" ), XOR( "Configs" ) ) ) {
				if( GUI::Form::BeginSubTab( XOR( "X" ), XOR( "General" ) ) ) {
					GUI::Group::BeginGroup( XOR( "General" ), vec2_t( 50, 100 ) ); {
						GUI::Controls::Checkbox( XOR( "Bunny hop" ), XOR( "misc_bhop" ) );
						GUI::Controls::Checkbox( XOR( "Duck while jumping" ), XOR( "misc_air_duck" ) );

						GUI::Controls::Dropdown( XOR( "Slow motion type" ), { XOR( "Prefer accuracy" ), XOR( "Prefer anti-aim" ) }, XOR( "movement_fakewalk_type" ) );
						GUI::Controls::Hotkey( XOR( "Slow motion" ), XOR( "movement_fakewalk_key" ) );

						GUI::Controls::Label( XOR( "Fake duck" ) );
						GUI::Controls::Hotkey( XOR( "Fake duck key" ), XOR( "rage_aa_fakeduck_key" ) );

						GUI::Controls::Checkbox( XOR( "Auto-strafer" ), XOR( "misc_auto_strafe" ) );
						GUI::Controls::Checkbox( XOR( "Movement key strafer" ), XOR( "misc_wasd_strafe" ) );

						if( g_cfg[ XOR( "cheat_mode" ) ].get<int>( ) == 1 ) {
							GUI::Controls::Label( XOR( "Circle strafer" ) );
							GUI::Controls::Hotkey( XOR( "Circle strafer key" ), XOR( "misc_circle_strafe" ) );

							GUI::Controls::Label( XOR( "Align strafer" ) );
							GUI::Controls::Hotkey( XOR( "Align strafer key" ), XOR( "misc_a_strafe" ) );

							GUI::Controls::Label( XOR( "Z strafer" ) );
							GUI::Controls::Hotkey( XOR( "Z Strafer key" ), XOR( "misc_z_strafe" ) );
							GUI::Controls::Slider( XOR( "Z Strafer frequency" ), XOR( "misc_z_strafe_freq" ), 1.f, 100.f, XOR( "%.0fhz" ) );
							GUI::Controls::Slider( XOR( "Z Strafer distance" ), XOR( "misc_z_strafe_dist" ), 1.f, 100.f, XOR( "%.0f%%" ) );
						}

						std::vector<MultiItem_t> indicators = {
							{ XOR( "Anti-aim" ), XOR( "misc_indicators_desync" ) },
							{ XOR( "Fakelag" ), XOR( "misc_indicators_lagcomp" ) },
							{ XOR( "Body-aim key" ), XOR( "misc_indicators_baim" ) },
							{ XOR( "Damage override key" ), XOR( "misc_indicators_dmg_override" ) },
						};

						GUI::Controls::Checkbox( XOR( "Override field-of-view" ), XOR( "misc_override_fov" ) );
						if( g_cfg[ XOR( "misc_override_fov" ) ].get<bool>( ) ) {
							GUI::Controls::Slider( XOR( "Field-of-view" ), XOR( "misc_override_fov_amount" ), 90.f, 130.f, XOR( "%.0f" ) );
							GUI::Controls::Checkbox( XOR( "Override while scoped" ), XOR( "misc_override_fov_scoped" ) );
						}

						GUI::Controls::Checkbox( XOR( "Override viewmodel field-of-view" ), XOR( "misc_override_viewmodel_fov" ) );
						if( g_cfg[ XOR( "misc_override_viewmodel_fov" ) ].get<bool>( ) ) {
							GUI::Controls::Slider( XOR( "Viewmodel field-of-view" ), XOR( "misc_override_viewmodel_fov_amount" ), 60.f, 100.f, XOR( "%.0f" ) );
						}

						std::vector<MultiItem_t> notifications = {
							{ XOR( "Damage given" ), XOR( "misc_log_damage" ) },
							{ XOR( "Damage taken" ), XOR( "misc_logs_harmed" ) },
							{ XOR( "Purchases" ), XOR( "misc_log_purchases" ) },
							{ XOR( "Bomb" ), XOR( "misc_log_bomb" ) },
							{ XOR( "Defuse" ), XOR( "misc_log_defuse" ) },
						};

						GUI::Controls::MultiDropdown( XOR( "Notifications" ), notifications );
						GUI::Controls::MultiDropdown( XOR( "Indicators" ), indicators );

						GUI::Controls::Checkbox( XOR( "Clantag spammer" ), XOR( "misc_clantag" ) );
						GUI::Controls::Dropdown( XOR( "Hitmarker" ), { XOR("Off"), XOR( "Screen" ), XOR( "World" ) }, XOR( "misc_hitmarker" ) );
						GUI::Controls::Dropdown( XOR( "Hitsound" ), { XOR( "Off" ), XOR( "Arena switch press" ), XOR( "Custom" ) }, XOR( "misc_hitsound" ) );

						//// this shit is so p seriously
						if( g_cfg[ XOR( "misc_hitsound" ) ].get<int>( ) ) {
							if( g_cfg[ XOR( "misc_hitsound" ) ].get<int>( ) == 2 ) {
								const auto GetScripts = [ & ] ( ) {
									std::string dir = XOR( "C:\\" );
									for( auto& file_path : std::filesystem::directory_iterator( dir ) ) {
										if( !file_path.path( ).string( ).empty( ) ) {
											if( file_path.path( ).string( ).find( ".wav" ) != std::string::npos ) {
												g_cl.m_hitsounds.emplace_back( file_path.path( ).string( ).erase( 0, dir.length( ) ) );
											}
										}
									}
								};

								if( g_cl.m_hitsounds.empty( ) ) {
									GetScripts( );
								}

								if( g_cl.m_hitsounds.empty( ) ) {
									GUI::Controls::Dropdown( XOR( "Custom hitsounds" ), { XOR( "None: C:\\*.wav" ) }, XOR( "misc_custom_hitsound" ) );
								}
								else {
									GUI::Controls::Dropdown( XOR( "Custom hitsounds" ), g_cl.m_hitsounds, XOR( "misc_custom_hitsound" ) );
								}
							}

							GUI::Controls::Slider( XOR( "Hitsound volume" ), XOR( "misc_hitsound_volume" ), 1.f, 100.f, XOR( "%.0f%%" ) );
						}

						GUI::Controls::Checkbox( XOR( "Spectators" ), XOR( "misc_spectators" ) );
						GUI::Controls::Checkbox( XOR( "Preserve killfeed" ), XOR( "misc_preserve_killfeed" ) );
						//	GUI::Controls::Checkbox( XOR( "Reveal matchmaking ranks" ), XOR( "misc_reveal_ranks" ) );

						GUI::Controls::Checkbox( XOR( "Ragdoll force" ), XOR( "misc_ragdoll_force" ) );
						GUI::Controls::Checkbox( XOR( "Unlock inventory in-game" ), XOR( "misc_unlock_inventory" ) );

						GUI::Controls::Label( XOR( "Last tick defuse" ) );
						GUI::Controls::Hotkey( XOR( "Last tick defuse key" ), XOR( "misc_last_tick_defuse_key" ), false );

						//GUI::Controls::Checkbox( XOR( "Viewmodel changer" ), XOR( "misc_viewmodel_enable" ) );
						//	if (g_cfg[XOR("misc_viewmodel_enable")].get<bool>()) {
						//		GUI::Controls::Slider(XOR("Viewmodel x"), XOR("misc_viewmodel_x"), -10.f, 10.f);
						//		GUI::Controls::Slider(XOR("Viewmodel y"), XOR("misc_viewmodel_y"), -10.f, 10.f);
						//		GUI::Controls::Slider(XOR("Viewmodel z"), XOR("misc_viewmodel_z"), -10.f, 10.f);
						//	}

						GUI::Controls::Button( XOR( "Unlock hidden convars" ), [ & ] ( ) -> void {
							g_cl.UnlockHiddenConvars( );
						} );
					}
					GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Other" ), vec2_t( 50, 50 ) ); {
						GUI::Controls::Dropdown( XOR( "Safety mode" ), { XOR( "Matchmaking" ), XOR( "No-spread" ) }, XOR( "cheat_mode" ) );
						//GUI::Controls::Checkbox( XOR( "Bypass sv_pure" ), XOR( "misc_bypass_sv_pure" ) );
					}
					GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Misc" ), vec2_t( 50, 50 ) ); {

						//GUI::Controls::Slider(XOR("MOVE_YAW POSE"), XOR("move_yaw_pose"), -10.f, 10.f);
						//GUI::Controls::Slider(XOR("MOVE_BLENDS CROUCH"), XOR("move_blends_pose_crouch"), -1.f, 1.f);
						//GUI::Controls::Slider(XOR("MOVE_BLENDS RUN"), XOR("move_blends_pose_run"), -1.f, 1.f);
						//GUI::Controls::Slider(XOR("MOVE_BLENDS WALK"), XOR("move_blends_pose_walk"), -1.f, 1.f);

						GUI::Controls::Dropdown( XOR( "Config slot" ), { XOR( "Slot 1" ), XOR( "Slot 2" ), XOR( "Slot 3" ), XOR( "Slot 4" ), XOR( "Slot 5" ), XOR( "Slot 6" ) }, XOR( "misc_cfg_slot" ) );

						static bool confirm_save = false;
						GUI::Controls::Button( !confirm_save ? XOR( "Save config#save" ) : XOR( "Are you sure?#save" ), [ & ] ( ) {
							if( !confirm_save ) {
								confirm_save = true;
								return;
							}

							if( confirm_save ) {
								g_config.save( std::to_string( g_cfg[ XOR( "misc_cfg_slot" ) ].get<int>( ) + 1 ) );
								confirm_save = false;

								GUI::ctx->SliderInfo.LastChangeTime.clear( );
								GUI::ctx->SliderInfo.PreviewAnimation.clear( );
								GUI::ctx->SliderInfo.PreviousAmount.clear( );
								GUI::ctx->SliderInfo.ShouldChangeValue.clear( );
								GUI::ctx->SliderInfo.ValueTimer.clear( );
								GUI::ctx->SliderInfo.ValueAnimation.clear( );
							}
						}, true );

						GUI::Controls::Button( XOR( "Load config" ), [ & ] ( ) {
							g_config.load( std::to_string( g_cfg[ XOR( "misc_cfg_slot" ) ].get<int>( ) + 1 ) );
							g_cfg[ XOR( "menu_color" ) ].set_color( Color( 132, 117, 230 ) ); // no no my fren u can not change menu clr 

							if( g_cl.m_processing ) {
								//if( g_cfg[ XOR( "skins_enable" ) ].get<bool>( ) ) {
								g_skins.m_update = true;
								//}
							}
						} );
					}
					GUI::Group::EndGroup( );
				}

				if( GUI::Form::BeginSubTab( XOR( "X" ), XOR( "Skins" ) ) ) {
					GUI::Group::BeginGroup( XOR( "General options" ), vec2_t( 50, 100 ) ); {

						GUI::Controls::Dropdown( XOR( "Knife model" ),
							{ XOR( "Off" ), XOR( "Bayonet" ), XOR( "Bowie" ), XOR( "Butterfly knife" ),
							XOR( "Falchion knife" ), XOR( "Flip knife" ), XOR( "Gut knife" ), XOR( "Huntsman knife" ),
							XOR( "Karambit" ), XOR( "M9 Bayonet" ), XOR( "Shadow daggers" ), XOR( "Classic knife" ),
							XOR( "Paracord knife" ), XOR( "Survival knife" ), XOR( "Ursus knife" ), XOR( "Navaja knife" ),
							XOR( "Nomad knife" ), XOR( "Stiletto knife" ), XOR( "Talon knife" ), XOR( "Skeleton knife" ) },
							XOR( "skins_knife" ) );

						GUI::Controls::Dropdown( XOR( "Glove model" ),
							{ XOR( "Off" ), XOR( "Bloodhound" ), XOR( "Sport" ), XOR( "Driver" ),
							XOR( "Handwraps" ), XOR( "Moto" ), XOR( "Specialist" ) },
							XOR( "skins_gloves" ) );

						if( !g_skins.m_glove_kits.empty( ) ) {
							GUI::Controls::Listbox( XOR( "Glove skin" ), g_skins.m_glove_kits, XOR( "skins_glove_kit" ), true, 7 );
						}
					}
					GUI::Group::EndGroup( );

					GUI::Group::BeginGroup( XOR( "Weapons skins" ), vec2_t( 50, 100 ) ); {
						GUI::Controls::Checkbox( XOR( "Enable#Skins" ), XOR( "skins_enable" ) );

						if( g_cl.m_weapon_id != -1 ) {
							std::string base_string = XOR( "skins_" );

							switch( g_cl.m_weapon_id ) {
							case DEAGLE:
								base_string.append( XOR( "DEAGLE" ) );
								break;
							case ELITE:
								base_string.append( XOR( "ELITE" ) );
								break;
							case FIVESEVEN:
								base_string.append( XOR( "FIVESEVEN" ) );
								break;
							case GLOCK:
								base_string.append( XOR( "GLOCK" ) );
								break;
							case AK47:
								base_string.append( XOR( "AK47" ) );
								break;
							case AUG:
								base_string.append( XOR( "AUG" ) );
								break;
							case AWP:
								base_string.append( XOR( "AWP" ) );
								break;
							case FAMAS:
								base_string.append( XOR( "FAMAS" ) );
								break;
							case G3SG1:
								base_string.append( XOR( "G3SG1" ) );
								break;
							case GALIL:
								base_string.append( XOR( "GALIL" ) );
								break;
							case M249:
								base_string.append( XOR( "M249" ) );
								break;
							case M4A4:
								base_string.append( XOR( "M4A4" ) );
								break;
							case MAC10:
								base_string.append( XOR( "MAC10" ) );
								break;
							case P90:
								base_string.append( XOR( "P90" ) );
								break;
							case MP5_SD:
								base_string.append( XOR( "MP5_SD" ) );
								break;
							case UMP45:
								base_string.append( XOR( "UMP45" ) );
								break;
							case XM1014:
								base_string.append( XOR( "XM1014" ) );
								break;
							case BIZON:
								base_string.append( XOR( "BIZON" ) );
								break;
							case MAG7:
								base_string.append( XOR( "MAG7" ) );
								break;
							case NEGEV:
								base_string.append( XOR( "NEGEV" ) );
								break;
							case SAWEDOFF:
								base_string.append( XOR( "SAWEDOFF" ) );
								break;
							case TEC9:
								base_string.append( XOR( "TEC9" ) );
								break;
							case P2000:
								base_string.append( XOR( "P2000" ) );
								break;
							case MP7:
								base_string.append( XOR( "MP7" ) );
								break;
							case MP9:
								base_string.append( XOR( "MP9" ) );
								break;
							case NOVA:
								base_string.append( XOR( "NOVA" ) );
								break;
							case P250:
								base_string.append( XOR( "P250" ) );
								break;
							case SCAR20:
								base_string.append( XOR( "SCAR20" ) );
								break;
							case SG553:
								base_string.append( XOR( "SG553" ) );
								break;
							case SSG08:
								base_string.append( XOR( "SSG08" ) );
								break;
							case M4A1S:
								base_string.append( XOR( "M4A1S" ) );
								break;
							case USPS:
								base_string.append( XOR( "USPS" ) );
								break;
							case CZ75A:
								base_string.append( XOR( "CZ75A" ) );
								break;
							case REVOLVER:
								base_string.append( XOR( "REVOLVER" ) );
								break;
							case KNIFE_BAYONET:
								base_string.append( XOR( "KNIFE_BAYONET" ) );
								break;
							case KNIFE_FLIP:
								base_string.append( XOR( "KNIFE_FLIP" ) );
								break;
							case KNIFE_GUT:
								base_string.append( XOR( "KNIFE_GUT" ) );
								break;
							case KNIFE_KARAMBIT:
								base_string.append( XOR( "KNIFE_KARAMBIT" ) );
								break;
							case KNIFE_M9_BAYONET:
								base_string.append( XOR( "KNIFE_M9_BAYONET" ) );
								break;
							case KNIFE_HUNTSMAN:
								base_string.append( XOR( "KNIFE_HUNTSMAN" ) );
								break;
							case KNIFE_FALCHION:
								base_string.append( XOR( "KNIFE_FALCHION" ) );
								break;
							case KNIFE_BOWIE:
								base_string.append( XOR( "KNIFE_BOWIE" ) );
								break;
							case KNIFE_BUTTERFLY:
								base_string.append( XOR( "KNIFE_BUTTERFLY" ) );
								break;
							case KNIFE_SHADOW_DAGGERS:
								base_string.append( XOR( "KNIFE_SHADOW_DAGGERS" ) );
								break;
							case KNIFE_CSS:
								base_string.append( XOR( "KNIFE_CSS" ) );
								break;
							case KNIFE_CORD:
								base_string.append( XOR( "KNIFE_CORD" ) );
								break;
							case KNIFE_CANIS:
								base_string.append( XOR( "KNIFE_CANIS" ) );
								break;
							case KNIFE_URSUS:
								base_string.append( XOR( "KNIFE_URSUS" ) );
								break;
							case KNIFE_GYPSY_JACKKNIFE:
								base_string.append( XOR( "KNIFE_GYPSY_JACKKNIFE" ) );
								break;
							case KNIFE_OUTDOOR:
								base_string.append( XOR( "KNIFE_OUTDOOR" ) );
								break;
							case KNIFE_STILETTO:
								base_string.append( XOR( "KNIFE_STILETTO" ) );
								break;
							case KNIFE_WIDOWMAKER:
								base_string.append( XOR( "KNIFE_WIDOWMAKER" ) );
								break;
							case KNIFE_SKELETON:
								base_string.append( XOR( "KNIFE_SKELETON" ) );
								break;

							default:
								break;
							}

							std::string paintkit_string = base_string.append( XOR( "_kit" ) );
							static int old_paintkit = g_cfg[ paintkit_string ].get<int>( );

							std::string stattrak_string = base_string.append( XOR( "_stattrak" ) );
							static float old_stattrak = g_cfg[ stattrak_string ].get<float>( );

							std::string seed_string = base_string.append( XOR( "_seed" ) );
							static float old_seed = g_cfg[ seed_string ].get<float>( );

							if( !g_skins.m_paint_kits.empty( ) ) {
								GUI::Controls::Listbox( XOR( "Paintkit" ), g_skins.m_paint_kits, paintkit_string, true, 7 );
							}

							GUI::Controls::Slider( XOR( "StatTrak" ), stattrak_string, 0.f, 1337.f, XOR( "%.0f" ) );

							GUI::Controls::Slider( XOR( "Seed" ), seed_string, 0.f, 255.f, XOR( "%.0f" ) );

							if( old_paintkit != g_cfg[ paintkit_string ].get<int>( ) ||
								old_stattrak != g_cfg[ stattrak_string ].get<float>( ) ||
								old_seed != g_cfg[ seed_string ].get<float>( ) ) {

								g_skins.m_update = true;

								old_paintkit = g_cfg[ paintkit_string ].get<int>( );
								old_stattrak = g_cfg[ stattrak_string ].get<float>( );
								old_seed = g_cfg[ seed_string ].get<float>( );
							}
						}

					}
					GUI::Group::EndGroup( );
				}
			}

			GUI::Form::EndWindow( );
		}

	}

	void Draw( ) {
		DrawMenu( );
	}
}