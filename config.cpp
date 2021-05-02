#include "includes.h"

Config g_config{ };

value pre_defined;

std::map< std::string, value > g_cfg{
	{ XOR( "rage_aimbot_enabled" ), pre_defined },
	{ XOR( "rage_aimbot_limit_fov" ), pre_defined },
	{ XOR( "rage_aimbot_limit_fov_amount" ), pre_defined },
	{ XOR( "rage_aimbot_target_selection" ), pre_defined },
	{ XOR( "rage_aimbot_silent_aim" ), pre_defined },
	{ XOR( "rage_aimbot_autofire" ), pre_defined },
	{ XOR( "rage_aimbot_hitbox_head" ), pre_defined },
	{ XOR( "rage_aimbot_hitbox_neck" ), pre_defined },
	{ XOR( "rage_aimbot_hitbox_chest" ), pre_defined },
	{ XOR( "rage_aimbot_hitbox_arms" ), pre_defined },
	{ XOR( "rage_aimbot_hitbox_stomach" ), pre_defined },
	{ XOR( "rage_aimbot_hitbox_legs" ), pre_defined },
	{ XOR( "rage_aimbot_hitbox_feet" ), pre_defined },
	{ XOR( "rage_aimbot_multipoint_head" ), pre_defined },
	{ XOR( "rage_aimbot_multipoint_chest" ), pre_defined },
	{ XOR( "rage_aimbot_multipoint_stomach" ), pre_defined },
	{ XOR( "rage_aimbot_multipoint_legs" ), pre_defined },
	{ XOR( "rage_aimbot_multipoint_feet" ), pre_defined },
	{ XOR( "rage_aimbot_multipoint_scale" ), pre_defined },
	{ XOR( "rage_aimbot_stomach_scale" ), pre_defined },
	{ XOR( "rage_aimbot_hitchance" ), pre_defined },
	{ XOR( "rage_aimbot_hitchance_amount" ), pre_defined },
	{ XOR("rage_safepoint_key"), pre_defined },
	{ XOR("rage_aimbot_prefer_safepoint"), pre_defined },
	{ XOR("rage_aimbot_prefered_hitbox"), pre_defined },
	{ XOR("rage_aimbot_ignore_limbs"), pre_defined },
	{ XOR( "rage_aimbot_minimal_damage_override_key" ), pre_defined },
	{ XOR( "rage_aimbot_minimal_damage_override" ), pre_defined },
	{ XOR( "rage_aimbot_minimal_damage_override_pen" ), pre_defined },
	{ XOR( "rage_aimbot_minimal_damage" ), pre_defined },
	{ XOR( "rage_aimbot_minimal_damage_hp" ), pre_defined },
	{ XOR( "rage_aimbot_penetration" ), pre_defined },
	{ XOR( "rage_aimbot_penetration_minimal_damage" ), pre_defined },
	{ XOR( "rage_aimbot_penetration_minimal_damage_hp" ), pre_defined },
	{ XOR( "misc_knifebot" ), pre_defined },
	{ XOR( "rage_aimbot_baim_key" ), pre_defined },
	{ XOR( "rage_aimbot_baim_prefer_always" ), pre_defined },
	{ XOR( "rage_aimbot_baim_prefer_lethal" ), pre_defined },
	{ XOR( "rage_aimbot_baim_prefer_lethalx2" ), pre_defined },
	{ XOR( "rage_aimbot_baim_prefer_air" ), pre_defined },
	{ XOR( "rage_aimbot_baim_always" ), pre_defined },
	{ XOR( "rage_aimbot_baim_always_health" ), pre_defined },
	{ XOR( "rage_aimbot_baim_always_air" ), pre_defined },
	{ XOR( "rage_aimbot_auto_scope" ), pre_defined },
	{ XOR( "rage_aimbot_auto_stop" ), pre_defined },
	{ XOR( "rage_aimbot_auto_stop_type_full" ), pre_defined },
	{ XOR( "rage_extended_backtrack" ), pre_defined },
	{ XOR( "rage_aimbot_norecoil" ), pre_defined },
	{ XOR( "rage_aimbot_nospread" ), pre_defined },
	{ XOR( "rage_predict_fakelag" ), pre_defined },
	{ XOR( "rage_exploit_type" ), pre_defined },
	{ XOR( "rage_exploit_key" ), pre_defined },
	{ XOR( "rage_exploit_key_type" ), pre_defined },
	{XOR("rage_exploit_charged"), pre_defined},
	{ XOR( "rage_aimbot_baim_always_health_amount" ), pre_defined },
	{ XOR( "rage_aa_enabled" ), pre_defined },
	{ XOR( "rage_aa_pitch" ), pre_defined },
	{ XOR( "rage_aa_direction_lock" ), pre_defined },
	{ XOR( "rage_aa_yaw_base" ), pre_defined },
	{ XOR( "rage_aa_yaw" ), pre_defined },
	{ XOR( "rage_aa_yaw_direction" ), pre_defined },
	{ XOR( "rage_aa_yaw_auto_time" ), pre_defined },
	{ XOR( "rage_aa_jitter_type" ), pre_defined },
	{ XOR( "rage_aa_jitter_range" ), pre_defined },
	{ XOR( "rage_aa_rotation_enabled" ), pre_defined },
	{ XOR( "rage_aa_rotation_range" ), pre_defined },
	{ XOR( "rage_aa_rotation_speed" ), pre_defined },
	{ XOR( "rage_aa_random_update" ), pre_defined },
	{ XOR( "rage_aa_yaw_fake" ), pre_defined },
	{ XOR( "rage_aa_custom_fake" ), pre_defined },
	{ XOR( "rage_aa_real_around_fake_standing" ), pre_defined },
	{ XOR( "rage_aa_real_around_fake_moving" ), pre_defined },
	{ XOR( "rage_aa_real_around_fake_air" ), pre_defined },
	{ XOR( "rage_aa_real_around_fake_slow_motion" ), pre_defined },
	{ XOR( "rage_aa_negate_key" ), pre_defined },
	{ XOR( "rage_aa_fakeduck_key" ), pre_defined },
	{ XOR( "rage_aa_fakeduck_key_type" ), pre_defined },
	{ XOR( "rage_fakelag_enabled" ), pre_defined },
	{ XOR( "rage_fakelag_standing_enabled" ), pre_defined },
	{ XOR( "rage_fakelag_move_enabled" ), pre_defined },
	{ XOR( "rage_fakelag_air_enabled" ), pre_defined },
	{ XOR( "rage_fakelag_crouch_enabled" ), pre_defined },
	{ XOR( "rage_fakelag_type" ), pre_defined },
	{ XOR( "rage_fakelag_limit" ), pre_defined },
	{ XOR( "rage_fakelag_on_land" ), pre_defined },
	{ XOR( "esp_enemies_flags_money" ), pre_defined },
	{ XOR( "esp_enemies_flags_armor" ), pre_defined },
	{ XOR( "esp_enemies_flags_scope" ), pre_defined },
	{ XOR( "esp_enemies_flags_bomb" ), pre_defined },
	{ XOR( "esp_team_flags_money" ), pre_defined },
	{ XOR( "esp_team_flags_armor" ), pre_defined },
	{ XOR( "esp_team_flags_scope" ), pre_defined },
	{ XOR( "esp_team_flags_bomb" ), pre_defined },
	{ XOR( "esp_local_glow" ), pre_defined },
	{ XOR( "esp_local_glow_color" ), pre_defined },
	{ XOR( "esp_chams_local_enable" ), pre_defined },
	{ XOR( "esp_chams_local_color" ), pre_defined },
	{ XOR( "esp_chams_local_material" ), pre_defined },
	{ XOR( "esp_chams_local_blend_if_scoped" ), pre_defined },
	{ XOR( "esp_chams_local_blend_amount" ), pre_defined },
	{ XOR( "esp_chams_desync_enable" ), pre_defined },
	{ XOR( "esp_chams_desync_color" ), pre_defined },
	{ XOR( "esp_chams_desync_material" ), pre_defined },
	{ XOR( "esp_chams_desync_type" ), pre_defined },
	{ XOR("esp_box_horizontal"), pre_defined },
	{ XOR("esp_box_vertical"), pre_defined },
	{ XOR("esp_box_outline"), pre_defined },
	{ XOR( "esp_enemies_dormant" ), pre_defined },
	{ XOR( "esp_enemies_offscreen" ), pre_defined },
	{ XOR( "esp_enemies_offscreen_color" ), pre_defined },
	{ XOR( "esp_enemies_box" ), pre_defined },
	{ XOR( "esp_enemies_box_color" ), pre_defined },
	{ XOR( "esp_enemies_name" ), pre_defined },
	{ XOR( "esp_enemies_name_color" ), pre_defined },
	{ XOR( "esp_enemies_health" ), pre_defined },
	{ XOR( "esp_enemies_health_override" ), pre_defined },
	{ XOR( "esp_enemies_health_color" ), pre_defined },
	{ XOR( "esp_enemies_weapon" ), pre_defined },
	{ XOR( "esp_enemies_weapon_color" ), pre_defined },
	{ XOR( "esp_enemies_weapon_type" ), pre_defined },
	{ XOR( "esp_enemies_ammo" ), pre_defined },
	{ XOR( "esp_enemies_ammo_color" ), pre_defined },
	{ XOR( "esp_enemy_glow" ), pre_defined },
	{ XOR( "esp_enemy_glow_color" ), pre_defined },
	{ XOR( "esp_enemies_skeleton" ), pre_defined },
	{ XOR( "esp_enemies_skeleton_color" ), pre_defined },
	{ XOR( "esp_chams_enemies" ), pre_defined },
	{ XOR( "esp_chams_enemies_color" ), pre_defined },
	{ XOR( "esp_chams_enemies_material" ), pre_defined },
	{ XOR( "esp_chams_enemies_invis" ), pre_defined },
	{ XOR( "esp_chams_enemies_invis_color" ), pre_defined },
	{ XOR( "esp_chams_enemies_invis_material" ), pre_defined },
	{ XOR( "esp_chams_enemy_backtrack" ), pre_defined },
	{ XOR( "esp_chams_enemy_backtrack_color" ), pre_defined },
	{ XOR( "esp_chams_enemy_resolve" ), pre_defined },
	{ XOR( "esp_chams_enemy_resolve_color" ), pre_defined },
	{ XOR( "esp_team_box" ), pre_defined },
	{ XOR( "esp_team_box_color" ), pre_defined },
	{ XOR( "esp_team_name" ), pre_defined },
	{ XOR( "esp_team_name_color" ), pre_defined },
	{ XOR( "esp_team_health" ), pre_defined },
	{ XOR( "esp_team_weapon" ), pre_defined },
	{ XOR( "esp_team_weapon_color" ), pre_defined },
	{ XOR( "esp_team_weapon_type" ), pre_defined },
	{ XOR( "esp_team_ammo" ), pre_defined },
	{ XOR( "esp_team_ammo_color" ), pre_defined },
	{ XOR( "esp_team_glow" ), pre_defined },
	{ XOR( "esp_team_glow_color" ), pre_defined },
	{ XOR( "esp_team_skeleton" ), pre_defined },
	{ XOR( "esp_team_skeleton_color" ), pre_defined },
	{ XOR( "esp_chams_team" ), pre_defined },
	{ XOR( "esp_chams_team_color" ), pre_defined },
	{ XOR( "esp_chams_team_material" ), pre_defined },
	{ XOR( "esp_chams_team_invis" ), pre_defined },
	{ XOR( "esp_chams_team_invis_color" ), pre_defined },
	{ XOR( "esp_chams_team_invis_material" ), pre_defined },
	{ XOR( "visuals_misc_dropped_bomb" ), pre_defined },
	{ XOR( "misc_force_crosshair" ), pre_defined },
	{ XOR( "esp_misc_bomb_2d" ), pre_defined },
	{ XOR( "esp_misc_bomb_3d" ), pre_defined },
	{ XOR( "visuals_misc_dropped_weapons" ), pre_defined },
	{ XOR( "visuals_misc_dropped_weapons_ammo" ), pre_defined },
	{ XOR( "visuals_misc_penetration_crosshair" ), pre_defined },
	{ XOR( "esp_enemies_radar" ), pre_defined },
	{ XOR( "visuals_misc_dropped_weapons_color" ), pre_defined },
	{ XOR( "visuals_misc_projectiles" ), pre_defined },
	{ XOR( "visuals_misc_projectiles_color" ), pre_defined },
	{ XOR( "visuals_misc_projectiles_range_color" ), pre_defined },
	{ XOR( "visuals_misc_remove_scope" ), pre_defined },
	{ XOR( "visuals_misc_remove_visual_recoil" ), pre_defined },
	{ XOR( "visuals_misc_remove_smoke" ), pre_defined },
	{ XOR( "visuals_misc_remove_flash" ), pre_defined },
	{ XOR( "visuals_misc_remove_fog" ), pre_defined },
	{ XOR( "visuals_misc_no_draw_team" ), pre_defined },
	{ XOR( "visuals_misc_grenade_prediction" ), pre_defined },
	{ XOR( "visuals_misc_grenade_prediction_color1" ), pre_defined },
	{ XOR( "movement_fakewalk_key" ), pre_defined },
	{ XOR( "movement_fakewalk_key_type" ), pre_defined },
	{ XOR( "movement_fakewalk_type" ), pre_defined },
	{ XOR( "misc_world_night" ), pre_defined },
	{ XOR( "misc_world_night_darkness" ), pre_defined },
	{ XOR( "misc_world_fullbright" ), pre_defined },
	{ XOR( "misc_thirdperson_key" ), pre_defined },
	{ XOR( "misc_thirdperson_key_type" ), pre_defined },
	{ XOR( "misc_thirdperson_distance" ), pre_defined },
	{ XOR( "visuals_misc_bullet_beam" ), pre_defined },
	{ XOR( "visuals_misc_bullet_beam_time" ), pre_defined },
	{ XOR( "visuals_misc_bullet_beam_color" ), pre_defined },
	{ XOR( "visuals_misc_bullet_beam_hurt_color" ), pre_defined },
	{ XOR( "visuals_misc_bullet_beam_type" ), pre_defined },
	{ XOR( "misc_bhop" ), pre_defined },
	{ XOR( "misc_ragdoll_force" ), pre_defined },
	{ XOR( "misc_air_duck" ), pre_defined },
	{ XOR( "misc_auto_strafe" ), pre_defined },
	{ XOR( "misc_circle_strafe" ), pre_defined },
	{ XOR( "misc_a_strafe" ), pre_defined },
	{ XOR( "misc_wasd_strafe" ), pre_defined },
	{ XOR( "misc_z_strafe" ), pre_defined },
	{ XOR( "misc_z_strafe_freq" ), pre_defined },
	{ XOR( "misc_z_strafe_dist" ), pre_defined },
	{ XOR( "cheat_mode" ), pre_defined },
	{ XOR( "menu_color" ), pre_defined },
	{ XOR( "misc_indicators_lagcomp" ), pre_defined },
	{ XOR( "misc_indicators_desync" ), pre_defined },
	{ XOR( "misc_indicators_baim" ), pre_defined },
	{ XOR( "misc_indicators_dmg_override" ), pre_defined },
	{ XOR( "misc_log_damage" ), pre_defined },
	{ XOR( "misc_log_purchases" ), pre_defined },
	{ XOR( "misc_log_bomb" ), pre_defined },
	{ XOR( "misc_log_defuse" ), pre_defined },
	{ XOR( "misc_hitmarker" ), pre_defined },
	{ XOR( "misc_logs_harmed" ), pre_defined },
	{ XOR( "misc_spectators" ), pre_defined },
	{ XOR( "misc_preserve_killfeed" ), pre_defined },
	{ XOR( "misc_reveal_ranks" ), pre_defined },
	{ XOR( "misc_ragdoll_force" ), pre_defined },
	{ XOR("misc_hitsound"), pre_defined },
	{ XOR("misc_custom_hitsound"), pre_defined },
	{ XOR("misc_hitsound_volume"), pre_defined },
	{ XOR( "misc_clantag" ), pre_defined },
	{ XOR( "misc_unlock_inventory" ), pre_defined },
	{ XOR( "misc_last_tick_defuse_key" ), pre_defined },
	{ XOR( "misc_override_fov" ), pre_defined },
	{ XOR( "misc_override_fov_scoped" ), pre_defined },
	{ XOR( "misc_override_fov_amount" ), pre_defined },
	{ XOR( "misc_override_viewmodel_fov" ), pre_defined },
	{ XOR( "misc_override_viewmodel_fov_amount" ), pre_defined },

	{ XOR( "misc_viewmodel_enable" ), pre_defined },
	{ XOR( "misc_viewmodel_x" ), pre_defined },
	{ XOR( "misc_viewmodel_y" ), pre_defined },
	{ XOR( "misc_viewmodel_z" ), pre_defined },


	{ XOR( "skins_DEAGLE_kit" ), pre_defined },
	{ XOR( "skins_DEAGLE_stattrak" ), pre_defined },
	{ XOR( "skins_DEAGLE_seed" ), pre_defined },
	{ XOR( "skins_ELITE_kit" ), pre_defined },
	{ XOR( "skins_ELITE_stattrak" ), pre_defined },
	{ XOR( "skins_ELITE_seed" ), pre_defined },
	{ XOR( "skins_FIVESEVEN_kit" ), pre_defined },
	{ XOR( "skins_FIVESEVEN_stattrak" ), pre_defined },
	{ XOR( "skins_FIVESEVEN_seed" ), pre_defined },
	{ XOR( "skins_GLOCK_kit" ), pre_defined },
	{ XOR( "skins_GLOCK_stattrak" ), pre_defined },
	{ XOR( "skins_GLOCK_seed" ), pre_defined },
	{ XOR( "skins_AK47_kit" ), pre_defined },
	{ XOR( "skins_AK47_stattrak" ), pre_defined },
	{ XOR( "skins_AK47_seed" ), pre_defined },
	{ XOR( "skins_AUG_kit" ), pre_defined },
	{ XOR( "skins_AUG_stattrak" ), pre_defined },
	{ XOR( "skins_AUG_seed" ), pre_defined },
	{ XOR( "skins_AWP_kit" ), pre_defined },
	{ XOR( "skins_AWP_stattrak" ), pre_defined },
	{ XOR( "skins_AWP_seed" ), pre_defined },
	{ XOR( "skins_FAMAS_kit" ), pre_defined },
	{ XOR( "skins_FAMAS_stattrak" ), pre_defined },
	{ XOR( "skins_FAMAS_seed" ), pre_defined },
	{ XOR( "skins_G3SG1_kit" ), pre_defined },
	{ XOR( "skins_G3SG1_stattrak" ), pre_defined },
	{ XOR( "skins_G3SG1_seed" ), pre_defined },
	{ XOR( "skins_GALIL_kit" ), pre_defined },
	{ XOR( "skins_GALIL_stattrak" ), pre_defined },
	{ XOR( "skins_GALIL_seed" ), pre_defined },
	{ XOR( "skins_M249_kit" ), pre_defined },
	{ XOR( "skins_M249_stattrak" ), pre_defined },
	{ XOR( "skins_M249_seed" ), pre_defined },
	{ XOR( "skins_M4A4_kit" ), pre_defined },
	{ XOR( "skins_M4A4_stattrak" ), pre_defined },
	{ XOR( "skins_M4A4_seed" ), pre_defined },
	{ XOR( "skins_MAC10_kit" ), pre_defined },
	{ XOR( "skins_MAC10_stattrak" ), pre_defined },
	{ XOR( "skins_MAC10_seed" ), pre_defined },
	{ XOR( "skins_P90_kit" ), pre_defined },
	{ XOR( "skins_P90_stattrak" ), pre_defined },
	{ XOR( "skins_P90_seed" ), pre_defined },
	{ XOR( "skins_MP5_SD_kit" ), pre_defined },
	{ XOR( "skins_MP5_SD_stattrak" ), pre_defined },
	{ XOR( "skins_MP5_SD_seed" ), pre_defined },
	{ XOR( "skins_UMP45_kit" ), pre_defined },
	{ XOR( "skins_UMP45_stattrak" ), pre_defined },
	{ XOR( "skins_UMP45_seed" ), pre_defined },
	{ XOR( "skins_XM1014_kit" ), pre_defined },
	{ XOR( "skins_XM1014_stattrak" ), pre_defined },
	{ XOR( "skins_XM1014_seed" ), pre_defined },
	{ XOR( "skins_BIZON_kit" ), pre_defined },
	{ XOR( "skins_BIZON_stattrak" ), pre_defined },
	{ XOR( "skins_BIZON_seed" ), pre_defined },
	{ XOR( "skins_MAG7_kit" ), pre_defined },
	{ XOR( "skins_MAG7_stattrak" ), pre_defined },
	{ XOR( "skins_MAG7_seed" ), pre_defined },
	{ XOR( "skins_NEGEV_kit" ), pre_defined },
	{ XOR( "skins_NEGEV_stattrak" ), pre_defined },
	{ XOR( "skins_NEGEV_seed" ), pre_defined },
	{ XOR( "skins_SAWEDOFF_kit" ), pre_defined },
	{ XOR( "skins_SAWEDOFF_stattrak" ), pre_defined },
	{ XOR( "skins_SAWEDOFF_seed" ), pre_defined },
	{ XOR( "skins_TEC9_kit" ), pre_defined },
	{ XOR( "skins_TEC9_stattrak" ), pre_defined },
	{ XOR( "skins_TEC9_seed" ), pre_defined },
	{ XOR( "skins_P2000_kit" ), pre_defined },
	{ XOR( "skins_P2000_stattrak" ), pre_defined },
	{ XOR( "skins_P2000_seed" ), pre_defined },
	{ XOR( "skins_MP7_kit" ), pre_defined },
	{ XOR( "skins_MP7_stattrak" ), pre_defined },
	{ XOR( "skins_MP7_seed" ), pre_defined },
	{ XOR( "skins_MP9_kit" ), pre_defined },
	{ XOR( "skins_MP9_stattrak" ), pre_defined },
	{ XOR( "skins_MP9_seed" ), pre_defined },
	{ XOR( "skins_NOVA_kit" ), pre_defined },
	{ XOR( "skins_NOVA_stattrak" ), pre_defined },
	{ XOR( "skins_NOVA_seed" ), pre_defined },
	{ XOR( "skins_P250_kit" ), pre_defined },
	{ XOR( "skins_P250_stattrak" ), pre_defined },
	{ XOR( "skins_P250_seed" ), pre_defined },
	{ XOR( "skins_SCAR20_kit" ), pre_defined },
	{ XOR( "skins_SCAR20_stattrak" ), pre_defined },
	{ XOR( "skins_SCAR20_seed" ), pre_defined },
	{ XOR( "skins_SG553_kit" ), pre_defined },
	{ XOR( "skins_SG553_stattrak" ), pre_defined },
	{ XOR( "skins_SG553_seed" ), pre_defined },
	{ XOR( "skins_SSG08_kit" ), pre_defined },
	{ XOR( "skins_SSG08_stattrak" ), pre_defined },
	{ XOR( "skins_SSG08_seed" ), pre_defined },
	{ XOR( "skins_M4A1S_kit" ), pre_defined },
	{ XOR( "skins_M4A1S_stattrak" ), pre_defined },
	{ XOR( "skins_M4A1S_seed" ), pre_defined },
	{ XOR( "skins_USPS_kit" ), pre_defined },
	{ XOR( "skins_USPS_stattrak" ), pre_defined },
	{ XOR( "skins_USPS_seed" ), pre_defined },
	{ XOR( "skins_CZ75A_kit" ), pre_defined },
	{ XOR( "skins_CZ75A_stattrak" ), pre_defined },
	{ XOR( "skins_CZ75A_seed" ), pre_defined },
	{ XOR( "skins_REVOLVER_kit" ), pre_defined },
	{ XOR( "skins_REVOLVER_stattrak" ), pre_defined },
	{ XOR( "skins_REVOLVER_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_BAYONET_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_BAYONET_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_BAYONET_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_FLIP_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_FLIP_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_FLIP_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_GUT_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_GUT_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_GUT_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_KARAMBIT_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_KARAMBIT_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_KARAMBIT_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_M9_BAYONET_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_M9_BAYONET_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_M9_BAYONET_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_HUNTSMAN_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_HUNTSMAN_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_HUNTSMAN_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_FALCHION_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_FALCHION_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_FALCHION_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_BOWIE_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_BOWIE_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_BOWIE_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_BUTTERFLY_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_BUTTERFLY_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_BUTTERFLY_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_SHADOW_DAGGERS_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_SHADOW_DAGGERS_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_SHADOW_DAGGERS_seed" ), pre_defined },

	{ XOR( "skins_KNIFE_CSS_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_CSS_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_CSS_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_CORD_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_CORD_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_CORD_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_CANIS_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_CANIS_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_CANIS_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_URSUS_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_URSUS_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_URSUS_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_GYPSY_JACKKNIFE_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_GYPSY_JACKKNIFE_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_GYPSY_JACKKNIFE_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_OUTDOOR_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_OUTDOOR_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_OUTDOOR_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_STILETTO_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_STILETTO_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_STILETTO_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_WIDOWMAKER_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_WIDOWMAKER_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_WIDOWMAKER_seed" ), pre_defined },
	{ XOR( "skins_KNIFE_SKELETON_kit" ), pre_defined },
	{ XOR( "skins_KNIFE_SKELETON_stattrak" ), pre_defined },
	{ XOR( "skins_KNIFE_SKELETON_seed" ), pre_defined },

};

