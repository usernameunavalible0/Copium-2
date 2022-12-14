#pragma once

#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbotMelee
{
private:
	bool CanMeleeHit(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, const QAngle& vecViewAngles, int nTargetIndex);

private:
	ESortMethod GetSortMethod();
	bool GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon);
	bool VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Target);
	bool GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Out);
	void Aim(CUserCmd* pCmd, QAngle& vAngle);
	bool ShouldSwing(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, const Target_t& Target);
	bool IsAttacking(CUserCmd* pCmd, C_TFWeaponBase* pWeapon);

public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd);
};

inline CAimbotMelee g_AimbotMelee;