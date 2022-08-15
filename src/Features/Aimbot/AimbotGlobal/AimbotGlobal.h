#pragma once
#include "../../../SDK/SDK.h"

#undef min
#undef max

enum struct ETargetType {
	UNKNOWN, PLAYER, BUILDING
};

enum struct ESortMethod {
	UNKNOWN, FOV, DISTANCE
};

struct Target_t
{
	C_BaseAnimating* m_pEntity = nullptr;
	ETargetType m_TargetType = ETargetType::UNKNOWN;
	Vector m_vPos = {};
	QAngle m_vAngleTo = {};
	float m_flFOVTo = std::numeric_limits<float>::max();
	float m_flDistTo = std::numeric_limits<float>::max();
	int m_nAimedHitbox = -1;
	bool m_bHasMultiPointed = false;
};

class CAimbotGlobal
{
public:
	bool IsKeyDown();
	void SortTargets(const ESortMethod& Method);
	const Target_t& GetBestTarget(const ESortMethod& Method);

public:
	std::vector<Target_t> m_vecTargets = {};
};

inline CAimbotGlobal g_AimbotGlobal;