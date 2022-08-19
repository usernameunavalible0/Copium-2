#include "AimbotHitscan.h"
#include "../../Vars.h"

int CAimbotHitscan::GetHitbox(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	switch (Vars::Aimbot::Hitscan::AimHitbox.m_Var)
	{
	case 0: { return HITBOX_HEAD; }
	case 1: { return HITBOX_PELVIS; }
	case 2:
	{
		int nClassNum = pLocal->m_iClass();

		if (nClassNum == TF_CLASS_SNIPER)
		{
			if (g_Globals.m_nCurItemDefIndex != Sniper_m_TheSydneySleeper)
				return (pLocal->IsZoomed() ? HITBOX_HEAD : HITBOX_PELVIS);

			return HITBOX_PELVIS;
		}

		else if (nClassNum == TF_CLASS_SPY)
		{
			bool bIsAmbassador = (g_Globals.m_nCurItemDefIndex == Spy_m_TheAmbassador || g_Globals.m_nCurItemDefIndex == Spy_m_FestiveAmbassador);
			return (bIsAmbassador ? HITBOX_HEAD : HITBOX_PELVIS);
		}

		else return HITBOX_PELVIS;
	}
	}

	return HITBOX_HEAD;
}

ESortMethod CAimbotHitscan::GetSortMethod()
{
	switch (Vars::Aimbot::Hitscan::SortMethod.m_Var)
	{
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbotHitscan::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	ESortMethod SortMethod = GetSortMethod();

	if (SortMethod == ESortMethod::FOV)
		g_Globals.m_flCurAimFOV = Vars::Aimbot::Hitscan::AimFOV.m_Var;

	g_AimbotGlobal.m_vecTargets.clear();

	Vector vLocalPos = pLocal->Weapon_ShootPosition();
	QAngle vLocalAngles;
	I::EngineClient->GetViewAngles(vLocalAngles);

	if (Vars::Aimbot::Global::AimPlayers.m_Var)
	{
		int nHitbox = GetHitbox(pLocal, pWeapon);

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
				//continue;

			Vector vPos; Player->GetHitboxPosition(nHitbox, vPos);
			QAngle vAngleTo = CalcAngle(vLocalPos, vPos);
			float flFOVTo = SortMethod == ESortMethod::FOV ? CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.m_Var)
				continue;

			g_AimbotGlobal.m_vecTargets.push_back({ Player, ETargetType::PLAYER, vPos, vAngleTo, flFOVTo, flDistTo, nHitbox});
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

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.m_Var)
				continue;

			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;

			g_AimbotGlobal.m_vecTargets.push_back({ Building, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	return !g_AimbotGlobal.m_vecTargets.empty();
}

bool CAimbotHitscan::ScanHitboxes(C_TFPlayer* pLocal, Target_t& Target)
{
	if (Target.m_TargetType == ETargetType::PLAYER)
	{
		if (!Vars::Aimbot::Hitscan::ScanHitboxes.m_Var)
			return false;
	}

	else if (Target.m_TargetType == ETargetType::BUILDING)
	{
		if (!Vars::Aimbot::Hitscan::ScanBuildings.m_Var)
			return false;
	}

	Vector vLocalPos = pLocal->Weapon_ShootPosition();

	for (int nHitbox = Target.m_TargetType == ETargetType::PLAYER ? 1 : 0; nHitbox < Target.m_pEntity->GetNumOfHitboxes(); nHitbox++)
	{
		if (Target.m_TargetType == ETargetType::PLAYER && nHitbox == HITBOX_PELVIS)
			continue;

		Vector vHitbox;
		Target.m_pEntity->GetHitboxPosition(nHitbox, vHitbox);

		if (VisPos(pLocal, Target.m_pEntity, vLocalPos, vHitbox)) {
			Target.m_vPos = vHitbox;
			Target.m_vAngleTo = CalcAngle(vLocalPos, vHitbox);
			return true;
		}
	}

	return false;
}

bool CAimbotHitscan::ScanHead(C_TFPlayer* pLocal, Target_t& Target)
{
	if (!Vars::Aimbot::Hitscan::ScanHead.m_Var)
		return false;

	const model_t* pModel = Target.m_pEntity->GetModel();
	if (!pModel)
		return false;

	studiohdr_t* pHDR = reinterpret_cast<studiohdr_t*>(I::ModelInfoClient->GetStudiomodel(pModel));
	if (!pHDR)
		return false;

	matrix3x4_t BoneMatrix[128];
	if (!Target.m_pEntity->SetupBones(BoneMatrix, 128, 0x100, I::GlobalVars->curtime))
		return false;

	mstudiohitboxset_t* pSet = pHDR->pHitboxSet(Target.m_pEntity->m_nHitboxSet());
	if (!pSet)
		return false;

	mstudiobbox_t* pBox = pSet->pHitbox(HITBOX_HEAD);
	if (!pBox)
		return false;

	Vector vLocalPos = pLocal->Weapon_ShootPosition();
	Vector vMins = pBox->bbmin;
	Vector vMaxs = pBox->bbmax;

	const float fScale = 0.8f;
	const std::vector<Vector> vecPoints = {
		Vector(((vMins.x + vMaxs.x) * 0.5f), (vMins.y * fScale), ((vMins.z + vMaxs.z) * 0.5f)),
		Vector((vMins.x * fScale), ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f)),
		Vector((vMaxs.x * fScale), ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f))
	};

	for (const auto& Point : vecPoints)
	{
		Vector vTransformed = {};
		VectorTransform(Point, BoneMatrix[pBox->bone], vTransformed);

		if (VisPosHitboxId(pLocal, Target.m_pEntity, vLocalPos, vTransformed, HITBOX_HEAD)) {
			Target.m_vPos = vTransformed;
			Target.m_vAngleTo = CalcAngle(vLocalPos, vTransformed);
			Target.m_bHasMultiPointed = true;
			return true;
		}
	}

	return false;
}