bool Config::get_hotkey( std::string str )
{
	int vk = g_cfg[ str ].get<int>( );
	int activation_type = g_cfg[ str + XOR( "_type" ) ].get<int>( );

	switch( activation_type ) {
	case 0:
		if( m_hotkey_states.find( str ) == m_hotkey_states.end( ) ) {
			m_hotkey_states[ str ] = InputHelper::Down( vk );
		}

		else if( InputHelper::Pressed( vk ) ) {
			m_hotkey_states[ str ] = !m_hotkey_states[ str ];
		}
		return m_hotkey_states[ str ];
		break;
	case 1:
		return InputHelper::Down( vk );
		break;
	case 2:
		return !InputHelper::Down( vk );
		break;
	case 3:
		return true;
		break;
	}

	return false;
}

void Config::init( ) {
	// reset.
	m_init = false;

	// alloc.
	m_path.resize( MAX_PATH + 1 );

	// get the path to mydocuments.
	if( !SUCCEEDED( g_winapi.SHGetFolderPathA( 0, CSIDL_PERSONAL, 0, SHGFP_TYPE_CURRENT, ( char* )m_path.data( ) ) ) )
		return;

	// append our subdir.
	g_winapi.PathAppendA( ( char* )m_path.c_str( ), g_cl.m_user.c_str( ) );

	// create folder if not present.
	g_winapi.CreateDirectoryA( m_path.c_str( ), 0 );

	// we found the path.
	// and we managed to create our subdir.
	m_init = true;
}

void Config::save( std::string name, bool to_clipboard ) {
	if( !m_init ) {
		return;
	}

	std::string file;
	file = tfm::format( XOR( "%s\\%s" ), m_path.data( ), name.data( ) );

	for( auto& e : g_cfg ) {
		WritePrivateProfileStringA( XOR( "pandora" ), e.first.c_str( ), std::to_string( e.second.get<double>( ) ).c_str( ), file.c_str( ) );
	}

	g_notify.add( XOR( "successfully saved config" ) );
}

void Config::load( std::string name, bool from_clipboard ) {
	if( !m_init ) {
		return;
	}

	std::string file;
	file = tfm::format( XOR( "%s\\%s" ), m_path.data( ), name.data( ) );

	for( auto& e : g_cfg ) {
		char value[ 64 ] = { '\0' };

		GetPrivateProfileStringA( XOR( "pandora" ), e.first.c_str( ), "", value, 64, file.c_str( ) );

		e.second.set<double>( atof( value ) );
	}

	g_notify.add( XOR( "successfully loaded config" ) );
}