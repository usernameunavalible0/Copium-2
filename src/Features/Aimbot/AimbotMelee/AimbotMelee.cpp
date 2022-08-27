#include "AimbotMelee.h"
#include "../../Vars.h"

bool CAimbotMelee::CanMeleeHit(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, const QAngle& vecViewAngles, int nTargetIndex)
{
	static Vector vecSwingMins = { -18.0f, -18.0f, -18.0f };
	static Vector vecSwingMaxs = { 18.0f, 18.0f, 18.0f };

	float flRange = (pWeapon->GetSwingRange(pLocal));

	if (flRange <= 0.0f)
		return false;

	Vector vecForward = Vector();
	AngleVectors(vecViewAngles, &vecForward);

	Vector vecTraceStart = pLocal->Weapon_ShootPosition();
	Vector vecTraceEnd = (vecTraceStart + (vecForward * flRange));

	CTraceFilterSimpleList filter = NULL;
	filter.AddEntityToIgnore(pLocal);
	CGameTrace trace;
	UTIL_TraceHull(vecTraceStart, vecTraceEnd, vecSwingMins, vecSwingMaxs, MASK_SHOT, &filter, &trace);

	if (!(trace.m_pEnt && trace.m_pEnt->entindex() == nTargetIndex))
	{
		if (!Vars::Aimbot::Melee::PredictSwing.m_Var || pWeapon->GetWeaponID() == TF_WEAPON_KNIFE || pLocal->InCond(TF_COND_SHIELD_CHARGE))
			return false;

		static const float flDelay = 0.2f; //it just works

		if (pLocal->m_fFlags().IsFlagSet(FL_ONGROUND))
			vecTraceStart += (pLocal->m_vecVelocity() * flDelay);

		else vecTraceStart += (pLocal->m_vecVelocity() * flDelay) - (Vector(0.0f, 0.0f, G::ConVars.sv_gravity->GetFloat()) * 0.5f * flDelay * flDelay);

		Vector vecTraceEnd = vecTraceStart + (vecForward * flRange);
		UTIL_TraceHull(vecTraceStart, vecTraceEnd, vecSwingMins, vecSwingMaxs, MASK_SHOT, &filter, &trace);

		return (trace.m_pEnt && trace.m_pEnt->entindex() == nTargetIndex);
	}

	else return true;

	return false;
}

