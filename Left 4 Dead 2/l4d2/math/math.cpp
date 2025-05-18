#include "math.h"
#include <cmath>

math g_math;

void math::sin_cos(float radians, float* sine, float* cosine)
{
	*sine = std::sin(radians);
	*cosine = std::cos(radians);
}

void math::forward_vectors(const vec3& angles, vec3& forward)
{
	float sp, sy, cp, cy;

	sin_cos(DEG2RAD(angles.x), &sp, &cp); // pitch
	sin_cos(DEG2RAD(angles.y), &sy, &cy); // yaw

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void math::angle_vectors(const vec3& angles, vec3& forward, vec3& right, vec3& up)
{
	float sp, cp, sy, cy, sr, cr;

	sin_cos(DEG2RAD(angles.x), &sp, &cp); // Pitch
	sin_cos(DEG2RAD(angles.y), &sy, &cy); // Yaw
	sin_cos(DEG2RAD(angles.z), &sr, &cr); // Roll

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;

	right.x = -1 * sr * sp * cy + -1 * cr * -sy;
	right.y = -1 * sr * sp * sy + -1 * cr * cy;
	right.z = -1 * sr * cp;

	up.x = cr * sp * cy + -sr * -sy;
	up.y = cr * sp * sy + -sr * cy;
	up.z = cr * cp;
}
