#pragma once

class CMovement {
public:
	void bunnyhop(c_usercmd* user_cmd) noexcept;
	void edge_jump_pre_prediction(c_usercmd * user_cmd) noexcept;
	void edge_jump_post_prediction(c_usercmd * user_cmd) noexcept;
};

extern CMovement movement;