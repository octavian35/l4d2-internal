#pragma once
#include "../../sdk/entity/c_base_entity.h"
#include "../../sdk/vec/vec3.h"
struct c_user_cmd;
class c_base_player;

class antiaim {
public:
	void run(c_user_cmd* cmd) noexcept;
};

extern antiaim g_antiaim;
