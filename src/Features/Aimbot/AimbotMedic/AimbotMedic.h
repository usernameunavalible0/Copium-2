#pragma once

#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbotMedic
{
public:

	void Run(C_TFPlayer* pLocal, CUserCmd* pCommand);

private:

	IClientEntity* GetBestTarget(C_TFPlayer* pLocal);

	int GetBestHitbox(C_TFPlayer* pLocal, C_TFPlayer* pEntity);
};

inline CAimbotMedic g_AimbotMedic;