#pragma once
#include "../../../dependencies/common_includes.hpp"

class c_misc {
public:
	void rank_reveal() noexcept;
	void spectators() noexcept;
	void watermark() noexcept;
	void draw_aim_fov();
	void disable_post_processing() noexcept;
	void force_crosshair() noexcept;
private:
};

extern c_misc misc;