bool CAimbotHitscan::ScanBuildings(C_TFPlayer* pLocal, Target_t& Target)
{
	if (!Vars::Aimbot::Hitscan::ScanBuildings.m_Var)
		return false;

	Vector vLocalPos = pLocal->Weapon_ShootPosition();

	Vector vMins = Target.m_pEntity->m_vecMins();
	Vector vMaxs = Target.m_pEntity->m_vecMaxs();

	const std::vector<Vector> vecPoints = {
		Vector(vMins.x * 0.9f, ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f)),
		Vector(vMaxs.x * 0.9f, ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f)),
		Vector(((vMins.x + vMaxs.x) * 0.5f), vMins.y * 0.9f, ((vMins.z + vMaxs.z) * 0.5f)),
		Vector(((vMins.x + vMaxs.x) * 0.5f), vMaxs.y * 0.9f, ((vMins.z + vMaxs.z) * 0.5f)),
		Vector(((vMins.x + vMaxs.x) * 0.5f), ((vMins.y + vMaxs.y) * 0.5f), vMins.z * 0.9f),
		Vector(((vMins.x + vMaxs.x) * 0.5f), ((vMins.y + vMaxs.y) * 0.5f), vMaxs.z * 0.9f)
	};

	const matrix3x4_t& Transform = Target.m_pEntity->GetRgflCoordinateFrame();

	for (const auto& Point : vecPoints)
	{
		Vector vTransformed = {};
		VectorTransform(Point, Transform, vTransformed);

		if (VisPos(pLocal, Target.m_pEntity, vLocalPos, vTransformed)) {
			Target.m_vPos = vTransformed;
			Target.m_vAngleTo = CalcAngle(vLocalPos, vTransformed);
			return true;
		}
	}

	return false;
}

bool CAimbotHitscan::VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Target)
{
	switch (Target.m_TargetType)
	{
	case ETargetType::PLAYER:
	{
		if (Target.m_nAimedHitbox == HITBOX_HEAD)
		{
			int nHit = -1;

			if (!VisPosHitboxIdOut(pLocal, Target.m_pEntity, pLocal->Weapon_ShootPosition(), Target.m_vPos, nHit))
				return false;

			if (nHit != HITBOX_HEAD && !ScanHead(pLocal, Target))
				return false;
		}

		else if (Target.m_nAimedHitbox == HITBOX_PELVIS)
		{
			if (!VisPos(pLocal, Target.m_pEntity, pLocal->Weapon_ShootPosition(), Target.m_vPos) && !ScanHitboxes(pLocal, Target))
				return false;
		}

		break;
	}

	case ETargetType::BUILDING:
	{
		if (!VisPos(pLocal, Target.m_pEntity, pLocal->Weapon_ShootPosition(), Target.m_vPos))
		{
			//Sentryguns have hitboxes, it's better to use ScanHitboxes for them
			if (Target.m_pEntity->GetClientClass()->GetTFClientClass() == ETFClientClass::CObjectSentrygun ? !ScanHitboxes(pLocal, Target) : !ScanBuildings(pLocal, Target))
				return false;
		}

		break;
	}

	default:
	{
		if (!VisPos(pLocal, Target.m_pEntity, pLocal->Weapon_ShootPosition(), Target.m_vPos))
			return false;

		break;
	}
	}

	return true;
}

