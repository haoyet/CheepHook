#include "misc.hpp"
#include "../aimbot/aimbot.hpp"
#include "..//..//..//dependencies/math/math.hpp"

c_misc misc;

void c_misc::rank_reveal() noexcept {
	if (!config_system.item.rank_reveal || !config_system.item.misc_enabled)
		return;

	if (GetAsyncKeyState(VK_TAB))
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);
}

void c_misc::spectators() noexcept {
	if (!config_system.item.spectators_list || !config_system.item.misc_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	int spectator_index = 0;
	int width, height;
	interfaces::engine->get_screen_size(width, height);

	render.draw_text(width - 80, height / 2 - 10, render.name_font, "spectators", true, color(255, 255, 255));
	for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

		if (!local_player)
			return;

		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			return;

		player_info_t info;

		if (entity && entity != local_player) {
			interfaces::engine->get_player_info(i, &info);
			if (!entity->is_alive() && !entity->dormant()) {
				auto target = entity->observer_target();

				if (!target)
					return;

				if (target) {
					auto spectator_target = interfaces::entity_list->get_client_entity_handle(target);
					if (spectator_target == local_player) {
						std::string player_name = info.name;
						std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);
						player_info_t spectator_info;
						interfaces::engine->get_player_info(i, &spectator_info);
						render.draw_text(width - 80, height / 2 + (10 * spectator_index), render.name_font, player_name.c_str(), true, color(255, 255, 255));
						spectator_index++;
					}
				}
			}
		}
	}
}

void c_misc::watermark() noexcept {
	if (!config_system.item.watermark || !config_system.item.misc_enabled)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	static int fps, old_tick_count;

	if ((interfaces::globals->tick_count - old_tick_count) > 50) {
		fps = static_cast<int>(1.f / interfaces::globals->frame_time);
		old_tick_count = interfaces::globals->tick_count;
	}
	std::stringstream ss;

	auto net_channel = interfaces::engine->get_net_channel_info();
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	std::string incoming = local_player ? std::to_string(static_cast<int>(net_channel->get_latency(FLOW_INCOMING) * 1000)) : "0";
	std::string outgoing = local_player ? std::to_string(static_cast<int>(net_channel->get_latency(FLOW_OUTGOING) * 1000)) : "0";

	ss << "CheepHook | fps: " << fps << " | incoming: " << incoming.c_str() << "ms" << " | outgoing: " << outgoing.c_str() << "ms";

	render.draw_filled_rect(width - 275, 4, 260, 20, color(33, 35, 47, 255));
	render.draw_outline(width - 275, 4, 260, 20, color(30, 30, 41, 255));
	render.draw_text(width - 270, 7, render.watermark_font, ss.str().c_str(), false, color(255, 255, 255, 255));
}

void c_misc::draw_aim_fov() {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!config_system.item.draw_aim_fov || !config_system.item.misc_enabled || !interfaces::engine->is_in_game() || !local_player->is_alive())
		return;

	auto weapon = local_player->active_weapon();

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	float half_fov;
	
	if ((width / height) == (16 / 9))
		half_fov = deg_to_rad(106.3)/2;

	else if ((width / height) == (16 / 10))
		half_fov = deg_to_rad(100.4)/2;

	else if ((width / height) == (4 / 3))
		half_fov = deg_to_rad(90)/2;
	
		if (aimbot.is_pistol(weapon) && config_system.item.aim_fov_pistol >= 1) {
			float d_draw_aim_fov_radius_pistol = tan(deg_to_rad(config_system.item.aim_fov_pistol) / 2) / tan(half_fov) * (width / 2);
			int i_draw_aim_fov_radius_pistol = std::round(d_draw_aim_fov_radius_pistol);
			render.draw_circle(width / 2, height / 2, i_draw_aim_fov_radius_pistol, i_draw_aim_fov_radius_pistol, color(0, 255, 255, 255));
		}

		if (aimbot.is_smg(weapon) && config_system.item.aim_fov_smg >= 1) {
			float d_draw_aim_fov_radius_smg = tan(deg_to_rad(config_system.item.aim_fov_smg) / 2) / tan(half_fov) * (width / 2);
			int i_draw_aim_fov_radius_smg = std::round(d_draw_aim_fov_radius_smg);
			render.draw_circle(width / 2, height / 2, i_draw_aim_fov_radius_smg, i_draw_aim_fov_radius_smg, color(0, 255, 255, 255));
		}

		if (aimbot.is_sniper(weapon) && config_system.item.aim_fov_sniper >= 1 && !local_player->is_scoped()) {
			float d_draw_aim_fov_radius_sniper = tan(deg_to_rad(config_system.item.aim_fov_sniper) / 2) / tan(half_fov) * (width / 2);
			int i_draw_aim_fov_radius_sniper = std::round(d_draw_aim_fov_radius_sniper);
			render.draw_circle(width / 2, height / 2, i_draw_aim_fov_radius_sniper, i_draw_aim_fov_radius_sniper, color(0, 255, 255, 255));

		}

		if (aimbot.is_heavy(weapon) && config_system.item.aim_fov_heavy >= 1) {
			float d_draw_aim_fov_radius_heavy = tan(deg_to_rad(config_system.item.aim_fov_heavy) / 2) / tan(half_fov) * (width / 2);
			int i_draw_aim_fov_radius_heavy = std::round(d_draw_aim_fov_radius_heavy);
			render.draw_circle(width / 2, height / 2, i_draw_aim_fov_radius_heavy, i_draw_aim_fov_radius_heavy, color(0, 255, 255, 255));
		}

		if (aimbot.is_rifle(weapon) && config_system.item.aim_fov_rifle >= 1) {
			float d_draw_aim_fov_radius_rifle = tan(deg_to_rad(config_system.item.aim_fov_rifle) / 2) / tan(half_fov) * (width / 2);
			int i_draw_aim_fov_radius_rifle = std::round(d_draw_aim_fov_radius_rifle);
			render.draw_circle(width / 2, height / 2, i_draw_aim_fov_radius_rifle, i_draw_aim_fov_radius_rifle, color(0, 255, 255, 255));
		}
}

void c_misc::disable_post_processing() noexcept {
	if (!config_system.item.misc_enabled)
		return;

	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_postprocess_enable");
	mat_postprocess_enable->set_value(config_system.item.disable_post_processing ? 0 : 1);
}

void c_misc::force_crosshair() noexcept {
	if (!config_system.item.misc_enabled)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	static auto weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");

	if (local_player && local_player->health() > 0) {
		weapon_debug_spread_show->set_value(local_player->is_scoped() || !config_system.item.force_crosshair ? 0 : 3);
	}
}
