#pragma once

#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbotHitscan
{
private:
	int GetHitbox(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon);
	ESortMethod GetSortMethod();
	bool GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon);
	bool ScanHitboxes(C_TFPlayer* pLocal, Target_t& Target);
	bool ScanHead(C_TFPlayer* pLocal, Target_t& Target);
	bool ScanBuildings(C_TFPlayer* pLocal, Target_t& Target);
	bool VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Target);
	bool GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Out);
	void Aim(CUserCmd* pCmd, QAngle& vAngle);
	bool ShouldFire(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, const Target_t& Target);
	bool IsAttacking(CUserCmd* pCmd, C_TFWeaponBase* pWeapon);

public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd);
};

inline CAimbotHitscan g_AimbotHitscan;