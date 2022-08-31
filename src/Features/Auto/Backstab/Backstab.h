#pragma once

#include "../../../SDK/SDK.h"

class CBackstab
{
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWep, CUserCmd* pCmd);
};

namespace F { inline CBackstab Backstab; }