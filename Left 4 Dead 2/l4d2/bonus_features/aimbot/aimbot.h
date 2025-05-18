#pragma once
#include "../../sdk/entity/c_base_entity.h"
#include "../../sdk/vec/vec3.h"
struct c_user_cmd;
class c_base_player;

class aimbot {
public:
	void run(c_user_cmd* cmd) noexcept;
private:
	int m_currentTarget = -1;
	int m_shotCount = 0;
};

extern aimbot g_aimbot;
