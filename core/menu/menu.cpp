#include "menu.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "../features/misc/logs.hpp"

c_menu menu;

#define UNLEN 256
IDirect3DStateBlock9 *state_block;
bool reverse = false;
int offset = 0;
bool show_popup = false;
bool save_config = false;
bool load_config = false;
namespace ImGui {
	long get_mils() {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	void begin_popup(const char* text, int on_screen_mils, bool* done) {
		if (!done)
			show_popup = true;

		ImGuiIO &io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		int width = io.DisplaySize.x;
		static long old_time = -1;
		ImGui::SetNextWindowPos(ImVec2(width - offset, 100));
		style.WindowMinSize = ImVec2(100.f, 20.f);
		ImGui::Begin("##PopUpWindow", &show_popup, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x - 15, p.y - 13), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 16), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));

		long current_time_ms = get_mils();

		ImVec2 text_size = ImGui::CalcTextSize(text);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - text_size.y / 2);
		ImGui::Text(text);

		if (!reverse) {
			if (offset < ImGui::GetWindowWidth())
				offset += (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);

			if (offset >= ImGui::GetWindowWidth() && old_time == -1) {
				old_time = current_time_ms;
			}
		}

		if (current_time_ms - old_time >= on_screen_mils && old_time != -1)
			reverse = true;

		if (reverse) {
			if (offset > 0)
				offset -= (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);
			if (offset <= 0) {
				offset = 0;
				reverse = false;
				*done = true;
				old_time = -1;
				show_popup = false;
			}
		}

		ImGui::End();
	}
}

