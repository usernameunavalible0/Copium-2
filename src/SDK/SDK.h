#pragma once

#include "DrawManager/DrawManager.h"

#include <random>

#define XASSERT(x) if (x) Error(#x)

#define FAILED_TO_INITIALIZE false
#define FAILED_TO_HOOK false

inline float MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline int RandIntSimple(int min, int max)
{
	std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distr(min, max);
	return distr(gen);
}

class CUtil
{
public:
	void FixMovement(const QAngle& va, CUserCmd* cmd);

	bool IsVisible(const Vector& vStart, const Vector& vEnd);

	Color GetTeamColor(const int nTeam);
	Color GetHealthColor(const int nHealth, const int nMaxHealth);

	IMaterial* CreateMaterial(const char* szVars);
};

namespace G { inline CUtil Util; }