ESortMethod CAimbotMelee::GetSortMethod()
{
	switch (Vars::Aimbot::Melee::SortMethod.m_Var) {
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbotMelee::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	ESortMethod SortMethod = GetSortMethod();

	if (SortMethod == ESortMethod::FOV)
		g_Globals.m_flCurAimFOV = Vars::Aimbot::Melee::AimFOV.m_Var;

	g_AimbotGlobal.m_vecTargets.clear();

	Vector vLocalPos = pLocal->Weapon_ShootPosition();
	QAngle vLocalAngles; I::EngineClient->GetViewAngles(vLocalAngles);

	if (Vars::Aimbot::Global::AimPlayers.m_Var)
	{
		const bool bWhipTeam = (pWeapon->m_iItemDefinitionIndex() == Soldier_t_TheDisciplinaryAction && Vars::Aimbot::Melee::WhipTeam.m_Var);

		for (const auto& pPlayer : g_EntityCache.GetGroup(bWhipTeam ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES))
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

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Melee::AimFOV.m_Var)
				continue;

			g_AimbotGlobal.m_vecTargets.push_back({ Player, ETargetType::PLAYER, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	if (Vars::Aimbot::Global::AimBuildings.m_Var)
	{
		for (const auto& pBuilding : g_EntityCache.GetGroup(EEntGroup::BUILDINGS_ENEMIES))
		{
			C_BaseObject* Building = pBuilding->As<C_BaseObject*>();

			if (!Building->IsAlive())
				continue;

			Vector vPos = Building->WorldSpaceCenter();
			QAngle vAngleTo = CalcAngle(vLocalPos, vPos);
			float flFOVTo = SortMethod == ESortMethod::FOV ? CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Melee::AimFOV.m_Var)
				continue;

			g_AimbotGlobal.m_vecTargets.push_back({ Building, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	return !g_AimbotGlobal.m_vecTargets.empty();
}

bool CAimbotMelee::VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Target)
{
	if (Vars::Aimbot::Melee::RangeCheck.m_Var ? !CanMeleeHit(pLocal, pWeapon, Target.m_vAngleTo, Target.m_pEntity->entindex()) :
		!VisPos(pLocal, Target.m_pEntity, pLocal->Weapon_ShootPosition(), Target.m_vPos))
		return false;

	return true;
}

bool CAimbotMelee::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Out)
{
	if (!GetTargets(pLocal, pWeapon))
		return false;

	g_AimbotGlobal.SortTargets(GetSortMethod());

	for (auto& Target : g_AimbotGlobal.m_vecTargets)
	{
		if (!VerifyTarget(pLocal, pWeapon, Target))
			continue;

		Out = Target;
		return true;
	}

	return false;
}

void CAimbotMelee::Aim(CUserCmd* pCmd, QAngle& vAngle)
{
	vAngle -= m_vPunchAngles;
	ClampAngles(vAngle);

	switch (Vars::Aimbot::Melee::AimMethod.m_Var)
	{
	case 0: {
		pCmd->viewangles = vAngle;
		I::EngineClient->SetViewAngles(pCmd->viewangles);
		break;
	}

	case 1: {
		QAngle vecDelta = vAngle - pCmd->viewangles;
		ClampAngles(vecDelta);
		pCmd->viewangles += vecDelta / Vars::Aimbot::Melee::SmoothingAmount.m_Var;
		I::EngineClient->SetViewAngles(pCmd->viewangles);
		break;
	}

	case 2: {
		G::Util.FixMovement(vAngle, pCmd);
		pCmd->viewangles = vAngle;
		break;
	}

	default: break;
	}
}

bool CAimbotMelee::ShouldSwing(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, const Target_t& Target)
{
	if (!Vars::Aimbot::Global::AutoShoot.m_Var)
		return false;

	//There's a reason for running this even if range check is enabled (it calls this too), trust me :)
	QAngle bruh; I::EngineClient->GetViewAngles(bruh);
	if (!CanMeleeHit(pLocal, pWeapon, Vars::Aimbot::Melee::AimMethod.m_Var == 2 ? Target.m_vAngleTo : bruh, Target.m_pEntity->entindex()))
		return false;

	return true;
}

bool CAimbotMelee::IsAttacking(CUserCmd* pCmd, C_TFWeaponBase* pWeapon)
{
	if (pWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
		return ((pCmd->buttons & IN_ATTACK) && g_Globals.m_bWeaponCanAttack);

	else return fabs(pWeapon->m_nInspectStage() - I::GlobalVars->curtime) < I::GlobalVars->interval_per_tick * 2.0f;
}

void CAimbotMelee::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	if (!Vars::Aimbot::Melee::Active.m_Var || g_Globals.m_bAutoBackstabRunning)
		return;

	Target_t Target = { };

	bool bShouldAim = (Vars::Aimbot::Global::AimKey.m_Var == VK_LBUTTON ? (pCmd->buttons & IN_ATTACK) : g_AimbotGlobal.IsKeyDown());

	if (GetTarget(pLocal, pWeapon, Target) && bShouldAim)
	{
		g_Globals.m_nCurrentTargetIdx = Target.m_pEntity->entindex();

		//if (Vars::Aimbot::Melee::AimMethod.m_Var == 2)
			//g_GlobalInfo.m_vAimPos = Target.m_vPos;

		if (ShouldSwing(pLocal, pWeapon, pCmd, Target))
			pCmd->buttons |= IN_ATTACK;

		if (Vars::CL_Move::Enabled.m_Var && Vars::CL_Move::Doubletap.m_Var && (pCmd->buttons & IN_ATTACK) && !g_Globals.m_nShifted && !g_Globals.m_nWaitForShift)
		{
			g_Globals.m_bShouldShift = true;
		}

		bool bIsAttacking = IsAttacking(pCmd, pWeapon);

		if (bIsAttacking)
			g_Globals.m_bAttacking = true;

		if (Vars::Aimbot::Melee::AimMethod.m_Var == 2)
		{
			if (bIsAttacking) {
				Aim(pCmd, Target.m_vAngleTo);
				g_Globals.m_bSilentTime = true;
			}
		}

		else Aim(pCmd, Target.m_vAngleTo);
	}
}