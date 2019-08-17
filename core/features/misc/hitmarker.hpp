#pragma once
#include "../../../dependencies/common_includes.hpp"

class CHitmarker {
public:
	void run() noexcept;
	void event(i_game_event * event) noexcept;
protected:
	void draw() noexcept;
};

extern CHitmarker hitmarker;