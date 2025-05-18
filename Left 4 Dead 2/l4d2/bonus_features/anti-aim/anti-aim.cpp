#include "anti-aim.h"
#include "../../globals/globals.h"
#include "../../modules/modules.h"
#include "../../sdk/vec/vec3.h"
#include "../../sdk/entity/c_base_entity.h"
#include "../../game_utilities/game_utilities.h"
#include "../../config/config.h"
#include "../../math/math.h"
#include "../aimbot/aimbot.h"

antiaim g_antiaim;

void antiaim::run(c_user_cmd* cmd) noexcept
{
	if (!g_var.m_anti_aim_var    // anti-aim master switch
		|| !g_l4d2.m_engine->is_playing())
		return;

	auto local = g.get_local();
	if (!local || !local->is_alive())
		return;

	// If we're holding down fire, use the aimbot angles instead
	if (cmd->buttons & player_controls::in_attack) {
		// Assume g_utils.find_best_target_angle fills cmd->viewangles appropriately
		vec3 aimAngle;
		if (aimbot.find_best_target_angle(aimAngle)) {
			cmd->viewangles = aimAngle;
		}
		return;
	}

	static float lastUpdate = 0.0f;
	static float desiredPitch = 0.0f;
	float curtime = g_l4d2.m_globals->curtime;

	// Every 0.5 seconds pick a new random pitch
	if (curtime - lastUpdate > 0.5f) {
		lastUpdate = curtime;
		// Random float in [-89, +89]
		desiredPitch = g_math.random_float(-89.0f, 89.0f);
	}

	// You can also jumble yaw here if you want:
	// static float desiredYaw = 0.0f;
	// desiredYaw += g_math.random_float(-45.0f, 45.0f);

	// Apply anti-aim angles
	cmd->viewangles.x = desiredPitch;
	// cmd->viewangles.y = desiredYaw;  // uncomment if you randomize yaw

	// leave roll alone
}
