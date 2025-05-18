#pragma once

#include "../sdk/vec/vec3.h"

using matrix3x4_t = float[3][4];
using VMatrix = float[4][4];

struct math {
public:
	static void sin_cos(float radians, float* sine, float* cosine);
	static void forward_vectors(const vec3 & angles, vec3 & forward);
	static void angle_vectors(const vec3 & angles, vec3 & forward, vec3 & right, vec3 & up);
public:
	template<typename F, typename ... T>
	inline bool CompareGroup(F&& first, T&& ... t) {
		return ((first == t) || ...);
	}
};

extern math g_math;