#pragma once

#include <filesystem>

class c_config final {
public:
	void run(const char*) noexcept;
	void load(size_t) noexcept;
	void save(size_t) const noexcept;
	void add(const char*) noexcept;
	void remove(size_t) noexcept;
	void rename(size_t, const char*) noexcept;
	void reset() noexcept;

	constexpr auto& get_configs() noexcept {
		return configs;
	}

	struct {
		bool backtrack{ false };
		bool backtrack_visualize{ false };
		bool aim_enabled{ false };
		bool scope_aim{ false };
		bool smoke_check{ false };
		bool aim_silent{ false };
		bool aim_distance_based_fov{ false };
		float rcs_x_pistol{ 0.0f };
		float rcs_y_pistol{ 0.0f };
		float rcs_x_rifle{ 0.0f };
		float rcs_y_rifle{ 0.0f };
		float rcs_x_sniper{ 0.0f };
		float rcs_y_sniper{ 0.0f };
		float rcs_x_heavy{ 0.0f };
		float rcs_y_heavy{ 0.0f };
		float rcs_x_smg{ 0.0f };
		float rcs_y_smg{ 0.0f };
		float aim_fov_pistol{ 0.0f };
		float aim_fov_rifle{ 0.0f };
		float aim_fov_sniper{ 0.0f };
		float aim_fov_heavy{ 0.0f };
		float aim_fov_smg{ 0.0f };
		float aim_smooth_pistol{ 5.f };
		float aim_smooth_rifle{ 5.f };
		float aim_smooth_sniper{ 5.f };
		float aim_smooth_heavy{ 5.f };
		float aim_smooth_smg{ 5.f };
		int aim_bone_pistol{ 0 };
		int aim_bone_rifle{ 0 };
		int aim_bone_sniper{ 0 };
		int aim_bone_heavy{ 0 };
		int aim_bone_smg{ 0 };
		bool aim_mode_pistol{ false };
		bool aim_mode_rifle{ false };
		bool aim_mode_sniper{ false };
		bool aim_mode_heavy{ false };
		bool aim_mode_smg{ false };
		int aim_weapon{ 0 };;
		bool aim_team_check{ false };
		bool bomb_planted{ false };
		bool danger_zone_dropped{ false };
		bool visuals_enabled{ false };
		bool anti_screenshot{ false };
		bool visuals_glow{ false };
		bool visuals_glow_enemy{ false };
		bool visuals_glow_team{ false };
		bool visuals_glow_planted{ false };
		bool visuals_glow_weapons{ false };
		float clr_chams_vis[4]{ 1.0f,1.0f,1.0f,1.0f };
		float clr_chams_invis[4]{ 1.0f,1.0f,1.0f,1.0f };
		float clr_glow[4]{ 1.0f,1.0f,1.0f,1.0f };
		float clr_glow_team[4]{ 1.0f,1.0f,1.0f,1.0f };
		float clr_glow_planted[4]{ 1.0f,1.0f,1.0f,1.0f };
		float clr_glow_dropped[4]{ 1.0f,1.0f,1.0f,1.0f };
		bool chams{ false };
		int vis_chams_type{ 0 };
		int knife_wear{ 0 };
		int fov{ 0 };
		float flash_alpha{ 255.f };
		bool force_crosshair{ false };
		bool vis_chams_vis{ false };
		bool vis_chams_invis{ false };
		bool spectators_list{ false };
		bool watermark{ false };
		bool draw_aim_fov{ false };
		bool misc_enabled{ false };
		bool misc_save_config{ false };
		bool misc_load_config{ false };
		bool bunny_hop{ false };
		bool bunny_hop_humanize{ false };
		bool bunny_hop_minimum{ false };
		bool bunny_hop_maximum{ false };
		int bunny_hop_minimum_value{ 0 };
		int bunny_hop_maximum_value{ 0 };
		int bunny_hop_hitchance{ 0 };
		bool radar{ false };
		bool hitmarker{ false };
		bool logs_player_hurt{ false };
		bool logs_player_bought{ false };
		bool logs_config_system{ true };
		int hitmarker_sound{ 0 };
		int knife_model{ 0 };
		int knife_skin{ 0 };
		int config_selection{ 0 };
		int keybinds_selection{ 0 };
		bool edge_jump{ false };
		bool edge_jump_duck_in_air{ false };
		int edge_jump_key{ 0 };
		int changer_skin{ 0 };
		int paint_kit_vector_index_knife{ 0 };
		int paint_kit_index_knife{ 0 };
		int paint_kit_vector_index_usp{ 0 };
		int paint_kit_index_usp{ 0 };
		int paint_kit_vector_index_p2000{ 0 };
		int paint_kit_index_p2000{ 0 };
		int paint_kit_vector_index_glock{ 0 };
		int paint_kit_index_glock{ 0 };
		int paint_kit_vector_index_p250{ 0 };
		int paint_kit_index_p250{ 0 };
		int paint_kit_vector_index_fiveseven{ 0 };
		int paint_kit_index_fiveseven{ 0 };
		int paint_kit_vector_index_tec{ 0 };
		int paint_kit_index_tec{ 0 };
		int paint_kit_vector_index_cz{ 0 };
		int paint_kit_index_cz{ 0 };
		int paint_kit_vector_index_duals{ 0 };
		int paint_kit_index_duals{ 0 };
		int paint_kit_vector_index_deagle{ 0 };
		int paint_kit_index_deagle{ 0 };
		int paint_kit_vector_index_revolver{ 0 };
		int paint_kit_index_revolver{ 0 };
		int paint_kit_vector_index_famas{ 0 };
		int paint_kit_index_famas{ 0 };
		int paint_kit_vector_index_galil{ 0 };
		int paint_kit_index_galil{ 0 };
		int paint_kit_vector_index_m4a4{ 0 };
		int paint_kit_index_m4a4{ 0 };
		int paint_kit_vector_index_m4a1{ 0 };
		int paint_kit_index_m4a1{ 0 };
		int paint_kit_vector_index_ak47{ 0 };
		int paint_kit_index_ak47{ 0 };
		int paint_kit_vector_index_sg553{ 0 };
		int paint_kit_index_sg553{ 0 };
		int paint_kit_vector_index_aug{ 0 };
		int paint_kit_index_aug{ 0 };
		int paint_kit_vector_index_ssg08{ 0 };
		int paint_kit_index_ssg08{ 0 };
		int paint_kit_vector_index_awp{ 0 };
		int paint_kit_index_awp{ 0 };
		int paint_kit_vector_index_scar{ 0 };
		int paint_kit_index_scar{ 0 };
		int paint_kit_vector_index_g3sg1{ 0 };
		int paint_kit_index_g3sg1{ 0 };
		int paint_kit_vector_index_mac10{ 0 };
		int paint_kit_index_mac10{ 0 };
		int paint_kit_vector_index_mp5sd{ 0 };
		int paint_kit_index_mp5sd{ 0 };
		int paint_kit_vector_index_mp7{ 0 };
		int paint_kit_index_mp7{ 0 };
		int paint_kit_vector_index_mp9{ 0 };
		int paint_kit_index_mp9{ 0 };
		int paint_kit_vector_index_ppbizon{ 0 };
		int paint_kit_index_ppbizon{ 0 };
		int paint_kit_vector_index_p90{ 0 };
		int paint_kit_index_p90{ 0 };
		int paint_kit_vector_index_ump45{ 0 };
		int paint_kit_index_ump45{ 0 };
		int paint_kit_vector_index_mag7{ 0 };
		int paint_kit_index_mag7{ 0 };
		int paint_kit_vector_index_nova{ 0 };
		int paint_kit_index_nova{ 0 };
		int paint_kit_vector_index_sawedoff{ 0 };
		int paint_kit_index_sawedoff{ 0 };
		int paint_kit_vector_index_xm1014{ 0 };
		int paint_kit_index_xm1014{ 0 };
		int paint_kit_vector_index_m249{ 0 };
		int paint_kit_index_m249{ 0 };
		int paint_kit_vector_index_negev{ 0 };
		int paint_kit_index_negev{ 0 };

		float backtrack_ms{ 0.0f };
		int aim_key{ 0 };
		bool vis_chams_smoke_check{ false };
		bool disable_post_processing{ false };
		bool rank_reveal{ false };
		bool vis_chams_vis_teammate{ false };
		bool vis_chams_invis_teammate{ false };
		float clr_chams_vis_teammate[4]{ 1.0f,1.0f,1.0f,1.0f };
		float clr_chams_invis_teammate[4]{ 1.0f,1.0f,1.0f,1.0f };
		int aimbot_delay_after_kill{ 0 };
		bool aimbot_auto_pistol{ false };
		bool aimbot_auto_shoot{ false };
		bool aimbot_auto_crouch{ false };
		bool aimbot_auto_scope{ false };
		int autoshoot_delay_scoped{ 0 };
		int glove_model{ 0 };
		int glove_skin{ 0 };
		int paint_kit_vector_index_glove{ 0 };
		int paint_kit_index_glove{ 0 };
		int glove_wear{ 0 };
		float rcs_standalone_x{ 0.0f };
		float rcs_standalone_y{ 0.0f };
		bool aim_jump_check{ false };
	} item;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern c_config config_system;