bool CAimbotHitscan::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, Target_t& Out)
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

void CAimbotHitscan::Aim(CUserCmd* pCmd, QAngle& vAngle)
{
	vAngle -= m_vPunchAngles;
	ClampAngles(vAngle);

	int nAimMethod = /*(Vars::Aimbot::Hitscan::SpectatedSmooth.m_Var && g_GlobalInfo.m_bLocalSpectated) ? 1 :*/ Vars::Aimbot::Hitscan::AimMethod.m_Var;

	switch (nAimMethod)
	{
	case 0: //Plain
	{
		pCmd->viewangles = vAngle;
		I::EngineClient->SetViewAngles(pCmd->viewangles);
		break;
	}

	case 1: //Smooth
	{
		//Calculate delta of current viewangles and wanted angles
		QAngle bruh;
		I::EngineClient->GetViewAngles(bruh);
		QAngle vecDelta = vAngle - bruh;

		//Clamp, keep the angle in possible bounds
		ClampAngles(vecDelta);

		//Basic smooth by dividing the delta by wanted smooth amount
		pCmd->viewangles += vecDelta / Vars::Aimbot::Hitscan::SmoothingAmount.m_Var;

		//Set the viewangles from engine
		I::EngineClient->SetViewAngles(pCmd->viewangles);
		break;
	}

	case 2: //Silent
	{
		G::Util.FixMovement(vAngle, pCmd);
		pCmd->viewangles = vAngle;
		break;
	}

	default: break;
	}
}

