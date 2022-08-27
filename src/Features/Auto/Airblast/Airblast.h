#pragma once

#include "../../../SDK/SDK.h"

class CAutoAirblast
{
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);
	int id;
};

namespace F { inline CAutoAirblast Airblast; }