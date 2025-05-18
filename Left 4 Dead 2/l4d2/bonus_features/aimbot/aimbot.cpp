#include "aimbot.h"
#include "../../globals/globals.h"
#include "../../modules/modules.h"
#include "../../sdk/vec/vec3.h"
#include "../../sdk/entity/c_base_entity.h"
#include "../../game_utilities/game_utilities.h"
#include "../../config/config.h"
#include "../../math/math.h"

aimbot g_aimbot;

/*
-------- TO-DO LIST --------
1. Awall
2. Angle rotation resolving
3. Is dying check
-------- TO-DO LIST --------
*/

bool Visible(c_base_player* local, c_base_player* entity)
{
	vec3 start = local->get_bone_position(14);
	vec3 end = entity->get_bone_position(15);

	ray_t ray(start, end);
	trace_class tr;
	trace_filter filter(local);

	g_l4d2.m_tracers->get_trace_ray(ray, &filter, &tr);

	return (tr.entity == entity || tr.fraction > 0.97f);
};

void aimbot::run(c_user_cmd* cmd) noexcept
{

	if (!g_var.m_aim_bot_var
		|| !g_l4d2.m_engine->is_playing())
		return;

	if (!g_var.m_aim_fire_var
		&& !(cmd->buttons & player_controls::in_attack))
		return;

	auto local = g.get_local();
	if (!local
		|| !local->get_flags() & !fl_on_ground)
		return;

	auto weapon = local->get_active_weapon();
	if (!weapon || !is_gun_weapon(weapon->get_item_definition_index()))
		return;

	if (weapon->reloading()
		|| !weapon->has_ammo())
		return;

	vec3 eyePos = local->get_eye_pos();
	vec3 viewAngles = cmd->viewangles;

	float aimbotFov = g_var.m_aim_fov_var;

	int me = g_l4d2.m_engine->get_local_player();
	int maxE = g_l4d2.m_entity_list->get_max_entity();

	c_base_player* closestTarget = nullptr;
	vec3 closestTargetPos{};
	float closestDistance = FLT_MAX;

	for (int i = 1; i <= maxE; i++)
	{
		if (i == me)
			continue;

		auto ent = reinterpret_cast<c_base_player*>(
			g_l4d2.m_entity_list->get_client_entity(i)
			);
		if (!ent)
			continue;

		auto cls = ent->client_class();
		if (!cls)
			continue;

		switch (cls->class_id) {

		case ET_CHARGER:
		case ET_BOOMER:
		case ET_WITCH:
		case ET_SMOKER:
		case ET_HUNTER:
		case ET_TANK:
		case ET_SPITTER:
		case ET_INFECTED:
		case ET_JOCKEY:
			break;        
		default:
			continue;    
		}

		if (ent->is_dormant()
			|| ent->is_dying()
			|| ent->is_dead()
			|| ent->get_life_state() == 0
			|| ent->get_sequence() >= 305)
			continue;
		if (ent->team() == local->team())
			continue;

		vec3 targetPos;
		switch (cls->class_id) {
		case ET_JOCKEY:
			targetPos = ent->get_bone_position(14);
			break;
		default:
			targetPos = ent->get_bone_position(16);
			break;
		}

		if (targetPos.is_zero())
			targetPos = ent->get_eye_pos();
		if (targetPos.is_zero())
			continue;

		vec3 delta = targetPos - eyePos;
		float distance = delta.length();
		vec3 aimAngle = delta.ToAngle();
		vec3 deltaAng = aimAngle - viewAngles;

		if (!Visible(local, ent))
			continue;

		float fov = std::hypot(deltaAng.x, deltaAng.y);
		if (fov > aimbotFov)
			continue;

		if (distance < closestDistance) {
			closestDistance = distance;
			closestTarget = ent;
			closestTargetPos = targetPos;
		}
	}

	if (closestTarget)
	{
		vec3 delta = closestTargetPos - eyePos;
		vec3 aimAngle = delta.ToAngle();
		vec3 deltaAng = aimAngle - viewAngles;
		deltaAng.clamp();

		vec3 wishangle = cmd->viewangles;
		cmd->viewangles += deltaAng;

		if (g_var.m_aim_compensation_var)
		{
			vec3 punch = local->get_punch_angle();

			static vec3 lastPunch{ 0.0f, 0.0f, 0.0f };
			vec3 deltaPunch = punch - lastPunch;
			lastPunch = punch;

			vec3 delta = closestTargetPos - eyePos;
			vec3 aimAngle = delta.ToAngle();

			constexpr float recoilScale = 2.0f;
			aimAngle.x -= deltaPunch.x * recoilScale;
			aimAngle.y -= deltaPunch.y * recoilScale;

			vec3 deltaAng = aimAngle - viewAngles;
			deltaAng.clamp();

			vec3 wishangle = cmd->viewangles;
			cmd->viewangles += deltaAng;
		}

		if (g_var.m_aim_fire_var)
		{	
			float nextAttackTime = weapon->next_primary_attack();
			float currentTime = local->get_tick_base() * g_l4d2.m_globals->interval_per_tick;

			if (nextAttackTime <= currentTime)
				cmd->buttons |= player_controls::in_attack;
		}

		// Movement fix starts here
		vec3 realF, realR, realU;
		vec3 realAngle = cmd->viewangles;
		realAngle.normalize();

		math::angle_vectors(realAngle, realF, realR, realU);
		realF.z = 0; realR.z = 0;
		realF.normalize(); realR.normalize();

		vec3 wishF, wishR, wishU;
		wishangle.normalize();
		math::angle_vectors(wishangle, wishF, wishR, wishU);
		wishF.z = 0; wishR.z = 0;
		wishF.normalize(); wishR.normalize();

		vec3 wishVel;
		wishVel.x = wishF.x * cmd->forwardmove + wishR.x * cmd->sidemove;
		wishVel.y = wishF.y * cmd->forwardmove + wishR.y * cmd->sidemove;
		wishVel.z = 0;

		float a = realF.x, b = realR.x, c = realF.y, d = realR.y;
		float v = wishVel.x, w = wishVel.y;

		float det = (a * d - b * c);
		if (det != 0.0f)
		{
			float newForward = (d * v - b * w) / det;
			float newSide = (a * w - c * v) / det;

			cmd->forwardmove = newForward;
			cmd->sidemove = newSide;
		}
	}
}