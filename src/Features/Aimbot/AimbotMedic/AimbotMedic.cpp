#include "AimbotMedic.h"
#include "../../Vars.h"

ESortMethod CAimbotMedic::GetSortMethod()
{
	switch (Vars::Aimbot::Medic::SortMethod.m_Var) {
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbotMedic::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	ESortMethod SortMethod = GetSortMethod();

	if (SortMethod == ESortMethod::FOV)
		g_Globals.m_flCurAimFOV = Vars::Aimbot::Medic::AimFOV.m_Var;

	g_AimbotGlobal.m_vecTargets.clear();

	Vector vLocalPos = pLocal->Weapon_ShootPosition();
	QAngle vLocalAngles; I::EngineClient->GetViewAngles(vLocalAngles);

	if (Vars::Aimbot::Global::AimPlayers.m_Var)
	{
		//const bool bWhipTeam = (pWeapon->m_iItemDefinitionIndex() == Soldier_t_TheDisciplinaryAction && Vars::Aimbot::Melee::WhipTeam.m_Var);

		for (const auto& pPlayer : g_EntityCache.GetGroup(EEntGroup::PLAYERS_ENEMIES))
		{
			C_TFPlayer* Player = pPlayer->As<C_TFPlayer*>();
			if (!Player->IsAlive() || Player->InCond(TF_COND_HALLOWEEN_GHOST_MODE))
				continue;

			if (Vars::Aimbot::Global::IgnoreInvlunerable.m_Var && Player->IsInvulnerable())
				continue;

			if (Vars::Aimbot::Global::IgnoreCloaked.m_Var && Player->IsCloaked())
				continue;

			if (Vars::Aimbot::Global::IgnoreTaunting.m_Var && Player->IsTaunting())
				continue;

			//if (Vars::Aimbot::Global::IgnoreFriends.m_Var && g_EntityCache.Friends[Player->GetIndex()])
			//	continue;

			Vector vPos; Player->GetHitboxPosition(HITBOX_BODY, vPos);
			QAngle vAngleTo = CalcAngle(vLocalPos, vPos);
			float flFOVTo = SortMethod == ESortMethod::FOV ? CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Medic::AimFOV.m_Var)
				continue;

			g_AimbotGlobal.m_vecTargets.push_back({ Player, ETargetType::PLAYER, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	return !g_AimbotGlobal.m_vecTargets.empty();
}

bool CAimbotMedic::VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Target)
{
	//if (Vars::Aimbot::Melee::RangeCheck.m_Var ? !CanMeleeHit(pLocal, pWeapon, Target.m_vAngleTo, Target.m_pEntity->entindex()) :
		if (!VisPos(pLocal, Target.m_pEntity, pLocal->Weapon_ShootPosition(), Target.m_vPos))
		return false;

	return true;
}