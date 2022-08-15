#pragma once
#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbotProjectile
{
private:
	struct ProjectileInfo_t
	{
		float m_flVelocity = 0.0f;
		float m_flGravity = 0.0f;
		float m_flMaxTime = 2.0f;
	};

	struct Predictor_t
	{
		C_BaseAnimating* m_pEntity = nullptr;
		Vector m_vPosition = {};
		Vector m_vVelocity = {};
		Vector m_vAcceleration = {};

		Vector Extrapolate(float time);
	};

	struct Solution_t
	{
		float m_flPitch = 0.0f;
		float m_flYaw = 0.0f;
		float m_flTime = 0.0f;
	};

	bool GetProjectileInfo(C_TFWeaponBase* pWeapon, ProjectileInfo_t& out);
	bool CalcProjAngle(const Vector& vLocalPos, const Vector& vTargetPos, const ProjectileInfo_t& ProjInfo, Solution_t& out);
	bool SolveProjectile(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, Predictor_t& Predictor, const ProjectileInfo_t& ProjInfo, Solution_t& out);

private:
	Vector GetAimPos(C_TFPlayer* pLocal, C_BaseAnimating* pEntity);
	ESortMethod GetSortMethod();
	bool GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon);
	bool VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, Target_t& Target);
	bool GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, Target_t& Out);
	void Aim(CUserCmd* pCmd, C_TFWeaponBase* pWeapon, QAngle& vAngle);
	bool ShouldFire(CUserCmd* pCmd);
	bool IsAttacking(CUserCmd* pCmd, C_TFWeaponBase* pWeapon);

	bool m_bIsFlameThrower;

public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd);
};

inline CAimbotProjectile g_AimbotProjectile;