#include "Aimbot.h"
#include "../Vars.h"

#include "AimbotHitscan/AimbotHitscan.h"
#include "AimbotProjectile/AimbotProjectile.h"
#include "AimbotMelee/AimbotMelee.h"

bool CAimbot::ShouldRun(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	if (!Vars::Aimbot::Global::Active.m_Var)
		return false;

	if (I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible())
		return false;

	if (!pLocal->IsAlive()
		|| pLocal->IsTaunting()
		|| pLocal->InCond(TF_COND_PHASE)
		|| pLocal->m_bFeignDeathReady()
		|| pLocal->IsCloaked()
		|| pLocal->InCond(TF_COND_HALLOWEEN_KART)
		|| pLocal->InCond(TF_COND_HALLOWEEN_GHOST_MODE))
		return false;

	switch (g_Globals.m_nCurItemDefIndex) {
	case Soldier_m_RocketJumper:
	case Demoman_s_StickyJumper: return false;
	default: break;
	}

	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_PDA:
	case TF_WEAPON_PDA_ENGINEER_BUILD:
	case TF_WEAPON_PDA_ENGINEER_DESTROY:
	case TF_WEAPON_PDA_SPY:
	case TF_WEAPON_PDA_SPY_BUILD:
	case TF_WEAPON_BUILDER:
	case TF_WEAPON_INVIS:
	case TF_WEAPON_LUNCHBOX:
	case TF_WEAPON_BUFF_ITEM:
	case TF_WEAPON_GRAPPLINGHOOK:
	case TF_WEAPON_MEDIGUN: {
		return false;
	}

	default: break;
	}

	return true;
}

void CAimbot::Run(CUserCmd* pCmd)
{
	g_Globals.m_nCurrentTargetIdx = 0;
	g_Globals.m_flCurAimFOV = 0.0f;
	g_Globals.m_bHitscanRunning = false;
	g_Globals.m_bHitscanSilentActive = false;

	auto pLocal = g_EntityCache.GetLocal();
	auto pWeapon = g_EntityCache.GetWeapon();

	if (pLocal && pWeapon)
	{
		if (!ShouldRun(pLocal, pWeapon))
			return;

		//g_AimbotHitscan.Run(pLocal, pWeapon, pCmd);

		
		switch (g_Globals.m_WeaponType)
		{
		case EWeaponType::HITSCAN: {
			g_AimbotHitscan.Run(pLocal, pWeapon, pCmd);
			break;
		}

		case EWeaponType::PROJECTILE: {
			g_AimbotProjectile.Run(pLocal, pWeapon, pCmd);
			break;
		}

		case EWeaponType::MELEE: {
			g_AimbotMelee.Run(pLocal, pWeapon, pCmd);
			break;
		}

		default: break;
		}
	}
}