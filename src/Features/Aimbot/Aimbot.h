#pragma once
#include "../../SDK/SDK.h"

class CAimbot
{
private:
	bool ShouldRun(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon);

public:
	void Run(CUserCmd* pCmd);
};

namespace F { inline CAimbot Aimbot; }