bool CAimbotHitscan::ShouldFire(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, const Target_t& Target)
{
	if (!Vars::Aimbot::Global::AutoShoot.m_Var)
		return false;

	switch (g_Globals.m_nCurItemDefIndex)
	{
	case Sniper_m_TheMachina:
	case Sniper_m_ShootingStar:
	{
		if (!pLocal->IsZoomed())
			return false;

		break;
	}
	default: break;
	}

	switch (pLocal->m_iClass())
	{
	case TF_CLASS_SNIPER:
	{
		bool bIsScoped = pLocal->IsZoomed();

		if (Vars::Aimbot::Hitscan::WaitForHeadshot.m_Var)
		{
			if (g_Globals.m_nCurItemDefIndex != Sniper_m_TheClassic
				&& g_Globals.m_nCurItemDefIndex != Sniper_m_TheSydneySleeper
				&& !g_Globals.m_bWeaponCanHeadShot && bIsScoped)
				return false;
		}

		if (Vars::Aimbot::Hitscan::WaitForCharge.m_Var && bIsScoped)
		{
			int nHealth = Target.m_pEntity->GetHealth();
			bool bIsCritBoosted = pLocal->IsCritBoostedNoMini();

			if (Target.m_nAimedHitbox == HITBOX_HEAD && g_Globals.m_nCurItemDefIndex != Sniper_m_TheSydneySleeper)
			{
				if (nHealth > 150)
				{
					float flDamage = RemapValClamped(pWeapon->m_flChargedDamage(), 0.0f, 150.0f, 0.0f, 450.0f);
					int nDamage = static_cast<int>(flDamage);

					if (nDamage < nHealth && nDamage != 450)
						return false;
				}

				else
				{
					if (!bIsCritBoosted && !g_Globals.m_bWeaponCanHeadShot)
						return false;
				}
			}

			else
			{
				if (nHealth > (bIsCritBoosted ? 150 : 50))
				{
					float flMult = Target.m_pEntity->As<C_TFPlayer*>()->IsInJarate() ? 1.36f : 1.0f;

					if (bIsCritBoosted)
						flMult = 3.0f;

					float flMax = 150.0f * flMult;
					int nDamage = static_cast<int>(pWeapon->m_flChargedDamage() * flMult);

					if (nDamage < Target.m_pEntity->GetHealth() && nDamage != static_cast<int>(flMax))
						return false;
				}
			}
		}

		break;
	}

	case TF_CLASS_SPY:
	{
		if (Vars::Aimbot::Hitscan::WaitForHeadshot.m_Var && !g_Globals.m_bWeaponCanHeadShot)
		{
			if (g_Globals.m_nCurItemDefIndex == Spy_m_TheAmbassador || g_Globals.m_nCurItemDefIndex == Spy_m_FestiveAmbassador)
				return false;
		}

		break;
	}

	default: break;
	}

	int nAimMethod = /*(Vars::Aimbot::Hitscan::SpectatedSmooth.m_Var && g_GlobalInfo.m_bLocalSpectated) ? 1 :*/ Vars::Aimbot::Hitscan::AimMethod.m_Var;

	if (nAimMethod == 1)
	{
		Vector vForward = { };
		AngleVectors(pCmd->viewangles, &vForward);
		Vector vTraceStart = pLocal->Weapon_ShootPosition();
		Vector vTraceEnd = (vTraceStart + (vForward * 8192.0f));

		CGameTrace trace = { };
		CTraceFilterSimpleList filter = NULL;
		filter.AddEntityToIgnore(pLocal);

		UTIL_TraceLine(vTraceStart, vTraceEnd, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

		if (trace.m_pEnt != Target.m_pEntity)
			return false;

		if (Target.m_nAimedHitbox == HITBOX_HEAD)
		{
			if (trace.hitbox != HITBOX_HEAD)
				return false;

			if (!Target.m_bHasMultiPointed)
			{
				Vector vMins = {}, vMaxs = {}, vCenter = {};
				matrix3x4_t Matrix = {};

				if (!Target.m_pEntity->GetHitboxMinsAndMaxsAndMatrix(HITBOX_HEAD, vMins, vMaxs, Matrix, &vCenter))
					return false;

				vMins *= 0.5f;
				vMaxs *= 0.5f;

				if (!RayToOBB(vTraceStart, vForward, vCenter, vMins, vMaxs, Matrix))
					return false;
			}
		}
	}

	return true;
}

bool CAimbotHitscan::IsAttacking(CUserCmd* pCmd, C_TFWeaponBase* pWeapon)
{
	return ((pCmd->buttons & IN_ATTACK) && g_Globals.m_bWeaponCanAttack);
}

void CAimbotHitscan::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	if (!Vars::Aimbot::Hitscan::Active.m_Var)
		return;

	Target_t Target = { };

	const int nWeaponID = pWeapon->GetWeaponID();
	const bool bShouldAim = (Vars::Aimbot::Global::AimKey.m_Var == VK_LBUTTON ? (pCmd->buttons & IN_ATTACK) : g_AimbotGlobal.IsKeyDown());

	//Rev minigun if enabled and aimbot active
	if (bShouldAim) {
		if (Vars::Aimbot::Hitscan::AutoRev.m_Var && nWeaponID == TF_WEAPON_MINIGUN)
			pCmd->buttons |= IN_ATTACK2;
	}

	if (GetTarget(pLocal, pWeapon, Target) && bShouldAim)
	{
		if (nWeaponID != TF_WEAPON_COMPOUND_BOW
			&& pLocal->m_iClass() == TF_CLASS_SNIPER
			&& pWeapon->GetSlot() == 0)
		{
			bool bScoped = pLocal->IsZoomed();

			if (Vars::Aimbot::Hitscan::AutoScope.m_Var && !bScoped) {
				pCmd->buttons |= IN_ATTACK2;
				return;
			}

			if (Vars::Aimbot::Hitscan::ScopedOnly.m_Var && !bScoped)
				return;
		}

		g_Globals.m_nCurrentTargetIdx = Target.m_pEntity->entindex();
		g_Globals.m_bHitscanRunning = true;
		g_Globals.m_bHitscanSilentActive = Vars::Aimbot::Hitscan::AimMethod.m_Var == 2;

		if (Vars::Aimbot::Hitscan::SpectatedSmooth.m_Var && g_Globals.m_bLocalSpectated)
			g_Globals.m_bHitscanSilentActive = false;

		//if (g_Globals.m_bHitscanSilentActive)
		//	m_vAimPos = Target.m_vPos;

		Aim(pCmd, Target.m_vAngleTo);

		if (ShouldFire(pLocal, pWeapon, pCmd, Target))
		{
			if (nWeaponID == TF_WEAPON_MINIGUN)
				pCmd->buttons |= IN_ATTACK2;

			pCmd->buttons |= IN_ATTACK;

			bool bIsAttacking = IsAttacking(pCmd, pWeapon);

			if (bIsAttacking)
				g_Globals.m_bAttacking = true;
		}
	}
}