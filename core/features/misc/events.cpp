#include "events.hpp"
#include "../misc/logs.hpp"
#include "../misc/hitmarker.hpp"
#include "../aimbot/aimbot.hpp"

#pragma comment(lib, "Winmm.lib")

CHookedEvents events;

void CHookedEvents::fire_game_event(i_game_event* event) noexcept {
	auto event_name = event->get_name();

	if (!strcmp(event_name, "player_hurt")) {
		hitmarker.event(event);
		event_logs.event_player_hurt(event);
	}

	else if (strstr(event->get_name(), "item_purchase")) {
		event_logs.event_item_purchase(event);
	}

	else if (!strcmp(event_name, "player_death")) {
		aimbot.event_player_death(event);
	}
}

int CHookedEvents::get_event_debug_id(void) noexcept {
	return EVENT_DEBUG_ID_INIT;
}

void CHookedEvents::setup() noexcept {
	m_i_debug_id = EVENT_DEBUG_ID_INIT;
	interfaces::event_manager->add_listener(this, "player_hurt", false);
	interfaces::event_manager->add_listener(this, "item_purchase", false);
	interfaces::event_manager->add_listener(this, "player_footstep", false);
	interfaces::event_manager->add_listener(this, "player_death", false);
}

void CHookedEvents::release() noexcept {
	interfaces::event_manager->remove_listener(this);
}
