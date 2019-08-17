#include "visuals.hpp"
#include "../backtrack/backtrack.hpp"
#include "../../../dependencies/common_includes.hpp"

CVisuals visuals;

void CVisuals::run() noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!config_system.item.visuals_enabled)
		return;

	if (config_system.item.anti_screenshot && interfaces::engine->is_taking_screenshot())
		return;

	if (!local_player)
		return;

	//non player drawing loop
	for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (entity && entity != local_player) {
			auto client_class = entity->client_class();
			auto model_name = interfaces::model_info->get_model_name(entity->model());

			if (client_class->class_id == class_ids::cplantedc4) { //this should be fixed in better ways than this - designer
				bomb_esp(entity);
				bomb_defuse_esp(entity);
			}
		}
	}
}

void CVisuals::bomb_esp(player_t* entity) noexcept {
	if (!config_system.item.bomb_planted)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!entity)
		return;

	if (!local_player)
		return;

	auto class_id = entity->client_class()->class_id;
	auto explode_time = entity->c4_blow_time();

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	vec3_t bomb_origin, bomb_position;

	bomb_origin = entity->origin();

	explode_time -= interfaces::globals->interval_per_tick * local_player->get_tick_base();
	if (explode_time <= 0)
		explode_time = 0;

	char buffer[64];
	sprintf_s(buffer, "%.1f", explode_time);

	auto c4_timer = interfaces::console->get_convar("mp_c4timer")->get_int();
	auto value = (explode_time * height) / c4_timer;

	float damage;
	auto distance = local_player->get_eye_pos().distance_to(entity->get_eye_pos());
	auto a = 450.7f, b = 75.68f, c = 789.2f, d = ((distance - b) / c), fl_damage = a * exp(-d * d);
	damage = float((std::max)((int)ceilf(utilities::csgo_armor(fl_damage, local_player->armor())), 0));

	std::string damage_text;
	damage_text += "-";
	damage_text += std::to_string((int)(damage));
	damage_text += "HP";

	if (explode_time <= 10) {
		render.draw_filled_rect(0, 0, 10, value, color(255, 0, 0, 180));
	}
	else {
		render.draw_filled_rect(0, 0, 10, value, color(0, 255, 0, 180));
	}

	render.draw_text(12, value - 11, render.name_font, buffer, false, color(255, 255, 255));

	if (local_player->is_alive()) {
		render.draw_text(12, value - 21, render.name_font, damage_text, false, color(255, 255, 255));
	}

	if (local_player->is_alive() && damage >= local_player->health()) {
		render.draw_text(12, value - 31, render.name_font, "FATAL", false, color(255, 255, 255));
	}

	if (!math.world_to_screen(bomb_origin, bomb_position))
		return;

	render.draw_text(bomb_position.x, bomb_position.y, render.name_font, buffer, true, color(255, 255, 255));
	render.draw_filled_rect(bomb_position.x - c4_timer / 2, bomb_position.y + 13, c4_timer, 3, color(10, 10, 10, 180));
	render.draw_filled_rect(bomb_position.x - c4_timer / 2, bomb_position.y + 13, explode_time, 3, color(167, 24, 71, 255));
}

