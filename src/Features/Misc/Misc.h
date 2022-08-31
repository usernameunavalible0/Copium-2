#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Misc
{
public:
	void Run(C_TFPlayer* pLocal, CUserCmd* cmd);
	//void NeedleGunNoSpread(C_TFWeaponBase* pWep, CUserCmd* cmd);

private:
	void Bunnyhop(C_TFPlayer* pLocal, CUserCmd* cmd);
	void AutoStrafe(CUserCmd* pCmd);
	void AntiWarp(C_TFPlayer* pLocal, CUserCmd* cmd);
};

namespace F { inline CFeatures_Misc Misc; }