void c_menu::run() {
	static int page = 0;

	if (opened) {
		ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(167 / 255.f, 24 / 255.f, 71 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(167 / 255.f, 24 / 255.f, 71 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(167 / 255.f, 24 / 71, 247 / 255.f, 1.f);

		ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("CheepHook", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImColor c = ImColor(32, 114, 247);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 30), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 3), ImColor(30, 30, 39));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 32), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + +30), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));
			ImGui::PushFont(font_menu);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7); 
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7); 
			ImGui::Text("CheepHook");
			ImGui::SameLine();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);
			if (ImGui::ButtonT("LEGIT", ImVec2(50, 30), page, 0, false, false)) page = 0; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("VISUALS", ImVec2(50, 30), page, 1, false, false)) page = 1; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("MISC", ImVec2(50, 30), page, 2, false, false)) page = 2; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("CONFIG", ImVec2(50, 30), page, 3, false, false)) page = 3; ImGui::SameLine(0, 0);
			ImGui::PopFont();

			ImGui::PushFont(font_menu);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 45);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 222); 

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();

			static int test = 0;
			switch (page) {
			case 0:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("AIMBOT", ImVec2(279, 268), true); {
					ImGui::Checkbox("Active", &config_system.item.aim_enabled);
					ImGui::PushStyleColor(ImGuiCol_Text, config_system.item.aim_enabled ? ImVec4(1.f, 1.f, 1.f, 1) : ImVec4(.6f, .6f, .6f, 1));
					ImGui::Checkbox("Distance Based FOV", &config_system.item.aim_distance_based_fov);
					ImGui::Checkbox("Silent Aim (Non-pSilent)", &config_system.item.aim_silent);
					ImGui::Checkbox("Scope Aim", &config_system.item.scope_aim);
					ImGui::Checkbox("Smoke Aim", &config_system.item.smoke_check);
					ImGui::Checkbox("Friendly Fire", &config_system.item.aim_team_check);
					ImGui::Checkbox("Jump Check", &config_system.item.aim_jump_check);
					ImGui::Checkbox("Auto Pistol", &config_system.item.aimbot_auto_pistol);
					ImGui::SliderInt("Aimlock Delay After Kill", &config_system.item.aimbot_delay_after_kill, 0, 350);
					ImGui::PopStyleColor();

				} ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("BACKTRACK", ImVec2(279, 267), true); {
					ImGui::Checkbox("Active", &config_system.item.backtrack);
					ImGui::PushStyleColor(ImGuiCol_Text, config_system.item.backtrack ? ImVec4(1.f, 1.f, 1.f, 1) : ImVec4(.6f, .6f, .6f, 1));
					ImGui::SliderFloat("Records (ms)", &config_system.item.backtrack_ms, 1.0f, 200.0f, "%.2f");
					ImGui::PopStyleColor();
				}
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("SETTINGS", ImVec2(279, 543), true); {
					if (ImGui::ButtonT("PISTOL", ImVec2(50, 30), test, 0, false, ImColor(0, 0, 0))) test = 0; ImGui::SameLine(0, 0);
					if (ImGui::ButtonT("RIFLE", ImVec2(50, 30), test, 1, false, false)) test = 1; ImGui::SameLine(0, 0); 
					if (ImGui::ButtonT("SNIPER", ImVec2(50, 30), test, 2, false, false)) test = 2; ImGui::SameLine(0, 0); 
					if (ImGui::ButtonT("SMG", ImVec2(50, 30), test, 3, false, false)) test = 3; ImGui::SameLine(0, 0); 
					if (ImGui::ButtonT("HEAVY", ImVec2(50, 30), test, 4, false, false)) test = 4; 

					ImGui::PushFont(font_menu);

					ImGui::PushStyleColor(ImGuiCol_Text, config_system.item.aim_enabled ? ImVec4(1.f, 1.f, 1.f, 1) : ImVec4(.6f, .6f, .6f, 1));
					switch (test) {
					case 0:
						ImGui::Checkbox("Nearest Hitbox", &config_system.item.aim_mode_pistol);
						if (config_system.item.aim_mode_pistol == 0) {
							ImGui::Combo("Hitbox", &config_system.item.aim_bone_pistol, "Head\0Neck\0Chest\0Stomach\0Pelvis");
						}
						ImGui::SliderFloat("FOV", &config_system.item.aim_fov_pistol, 0.0f, 5.0f, "%.2f");
						ImGui::SliderFloat("Smoothing", &config_system.item.aim_smooth_pistol, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("RCS-X", &config_system.item.rcs_x_pistol, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("RCS-Y", &config_system.item.rcs_y_pistol, 0.0f, 1.0f, "%.2f");
						break;
					case 1:
						ImGui::Checkbox("Nearest Hitbox", &config_system.item.aim_mode_rifle);
						if (config_system.item.aim_mode_rifle == 0) {
							ImGui::Combo("Hitbox", &config_system.item.aim_bone_rifle, "Head\0Neck\0Chest\0Stomach\0Pelvis");
						}
						ImGui::SliderFloat("FOV", &config_system.item.aim_fov_rifle, 0.0f, 5.0f, "%.2f");
						ImGui::SliderFloat("Smoothing", &config_system.item.aim_smooth_rifle, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("RCS-X", &config_system.item.rcs_x_rifle, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("RCS-Y", &config_system.item.rcs_y_rifle, 0.0f, 1.0f, "%.2f");
						break;
					case 2:
						ImGui::Checkbox("Nearest Hitbox", &config_system.item.aim_mode_sniper);
						if (config_system.item.aim_mode_sniper == 0) {
							ImGui::Combo("Hitbox", &config_system.item.aim_bone_sniper, "Head\0Neck\0Chest\0Stomach\0Pelvis");
						}
						ImGui::SliderFloat("FOV", &config_system.item.aim_fov_sniper, 0.0f, 5.0f, "%.2f");
						ImGui::SliderFloat("Smoothing", &config_system.item.aim_smooth_sniper, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("RCS-X", &config_system.item.rcs_x_sniper, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("RCS-Y", &config_system.item.rcs_y_sniper, 0.0f, 1.0f, "%.2f");
						break;
					case 3:
						ImGui::Checkbox("Nearest Hitbox", &config_system.item.aim_mode_smg);
						if (config_system.item.aim_mode_smg == 0) {
							ImGui::Combo("Hitbox", &config_system.item.aim_bone_smg, "Head\0Neck\0Chest\0Stomach\0Pelvis");
						}
						ImGui::SliderFloat("FOV", &config_system.item.aim_fov_smg, 0.0f, 5.0f, "%.2f");
						ImGui::SliderFloat("Smoothing", &config_system.item.aim_smooth_smg, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("RCS-X", &config_system.item.rcs_x_smg, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("RCS-Y", &config_system.item.rcs_y_smg, 0.0f, 1.0f, "%.2f");
						break;
					case 4:
						ImGui::Checkbox("Nearest Hitbox", &config_system.item.aim_mode_heavy);
						if (config_system.item.aim_mode_heavy == 0) {
							ImGui::Combo("Hitbox", &config_system.item.aim_bone_heavy, "Head\0Neck\0Chest\0Stomach\0Pelvis");
						}
						ImGui::SliderFloat("FOV", &config_system.item.aim_fov_heavy, 0.0f, 5.0f, "%.2f");
						ImGui::SliderFloat("Smoothing", &config_system.item.aim_smooth_heavy, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("RCS-X", &config_system.item.rcs_x_heavy, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("RCS-Y", &config_system.item.rcs_y_heavy, 0.0f, 1.0f, "%.2f");
						break;
					}
					ImGui::PopStyleColor();

					ImGui::PopFont();
				} ImGui::EndChild(true);
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				break;
			case 1:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

				ImGui::BeginChild("ENGINE ESP", ImVec2(279, 268), true); {
					ImGui::Checkbox("Active", &config_system.item.visuals_enabled);
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("EFFECTS", ImVec2(279, 267), true); {
					ImGui::Checkbox("Force Crosshair", &config_system.item.force_crosshair);

					ImGui::PushStyleColor(ImGuiCol_Text, config_system.item.visuals_enabled ? ImVec4(1.f, 1.f, 1.f, 1) : ImVec4(.6f, .6f, .6f, 1));
					ImGui::SliderInt("FOV Override", &config_system.item.fov, 0, 60);
					ImGui::PopStyleColor();

					ImGui::PushStyleColor(ImGuiCol_Text, config_system.item.visuals_enabled ? ImVec4(1.f, 1.f, 1.f, 1) : ImVec4(.6f, .6f, .6f, 1));

					if (ImGui::BeginCombo("World ESP", "...", ImVec2(0, 49))) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Planted Bomb"), &config_system.item.bomb_planted, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Danger Zone"), &config_system.item.danger_zone_dropped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}
					ImGui::PopStyleColor();
				}
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("GLOW", ImVec2(279, 268), true); {
					ImGui::Checkbox("Active", &config_system.item.visuals_glow);
					ImGui::Checkbox("Enemy", &config_system.item.visuals_glow_enemy);
					ImGui::ColorEdit4("Enemy Glow Color", config_system.item.clr_glow, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Teammate", &config_system.item.visuals_glow_team);
					ImGui::ColorEdit4("Teammate Glow Color", config_system.item.clr_glow_team, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Planted Bomb", &config_system.item.visuals_glow_planted);
					ImGui::ColorEdit4("Bomb Glow Color", config_system.item.clr_glow_planted, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Dropped Weapons", &config_system.item.visuals_glow_weapons);
					ImGui::ColorEdit4("Weapon Glow Color", config_system.item.clr_glow_dropped, ImGuiColorEditFlags_NoInputs);

				}
				ImGui::EndChild(true);

				ImGui::Spacing();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);

				ImGui::BeginChild("CHAMS", ImVec2(279, 267), true); {
					ImGui::PushStyleColor(ImGuiCol_Text, config_system.item.visuals_enabled ? ImVec4(1.f, 1.f, 1.f, 1) : ImVec4(.6f, .6f, .6f, 1));
					ImGui::Combo("Chams Type", &config_system.item.vis_chams_type, "textured\0flat\0metallic\0pulsating");
					ImGui::PopStyleColor();
					ImGui::Checkbox("Enemy", &config_system.item.vis_chams_vis);
					ImGui::ColorEdit4("Enemy Cham Color", config_system.item.clr_chams_vis, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Enemy (behind wall)", &config_system.item.vis_chams_invis);
					ImGui::ColorEdit4("Enemy (behind wall) Cham Color", config_system.item.clr_chams_invis, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("Teammate", &config_system.item.vis_chams_vis_teammate);
					ImGui::ColorEdit4("Teammate Cham Color", config_system.item.clr_chams_vis_teammate, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Teammate (behind wall)", &config_system.item.vis_chams_invis_teammate);
					ImGui::ColorEdit4("Teammate (behind wall) Cham Color", config_system.item.clr_chams_invis_teammate, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("Backtrack Chams", &config_system.item.backtrack_visualize);
					ImGui::Checkbox("Disable Behind Smoke", &config_system.item.vis_chams_smoke_check);
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			case 2:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

				ImGui::BeginChild("MISC", ImVec2(279, 543), true); {
					ImGui::Checkbox("Active", &config_system.item.misc_enabled);
					ImGui::Checkbox("Engine Radar", &config_system.item.radar);

					ImGui::PushStyleColor(ImGuiCol_Text, config_system.item.misc_enabled ? ImVec4(1.f, 1.f, 1.f, 1) : ImVec4(.6f, .6f, .6f, 1));
					if (ImGui::BeginCombo("Logs", "...", ImVec2(0, 65))) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("player hurt"), &config_system.item.logs_player_hurt, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("player bought"), &config_system.item.logs_player_bought, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("config system"), &config_system.item.logs_config_system, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}
					ImGui::PopStyleColor();

					ImGui::Checkbox("Hitmarker", &config_system.item.hitmarker);
					if (config_system.item.hitmarker) {
						ImGui::Combo("Hitmarker Sound", &config_system.item.hitmarker_sound, "none\0one\0two\0three");
					}
					ImGui::Checkbox("Anti Screenshot", &config_system.item.anti_screenshot);
					ImGui::Checkbox("Spectator List", &config_system.item.spectators_list);
					ImGui::Checkbox("Watermark", &config_system.item.watermark);
					ImGui::Checkbox("Draw Aimbot Fov", &config_system.item.draw_aim_fov);
					ImGui::Checkbox("Disable Post Processing", &config_system.item.disable_post_processing);
					ImGui::Checkbox("Rank Reveal", &config_system.item.rank_reveal);
				}
				ImGui::EndChild(true);
				ImGui::NextColumn();

				ImGui::BeginChild("MOVEMENT", ImVec2(279, 543), true); {
					ImGui::Checkbox("Bunnyhop", &config_system.item.bunny_hop);
					if (config_system.item.bunny_hop) {
						ImGui::SliderInt("Hitchance", &config_system.item.bunny_hop_hitchance, 0, 100);
						ImGui::SliderInt("Minimum hops", &config_system.item.bunny_hop_minimum_value, 0, 20);
						ImGui::SliderInt("Maximum hops", &config_system.item.bunny_hop_maximum_value, 0, 20);
					}

					ImGui::Checkbox("Edge Jump", &config_system.item.edge_jump);
					if (config_system.item.edge_jump) {
						ImGui::Checkbox("Duck in Air", &config_system.item.edge_jump_duck_in_air);
					}
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			case 3:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("CONFIG", ImVec2(279, 268), true); {
					constexpr auto& config_items = config_system.get_configs();
					static int current_config = -1;

					if (static_cast<size_t>(current_config) >= config_items.size())
						current_config = -1;

					static char buffer[16];

					ImGui::PushItemWidth(213);
					if (ImGui::ListBox("", &current_config, [](void* data, int idx, const char** out_text) {
						auto& vector = *static_cast<std::vector<std::string>*>(data);
						*out_text = vector[idx].c_str();
						return true;
						}, & config_items, config_items.size(), 5) && current_config != -1)
						strcpy(buffer, config_items[current_config].c_str());

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						ImGui::PushID(0);
						ImGui::PushItemWidth(211);
						if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
							if (current_config != -1)
								config_system.rename(current_config, buffer);
						}
						ImGui::PopID();
						ImGui::NextColumn();

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (ImGui::Button(("CREATE"), ImVec2(102, 20))) {
							config_system.add(buffer);
						}

						ImGui::SameLine(0,7);

						if (ImGui::Button(("RESET"), ImVec2(102, 20))) {
							config_system.reset();
						}

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (current_config != -1) {
							if (ImGui::Button(("LOAD"), ImVec2(102, 20))) {
								config_system.load(current_config);

								load_config = true;

								if (config_system.item.logs_config_system) {
									utilities::console_warning("[config system] ");
									interfaces::console->console_printf(config_items[current_config].c_str());
									interfaces::console->console_printf(" loaded. \n");
									std::stringstream ss;
									ss << config_items[current_config].c_str() << " loaded.";
									event_logs.add(ss.str(), color(167, 255, 255, 255));
								}

							}

							ImGui::SameLine();

							if (ImGui::Button(("SAVE"), ImVec2(102, 20))) {
								config_system.save(current_config);

								save_config = true;

								if (config_system.item.logs_config_system) {
									utilities::console_warning("[config system] ");
									interfaces::console->console_printf(config_items[current_config].c_str());
									interfaces::console->console_printf(" saved. \n");
									std::stringstream ss;
									ss << config_items[current_config].c_str() << " saved.";
									event_logs.add(ss.str(), color(167, 255, 255, 255));
								}

							}

							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
							if (ImGui::Button(("REMOVE"), ImVec2(102, 20))) {
								config_system.remove(current_config);
							}
						}
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("KEY BINDS", ImVec2(279, 267), true); {
					ImGui::PushItemWidth(211);
					ImGui::Combo("Keybinds", &config_system.item.keybinds_selection, "Edge Jump Key\0Aimbot Key");

					if (config_system.item.keybinds_selection == 0) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						ImGui::InputInt("##AimbotKey", &config_system.item.aim_key);
					}

					else if (config_system.item.keybinds_selection == 1) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						ImGui::InputInt("##AimbotKey", &config_system.item.aim_key);
					}
				}
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("INFO", ImVec2(279, 543), true); {
					char buffer[UNLEN + 1];
					DWORD size;
					size = sizeof(buffer);
					GetUserName(buffer, &size);
					char title[UNLEN];
					char ch1[25] = "welcome, ";
					char *ch = strcat(ch1, buffer);

					ImGui::Text(ch);
					ImGui::Text("build: " __DATE__ " / " __TIME__);
					if (std::strstr(GetCommandLineA(), "-insecure")) {
						ImGui::Text("insecure mode!");
					}
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			}
		}

		ImGui::PopFont();

		ImGui::End();
	}
}

void c_menu::run_popup() {
	ImGui::PushFont(font_menu);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

	if (save_config) {
		bool done = false;
		ImGui::begin_popup("config saved.", 2000, &done);
		if (done)
			save_config = false;
	}

	if (load_config) {
		bool done = false;
		ImGui::begin_popup("config loaded.", 2000, &done);
		if (done)
			load_config = false;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}