void CVisuals::bomb_defuse_esp(player_t* entity) noexcept {
	if (!config_system.item.bomb_planted)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	auto remaining_time = entity->c4_blow_time() - (interfaces::globals->interval_per_tick * local_player->get_tick_base());
	auto countdown = entity->c4_defuse_countdown() - (interfaces::globals->interval_per_tick * local_player->get_tick_base());
	auto start_defusing = entity->c4_gets_defused();

	char defuse_time_string[24];
	sprintf_s(defuse_time_string, sizeof(defuse_time_string) - 1, "%.1f", countdown);
	auto defuse_value = (countdown * height) / (remaining_time * 2);

	vec3_t bomb_origin, bomb_position;
	bomb_origin = entity->origin();

	if (!math.world_to_screen(bomb_origin, bomb_position))
		return;

	if (start_defusing > 0) {
		if (remaining_time > countdown) {
			render.draw_filled_rect(10, 0, 10, defuse_value, color(0, 191, 255, 180));// on srcreen
			render.draw_text(12, defuse_value - 11, render.name_font, defuse_time_string, false, color(0, 191, 255));// on srcreen
			render.draw_text(bomb_position.x, bomb_position.y - 18, render.name_font, defuse_time_string, true, color(0, 191, 255));// on bomb
		}
		else {
			render.draw_filled_rect(10, 0, 10, defuse_value, color(255, 0, 0, 180));// on srcreen
			render.draw_text(12, defuse_value - 11, render.name_font, "NO TIME", false, color(255, 0, 0));// on srcreen
			render.draw_text(bomb_position.x, bomb_position.y - 18, render.name_font, "NO TIME", true, color(255, 0, 0));// on bomb
		}
	}
}

void CVisuals::chams() noexcept {
	if (!config_system.item.visuals_enabled || (!config_system.item.vis_chams_vis && !config_system.item.vis_chams_invis))
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

		if (!entity || !entity->is_alive() || entity->dormant() || !local_player)
			continue;

		bool is_teammate = entity->team() == local_player->team();
		bool is_enemy = entity->team() != local_player->team();

		static i_material* mat = nullptr;
		auto textured = interfaces::material_system->find_material("aristois_material", TEXTURE_GROUP_MODEL, true, nullptr);
		auto metalic = interfaces::material_system->find_material("aristois_reflective", TEXTURE_GROUP_MODEL, true, nullptr);
		auto dogtag = interfaces::material_system->find_material("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL, true, nullptr);
		auto flat = interfaces::material_system->find_material("debug/debugdrawflat", TEXTURE_GROUP_MODEL, true, nullptr);
		textured->increment_reference_count();  //we need increment_reference_count cuz without it our materialsystem.dll will crash after  map change - designer
		metalic->increment_reference_count();
		flat->increment_reference_count();
		dogtag->increment_reference_count();

		switch (config_system.item.vis_chams_type) {
		case 0:
			mat = textured;
			break;
		case 1:
			mat = flat;
			break;
		case 2:
			mat = metalic;
			break;
		case 3:
			mat = dogtag;
			break;
		}

		if (is_enemy) {
			if (config_system.item.vis_chams_invis) {
				if (utilities::is_behind_smoke(local_player->get_eye_pos(), entity->get_hitbox_position(entity, hitbox_head)) && config_system.item.vis_chams_smoke_check)
					return;
				interfaces::render_view->modulate_color(config_system.item.clr_chams_invis);
				interfaces::render_view->set_blend(config_system.item.clr_chams_invis[3]);
				mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, true);

				interfaces::model_render->override_material(mat);
				entity->draw_model(1, 255);
			}
			if (config_system.item.vis_chams_vis) {
				if (utilities::is_behind_smoke(local_player->get_eye_pos(), entity->get_hitbox_position(entity, hitbox_head)) && config_system.item.vis_chams_smoke_check)
					return;

				interfaces::render_view->modulate_color(config_system.item.clr_chams_vis);
				interfaces::render_view->set_blend(config_system.item.clr_chams_vis[3]);
				mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, false);

				interfaces::model_render->override_material(mat);
				entity->draw_model(1, 255);
			}
		}

		if (is_teammate) {
			if (config_system.item.vis_chams_invis_teammate) {
				interfaces::render_view->modulate_color(config_system.item.clr_chams_invis_teammate);
				interfaces::render_view->set_blend(config_system.item.clr_chams_invis_teammate[3]);
				mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, true);

				interfaces::model_render->override_material(mat);
				entity->draw_model(1, 255);
			}
			if (config_system.item.vis_chams_vis_teammate) {
				interfaces::render_view->modulate_color(config_system.item.clr_chams_vis_teammate);
				interfaces::render_view->set_blend(config_system.item.clr_chams_vis_teammate[3]);
				mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, false);

				interfaces::model_render->override_material(mat);
				entity->draw_model(1, 255);
			}
		}

		interfaces::model_render->override_material(nullptr);
	}
}

