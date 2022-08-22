#pragma once

#include "DrawManager/DrawManager.h"

#include <random>

#define XASSERT(x) if (x) Error(#x)

#define FAILED_TO_INITIALIZE false
#define FAILED_TO_HOOK false

#define M_RADPI		57.295779513082
#define PI			3.14159265358979323846

#define floatCompare(x, y) \
	(fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

inline QAngle m_vPunchAngles;

inline Vector m_vPredictedPos = {};

inline EWeaponType GetWeaponType(C_TFWeaponBase* pWeapon)
{
	if (!pWeapon)
		return EWeaponType::UNKNOWN;

	if (pWeapon->GetSlot() == 2)
		return EWeaponType::MELEE;

	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_ROCKETLAUNCHER:
	case TF_WEAPON_GRENADELAUNCHER:
	case TF_WEAPON_FLAREGUN:
	case TF_WEAPON_COMPOUND_BOW:
	case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
	case TF_WEAPON_CROSSBOW:
	case TF_WEAPON_PARTICLE_CANNON:
	case TF_WEAPON_DRG_POMSON:
	case TF_WEAPON_FLAREGUN_REVENGE:
	case TF_WEAPON_CANNON:
	case TF_WEAPON_SYRINGEGUN_MEDIC:
	case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
	case TF_WEAPON_FLAMETHROWER:
	{
		return EWeaponType::PROJECTILE;
	}

	case TF_WEAPON_PIPEBOMBLAUNCHER:
	case 109: //dragon's fury
	{
		//broken Idk
		return EWeaponType::UNKNOWN;
	}
	
	case TF_WEAPON_MEDIGUN:
	{
		return EWeaponType::MEDIGUN;
	}

	default:
	{
		int nDamageType = pWeapon->GetDamageType();

		if (nDamageType & DMG_BULLET || nDamageType && DMG_BUCKSHOT)
			return EWeaponType::HITSCAN;

		break;
	}
	}

	return EWeaponType::UNKNOWN;
}


inline void GetProjectileFireSetup(C_TFPlayer* pPlayer, const QAngle &vViewAngles, Vector vOffset, Vector* vSrc)
{
	if (G::ConVars.cl_flipviewmodels->GetInt())
		vOffset.y *= -1.0f;

	Vector vecForward = Vector(), vecRight = Vector(), vecUp = Vector();
	AngleVectors(vViewAngles, &vecForward, &vecRight, &vecUp);

	*vSrc = pPlayer->Weapon_ShootPosition() + (vecForward * vOffset.x) + (vecRight * vOffset.y) + (vecUp * vOffset.z);
}

inline bool CanShoot(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon) {
	if (!pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->InCond(TF_COND_PHASE) || pLocal->InCond(TF_COND_HALLOWEEN_GHOST_MODE) || pLocal->InCond(TF_COND_HALLOWEEN_KART))
		return false;

	if (pLocal->m_iClass() == TF_CLASS_SPY)
	{
		{ //DR
			static float flTimer = 0.0f;

			if (pLocal->m_bFeignDeathReady()) {
				flTimer = 0.0f;
				return false;
			}
			else {
				if (!flTimer)
					flTimer = I::GlobalVars->curtime;

				if ((I::GlobalVars->curtime - flTimer) < 0.4f)
					return false;
			}
		}

		{ //Invis
			static float flTimer = 0.0f;

			if (pLocal->IsCloaked()) {
				flTimer = 0.0f;
				return false;
			}
			else {
				if (!flTimer)
					flTimer = I::GlobalVars->curtime;

				if ((I::GlobalVars->curtime - flTimer) < 2.0f)
					return false;
			}
		}
	}

	return (pWeapon->m_flNextPrimaryAttack() < pLocal->m_nTickBase() * I::GlobalVars->interval_per_tick);
}

//adapted from https://github.com/gszauer/GamePhysicsCookbook/blob/15810bbf902c1cc19064c176a7e0626eda3b83bd/Code/Geometry3D.cpp#L584
inline bool RayToOBB(const  Vector & origin, const  Vector & direction, const Vector & position, const Vector & min, const Vector & max, const matrix3x4_t orientation) {

	Vector p = position - origin;

	Vector X(orientation[0][0], orientation[0][1], orientation[0][2]);
	Vector Y(orientation[1][0], orientation[1][1], orientation[1][2]);
	Vector Z(orientation[2][0], orientation[2][1], orientation[2][2]);

	Vector f(
		X.Dot(direction),
		Y.Dot(direction),
		Z.Dot(direction)
	);

	Vector e(
		X.Dot(p),
		Y.Dot(p),
		Z.Dot(p)
	);

	float t[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 3; ++i) {
		if (floatCompare(f[i], 0)) {
			if (-e[i] + min[i] > 0 || -e[i] + max[i] < 0) {
				return false;
			}
			f[i] = 0.00001f; // Avoid div by 0!
		}

		t[i * 2 + 0] = (e[i] + max[i]) / f[i]; // tmin[x, y, z]
		t[i * 2 + 1] = (e[i] + min[i]) / f[i]; // tmax[x, y, z]
	}

	float tmin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));
	float tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));

	// if tmax < 0, ray is intersecting AABB
	// but entire AABB is behing it's origin
	if (tmax < 0) {
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) {
		return false;
	}

	return true;
}

inline bool VisPosHitboxIdOut(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int& nHitboxOut)
{
	CGameTrace trace = {};
	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pSkip);
	UTIL_TraceLine(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

	if (trace.m_pEnt && trace.m_pEnt == pEntity) {
		nHitboxOut = trace.hitbox;
		return true;
	}

	return false;
}

inline bool VisPosHitboxId(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to, int nHitbox)
{
	CGameTrace trace = {};
	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pSkip);
	UTIL_TraceLine(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);
	return (trace.m_pEnt && trace.m_pEnt == pEntity && trace.hitbox == nHitbox);
}

inline bool VisPos(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vector& from, const Vector& to)
{
	CGameTrace trace = {};
	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pSkip);
	UTIL_TraceLine(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);
	return ((trace.m_pEnt && trace.m_pEnt == pEntity) || trace.fraction > 0.99f);
}

inline QAngle CalcAngle(const Vector & source, const Vector & destination)
{
	QAngle angles = QAngle(0.0f, 0.0f, 0.0f);
	Vector delta = (source - destination);
	float fHyp = FastSqrt((delta.x * delta.x) + (delta.y * delta.y));

	angles.x = (atanf(delta.z / fHyp) * M_RADPI);
	angles.y = (atanf(delta.y / delta.x) * M_RADPI);
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}

inline float CalcFov(const QAngle &src, const QAngle& dst)
{
	Vector v_src = Vector();
	AngleVectors(src, &v_src);

	Vector v_dst = Vector();
	AngleVectors(dst, &v_dst);

	float result = RAD2DEG(acos(v_dst.Dot(v_src) / v_dst.LengthSqr()));

	if (!isfinite(result) || isinf(result) || isnan(result))
		result = 0.0f;

	return result;
}

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