void CVisuals::glow() noexcept {
	if (!config_system.item.visuals_enabled || !config_system.item.visuals_glow)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	for (size_t i = 0; i < interfaces::glow_manager->size; i++) {
		auto &glow = interfaces::glow_manager->objects[i];

		if (glow.unused())
			continue;

		auto glow_entity = reinterpret_cast<player_t*>(glow.entity);
		auto client_class = glow_entity->client_class();

		if (!glow_entity || glow_entity->dormant())
			continue;

		auto is_enemy = glow_entity->team() != local_player->team();
		auto is_teammate = glow_entity->team() == local_player->team();

		switch (client_class->class_id) {
		case class_ids::ccsplayer:
			if (is_enemy && config_system.item.visuals_glow_enemy) {
				glow.set(config_system.item.clr_glow[0], config_system.item.clr_glow[1], config_system.item.clr_glow[2], config_system.item.clr_glow[3]);
			}

			else if (is_teammate && config_system.item.visuals_glow_team) {
				glow.set(config_system.item.clr_glow_team[0], config_system.item.clr_glow_team[1], config_system.item.clr_glow_team[2], config_system.item.clr_glow_team[3]);
			}
			break;
		case class_ids::cplantedc4:
			if (config_system.item.visuals_glow_planted) {
				glow.set(config_system.item.clr_glow_planted[0], config_system.item.clr_glow_planted[1], config_system.item.clr_glow_planted[2], config_system.item.clr_glow_planted[3]);
			}
			break;
		}

		if (strstr(client_class->network_name, ("CWeapon")) && config_system.item.visuals_glow_weapons) {
			glow.set(config_system.item.clr_glow_dropped[0], config_system.item.clr_glow_dropped[1], config_system.item.clr_glow_dropped[2], config_system.item.clr_glow_dropped[3]);
		}

		else if (client_class->class_id == class_ids::cak47 && config_system.item.visuals_glow_weapons) {
			glow.set(config_system.item.clr_glow_dropped[0], config_system.item.clr_glow_dropped[1], config_system.item.clr_glow_dropped[2], config_system.item.clr_glow_dropped[3]);
		}

		else if (client_class->class_id == class_ids::cc4 && config_system.item.visuals_glow_weapons) {
			glow.set(config_system.item.clr_glow_dropped[0], config_system.item.clr_glow_dropped[1], config_system.item.clr_glow_dropped[2], config_system.item.clr_glow_dropped[3]);
		}

		else if (client_class->class_id == class_ids::cdeagle && config_system.item.visuals_glow_weapons) {
			glow.set(config_system.item.clr_glow_dropped[0], config_system.item.clr_glow_dropped[1], config_system.item.clr_glow_dropped[2], config_system.item.clr_glow_dropped[3]);
		}

	}
}

void CVisuals::backtrack_chams(IMatRenderContext* ctx, const draw_model_state_t& state, const model_render_info_t& info) noexcept {
	if (!config_system.item.backtrack_visualize || !interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto model_name = interfaces::model_info->get_model_name((model_t*)info.model);

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	if (!local_player || !local_player->is_alive() || !entity)
		return;

	static auto draw_model_execute_fn = reinterpret_cast<hooks::draw_model_execute_fn>(hooks::modelrender_hook->get_original(21));

	if (strstr(model_name, "models/player"))
		if (entity && entity->is_alive() && !entity->dormant()) {
			int i = entity->index();

			if (local_player && local_player->is_alive() && entity->team() != local_player->team()) {
				auto record = &records[info.entity_index];
				if (!record)
					return;

				if (record && record->size() && backtrack.valid_tick(record->front().simulation_time)) {
					draw_model_execute_fn(interfaces::model_render, ctx, state, info, record->back().matrix);
					interfaces::model_render->override_material(nullptr);
				}
			}
		}

}