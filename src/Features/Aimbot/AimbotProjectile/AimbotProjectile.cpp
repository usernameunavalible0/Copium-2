#include "AimbotProjectile.h"
#include "../../Vars.h"

Vector CAimbotProjectile::Predictor_t::Extrapolate(float time)
{
	Vector vecOut = {};

	if (m_pEntity->As<C_TFPlayer*>()->m_fFlags().IsFlagSet(FL_ONGROUND))
		vecOut = (m_vPosition + (m_vVelocity * time));

	else vecOut = (m_vPosition + (m_vVelocity * time) - m_vAcceleration * 0.5f * time * time);

	return vecOut;
}

bool CAimbotProjectile::GetProjectileInfo(C_TFWeaponBase* pWeapon, ProjectileInfo_t& out)
{
	switch (g_Globals.m_nCurItemDefIndex)
	{
	case Soldier_m_RocketLauncher:
	case Soldier_m_RocketLauncherR:
	case Soldier_m_TheBlackBox:
	case Soldier_m_TheCowMangler5000:
	case Soldier_m_TheOriginal:
	case Soldier_m_FestiveRocketLauncher:
	case Soldier_m_TheBeggarsBazooka:
	case Soldier_m_SilverBotkillerRocketLauncherMkI:
	case Soldier_m_GoldBotkillerRocketLauncherMkI:
	case Soldier_m_RustBotkillerRocketLauncherMkI:
	case Soldier_m_BloodBotkillerRocketLauncherMkI:
	case Soldier_m_CarbonadoBotkillerRocketLauncherMkI:
	case Soldier_m_DiamondBotkillerRocketLauncherMkI:
	case Soldier_m_SilverBotkillerRocketLauncherMkII:
	case Soldier_m_GoldBotkillerRocketLauncherMkII:
	case Soldier_m_FestiveBlackBox:
	case Soldier_m_TheAirStrike:
	case Soldier_m_WoodlandWarrior:
	case Soldier_m_SandCannon:
	case Soldier_m_AmericanPastoral:
	case Soldier_m_SmalltownBringdown:
	case Soldier_m_ShellShocker:
	case Soldier_m_AquaMarine:
	case Soldier_m_Autumn:
	case Soldier_m_BlueMew:
	case Soldier_m_BrainCandy:
	case Soldier_m_CoffinNail:
	case Soldier_m_HighRollers:
	case Soldier_m_Warhawk: {
		out = { 1100.0f, 0.0f };
		break;
	}

	case Soldier_m_TheDirectHit: {
		out = { 1980.0f, 0.0f };
		break;
	}

	case Soldier_m_TheLibertyLauncher: {
		out = { 1540.0f, 0.0f };
		break;
	}

	case Demoman_m_GrenadeLauncher:
	case Demoman_m_GrenadeLauncherR:
	case Demoman_m_FestiveGrenadeLauncher:
	case Demoman_m_TheIronBomber:
	case Demoman_m_Autumn:
	case Demoman_m_MacabreWeb:
	case Demoman_m_Rainbow:
	case Demoman_m_SweetDreams:
	case Demoman_m_CoffinNail:
	case Demoman_m_TopShelf:
	case Demoman_m_Warhawk:
	case Demoman_m_ButcherBird: {
		out = { 1200.0f, 0.4f };
		break;
	}

	case Soldier_s_TheRighteousBison:
	case Engi_m_ThePomson6000: {
		out = { 1200.0f, 0.0f };
		break;
	}

	case Demoman_m_TheLooseCannon: {
		out = { 1453.9f, 0.4f };
		break;
	}

	case Demoman_m_TheLochnLoad: {
		out = { 1513.3f, 0.4f };
		break;
	}

	case Engi_m_TheRescueRanger:
	case Medic_m_FestiveCrusadersCrossbow:
	case Medic_m_CrusadersCrossbow: {
		out = { 2400.0f, 0.2f };
		break;
	}

	case Pyro_m_DragonsFury: {
		out = { 3000.0f, 0.0f, 0.12f };
		break;
	}

	case Pyro_m_FlameThrower:
	case Pyro_m_FlameThrowerR:
	case Pyro_m_TheBackburner:
	case Pyro_m_TheDegreaser:
	case Pyro_m_ThePhlogistinator:
	case Pyro_m_FestiveFlameThrower:
	case Pyro_m_TheRainblower:
	case Pyro_m_SilverBotkillerFlameThrowerMkI:
	case Pyro_m_GoldBotkillerFlameThrowerMkI:
	case Pyro_m_RustBotkillerFlameThrowerMkI:
	case Pyro_m_BloodBotkillerFlameThrowerMkI:
	case Pyro_m_CarbonadoBotkillerFlameThrowerMkI:
	case Pyro_m_DiamondBotkillerFlameThrowerMkI:
	case Pyro_m_SilverBotkillerFlameThrowerMkII:
	case Pyro_m_GoldBotkillerFlameThrowerMkII:
	case Pyro_m_FestiveBackburner:
	case Pyro_m_ForestFire:
	case Pyro_m_BarnBurner:
	case Pyro_m_BovineBlazemaker:
	case Pyro_m_EarthSkyandFire:
	case Pyro_m_FlashFryer:
	case Pyro_m_TurbineTorcher:
	case Pyro_m_Autumn:
	case Pyro_m_PumpkinPatch:
	case Pyro_m_Nutcracker:
	case Pyro_m_Balloonicorn:
	case Pyro_m_Rainbow:
	case Pyro_m_CoffinNail:
	case Pyro_m_Warhawk:
	case Pyro_m_NostromoNapalmer: {
		out = { 1000.0f, 0.0f, 0.33f };
		m_bIsFlameThrower = true;
		break;
	}

	case Pyro_s_TheDetonator:
	case Pyro_s_TheFlareGun:
	case Pyro_s_FestiveFlareGun:
	case Pyro_s_TheScorchShot: {
		out = { 2000.0f, 0.3f };
		break;
	}

	case Pyro_s_TheManmelter: {
		out = { 3000.0f, 0.2f };
		break;
	}

	case Medic_m_SyringeGun:
	case Medic_m_SyringeGunR:
	case Medic_m_TheBlutsauger:
	case Medic_m_TheOverdose: {
		out = { 1000.0f, 0.2f };
		break;
	}

	case Sniper_m_TheHuntsman:
	case Sniper_m_FestiveHuntsman:
	case Sniper_m_TheFortifiedCompound: {
		float charge = (I::GlobalVars->curtime - pWeapon->m_flChargeBeginTime());
		out = { ((fminf(fmaxf(charge, 0.0f), 1.0f) * 800.0f) + 1800.0f), ((fminf(fmaxf(charge, 0.0f), 1.0f) * -0.4f) + 0.5f) };
		break;
	}
	}

	return out.m_flVelocity;
}

bool CAimbotProjectile::CalcProjAngle(const Vector& vLocalPos, const Vector& vTargetPos, const ProjectileInfo_t& ProjInfo, Solution_t& out)
{
	const Vector v = vTargetPos - vLocalPos;
	const float dx = sqrt(v.x * v.x + v.y * v.y);
	const float dy = v.z;
	const float v0 = ProjInfo.m_flVelocity;

	//Ballistics
	if (const float g = G::ConVars.sv_gravity->GetFloat() * ProjInfo.m_flGravity)
	{
		const float root = v0 * v0 * v0 * v0 - g * (g * dx * dx + 2.0f * dy * v0 * v0);

		if (root < 0.0f)
			return false;

		out.m_flPitch = atan((v0 * v0 - sqrt(root)) / (g * dx));
		out.m_flYaw = atan2(v.y, v.x);
	}

	//Straight trajectory (the time stuff later doesn't make sense with this but hey it works!)
	else
	{
		QAngle vecAngle = CalcAngle(vLocalPos, vTargetPos);
		out.m_flPitch = -DEG2RAD(vecAngle.x);
		out.m_flYaw = DEG2RAD(vecAngle.y);
	}

	out.m_flTime = dx / (cos(out.m_flPitch) * v0);

	return true;
}

bool CAimbotProjectile::SolveProjectile(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, Predictor_t& Predictor, const ProjectileInfo_t& ProjInfo, Solution_t& out)
{
	INetChannelInfo* pNetChannel = reinterpret_cast<INetChannelInfo*>(I::EngineClient->GetNetChannelInfo());

	if (!pNetChannel)
		return false;

	Ray_t Ray = {};
	CTraceFilterSimpleList TraceFilter = NULL;
	CGameTrace Trace = {};

	TraceFilter.AddEntityToIgnore(Predictor.m_pEntity);

	Vector vLocalPos = pLocal->EyePosition();

	float fInterp = G::ConVars.cl_interp->GetFloat();
	float fLatency = (pNetChannel->GetLatency(FLOW_OUTGOING) + pNetChannel->GetLatency(FLOW_INCOMING));

	float MAX_TIME = ProjInfo.m_flMaxTime;
	float TIME_STEP = (MAX_TIME / 128.0f);

	for (float fPredTime = 0.0f; fPredTime < MAX_TIME; fPredTime += TIME_STEP)
	{
		float fCorrectTime = (fPredTime + fInterp + fLatency);
		Vector vPredictedPos = Predictor.Extrapolate(fCorrectTime);

		switch (pWeapon->GetWeaponID())
		{
		case TF_WEAPON_GRENADELAUNCHER:
		case TF_WEAPON_PIPEBOMBLAUNCHER:
		{
			Vector vDelta = (vPredictedPos - vLocalPos);
			float fRange = VectorNormalize(vDelta);

			float fElevationAngle = (fRange * (g_Globals.m_nCurItemDefIndex == Demoman_m_TheLochnLoad ? 0.0075f : 0.013f));

			if (fElevationAngle > 45.0f)
				fElevationAngle = 45.0f;

			float s = 0.0f, c = 0.0f;
			SinCos((fElevationAngle * PI / 180.0f), &s, &c);

			float fElevation = (fRange * (s / c));
			vPredictedPos.z += (c > 0.0f ? fElevation : 0.0f);
			break;
		}

		default: break;
		}

		UTIL_TraceHull(Predictor.m_vPosition, vPredictedPos, Vector(-2, -2, -2), Vector(2, 2, 2), MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);

		if (Trace.DidHit())
			vPredictedPos.z = Trace.endpos.z;

		switch (pWeapon->GetWeaponID())
		{
		case TF_WEAPON_GRENADELAUNCHER:
		case TF_WEAPON_PIPEBOMBLAUNCHER:
		{
			Vector vecOffset(16.0f, 8.0f, -6.0f);
			GetProjectileFireSetup(pLocal, pCmd->viewangles, vecOffset, &vLocalPos);
			break;
		}

		default: break;
		}

		if (!CalcProjAngle(vLocalPos, vPredictedPos, ProjInfo, out))
			return false;

		if (out.m_flTime < fCorrectTime)
		{
			Vector vVisCheck = vLocalPos;

			switch (pWeapon->GetWeaponID())
			{
			case TF_WEAPON_ROCKETLAUNCHER:
			case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
			case TF_WEAPON_FLAREGUN:
			case TF_WEAPON_FLAREGUN_REVENGE:
			case TF_WEAPON_COMPOUND_BOW:
			case TF_WEAPON_SYRINGEGUN_MEDIC:
			{
				if (g_Globals.m_nCurItemDefIndex != Soldier_m_TheOriginal)
				{
					Vector vecOffset(23.5f, 12.0f, -3.0f);

					if (pLocal->IsDucking())
						vecOffset.z = 8.0f;

					GetProjectileFireSetup(pLocal, pCmd->viewangles, vecOffset, &vVisCheck);
				}

				break;
			}

			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				QAngle vecAngle = QAngle(); Vector vecForward = Vector(), vecRight = Vector(), vecUp = Vector();
				AngleVectors({ -RAD2DEG(out.m_flPitch), RAD2DEG(out.m_flYaw), 0.0f }, &vecForward, &vecRight, &vecUp);
				Vector vecVelocity = ((vecForward * ProjInfo.m_flVelocity) - (vecUp * 200.0f));
				VectorAngles(vecVelocity, vecAngle);
				out.m_flPitch = -DEG2RAD(vecAngle.x);

				break;
			}

			default: break;
			}

			UTIL_TraceHull(vVisCheck, vPredictedPos, Vector(-2, -2, -2), Vector(2, 2, 2), MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);

			if (Trace.DidHit())
				return false;

			m_vPredictedPos = vPredictedPos;
			return true;
		}
	}

	return false;
}

Vector CAimbotProjectile::GetAimPos(C_TFPlayer* pLocal, C_BaseAnimating* pEntity)
{
	switch (Vars::Aimbot::Projectile::AimPosition.m_Var)
	{
	case 0: return pEntity->WorldSpaceCenter();
	case 1: return pEntity->WorldSpaceCenter() - Vector(0.0f, 0.0f, 27.0f);
	case 2:
	{
		switch (pLocal->m_iClass())
		{
		case TF_CLASS_SOLDIER: return pEntity->WorldSpaceCenter() - Vector(0.0f, 0.0f, 27.0f);

		case TF_CLASS_SNIPER:
		{
			Vector vPos; pEntity->GetHitboxPosition(HITBOX_HEAD, vPos);

			Vector vEntForward = {};
			AngleVectors(pEntity->EyeAngles(), &vEntForward);
			Vector vToEnt = pEntity->GetAbsOrigin() - pLocal->GetAbsOrigin();
			vToEnt.NormalizeInPlace();

			if (vToEnt.Dot(vEntForward) > 0.1071f)
				vPos.z += 5.0f;

			return vPos;
		}

		default: return pEntity->WorldSpaceCenter();
		}
	}
	default: return Vector();
	}
}

ESortMethod CAimbotProjectile::GetSortMethod()
{
	switch (Vars::Aimbot::Projectile::SortMethod.m_Var) {
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbotProjectile::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	ESortMethod SortMethod = GetSortMethod();

	if (SortMethod == ESortMethod::FOV)
		g_Globals.m_flCurAimFOV = Vars::Aimbot::Projectile::AimFOV.m_Var;

	g_AimbotGlobal.m_vecTargets.clear();

	Vector vLocalPos = pLocal->Weapon_ShootPosition();
	QAngle vLocalAngles; I::EngineClient->GetViewAngles(vLocalAngles);

	if (Vars::Aimbot::Global::AimPlayers.m_Var)
	{
		int nWeaponID = pWeapon->GetWeaponID();
		bool bIsCrossbow = nWeaponID == TF_WEAPON_CROSSBOW;

		for (const auto& pPlayer : g_EntityCache.GetGroup(bIsCrossbow ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES))
		{
			const auto& Player = pPlayer->As<C_TFPlayer*>();

			if (!Player->IsAlive() || Player->InCond(TF_COND_HALLOWEEN_GHOST_MODE) || Player == pLocal)
				continue;

			if (Player->GetTeamNumber() != pLocal->GetTeamNumber())
			{
				if (Vars::Aimbot::Global::IgnoreInvlunerable.m_Var && Player->IsInvulnerable())
					continue;

				if (Vars::Aimbot::Global::IgnoreCloaked.m_Var && Player->IsCloaked())
					continue;

				if (Vars::Aimbot::Global::IgnoreTaunting.m_Var && Player->IsTaunting())
					continue;

				//if (Vars::Aimbot::Global::IgnoreFriends.m_Var && g_EntityCache.Friends[Player->GetIndex()])
				//	continue;
			}

			Vector vPos = GetAimPos(pLocal, Player);
			QAngle vAngleTo = CalcAngle(vLocalPos, vPos);
			float flFOVTo = (SortMethod == ESortMethod::FOV) ? CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			float flDistTo = (SortMethod == ESortMethod::DISTANCE) ? vLocalPos.DistTo(vPos) : 0.0f;

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Projectile::AimFOV.m_Var)
				continue;

			g_AimbotGlobal.m_vecTargets.push_back({ Player, ETargetType::PLAYER, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	if (Vars::Aimbot::Global::AimBuildings.m_Var)
	{
		bool bIsRescueRanger = pWeapon->GetWeaponID() == TF_WEAPON_SHOTGUN_BUILDING_RESCUE;

		for (const auto& pBuilding : g_EntityCache.GetGroup(bIsRescueRanger ? EEntGroup::BUILDINGS_ALL : EEntGroup::BUILDINGS_ENEMIES))
		{
			const auto& Building = pBuilding->As<C_BaseObject*>();
			if (!Building->IsAlive())
				continue;

			Vector vPos = Building->WorldSpaceCenter();
			QAngle vAngleTo = CalcAngle(vLocalPos, vPos);
			float flFOVTo = SortMethod == ESortMethod::FOV ? CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Projectile::AimFOV.m_Var)
				continue;

			g_AimbotGlobal.m_vecTargets.push_back({ Building, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	return !g_AimbotGlobal.m_vecTargets.empty();
}

bool CAimbotProjectile::VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, Target_t& Target)
{
	ProjectileInfo_t ProjInfo = {};

	if (!GetProjectileInfo(pWeapon, ProjInfo))
		return false;

	Vector vVelocity = Vector();
	Vector vAcceleration = Vector();

	switch (Target.m_TargetType)
	{
	case ETargetType::PLAYER: {
		vVelocity = Target.m_pEntity->As<C_TFPlayer*>()->m_vecVelocity();
		vAcceleration = Vector(0.0f, 0.0f, G::ConVars.sv_gravity->GetFloat() * ((Target.m_pEntity->As<C_TFPlayer*>()->InCond(TF_COND_PARACHUTE_ACTIVE)) ? 0.224f : 1.0f));
		break;
	}

	default: break;
	}

	Predictor_t Predictor = {
		Target.m_pEntity,
		Target.m_vPos,
		vVelocity,
		vAcceleration
	};

	Solution_t Solution = {};

	if (!SolveProjectile(pLocal, pWeapon, pCmd, Predictor, ProjInfo, Solution))
		return false;

	Target.m_vAngleTo = { -RAD2DEG(Solution.m_flPitch), RAD2DEG(Solution.m_flYaw), 0.0f };

	return true;
}

bool CAimbotProjectile::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd, Target_t& Out)
{
	if (!GetTargets(pLocal, pWeapon))
		return false;

	if (Vars::Aimbot::Projectile::PerformanceMode.m_Var)
	{
		Target_t Target = g_AimbotGlobal.GetBestTarget(GetSortMethod());

		if (!VerifyTarget(pLocal, pWeapon, pCmd, Target))
			return false;

		Out = Target;
		return true;
	}

	else
	{
		g_AimbotGlobal.SortTargets(GetSortMethod());

		for (auto& Target : g_AimbotGlobal.m_vecTargets)
		{
			if (!VerifyTarget(pLocal, pWeapon, pCmd, Target))
				continue;

			Out = Target;
			return true;
		}
	}

	return false;
}

void CAimbotProjectile::Aim(CUserCmd* pCmd, C_TFWeaponBase* pWeapon, QAngle& vAngle)
{
	vAngle -= m_vPunchAngles;
	ClampAngles(vAngle);

	switch (Vars::Aimbot::Projectile::AimMethod.m_Var)
	{
	case 0: {
		pCmd->viewangles = vAngle;
		I::EngineClient->SetViewAngles(pCmd->viewangles);
		break;
	}

	case 1: {
		G::Util.FixMovement(vAngle, pCmd);
		pCmd->viewangles = vAngle;
		break;
	}

	default: break;
	}
}

bool CAimbotProjectile::ShouldFire(CUserCmd* pCmd)
{
	return (Vars::Aimbot::Global::AutoShoot.m_Var && g_Globals.m_bWeaponCanAttack);
}

bool CAimbotProjectile::IsAttacking(CUserCmd* pCmd, C_TFWeaponBase* pWeapon)
{
	if (g_Globals.m_nCurItemDefIndex == Soldier_m_TheBeggarsBazooka)
	{
		static bool bLoading = false;

		if (pWeapon->Clip1() > 0)
			bLoading = true;

		if (!(pCmd->buttons & IN_ATTACK) && bLoading) {
			bLoading = false;
			return true;
		}
	}

	else
	{
		if (pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW)
		{
			static bool bCharging = false;

			if (pWeapon->m_flChargeBeginTime() > 0.0f)
				bCharging = true;

			if (!(pCmd->buttons & IN_ATTACK) && bCharging) {
				bCharging = false;
				return true;
			}
		}

		else
		{
			if ((pCmd->buttons & IN_ATTACK) && g_Globals.m_bWeaponCanAttack)
				return true;
		}
	}

	return false;
}

void CAimbotProjectile::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	m_bIsFlameThrower = false;

	if (!Vars::Aimbot::Projectile::Active.m_Var)
		return;

	Target_t Target = {};

	bool bShouldAim = (Vars::Aimbot::Global::AimKey.m_Var == VK_LBUTTON ? (pCmd->buttons & IN_ATTACK) : g_AimbotGlobal.IsKeyDown());

	if (GetTarget(pLocal, pWeapon, pCmd, Target) && bShouldAim)
	{
		g_Globals.m_nCurrentTargetIdx = Target.m_pEntity->entindex();

		//if (Vars::Aimbot::Projectile::AimMethod.m_Var == 1)
			//g_Globals.m_vAimPos = g_Globals.m_vPredictedPos;

		if (ShouldFire(pCmd))
		{
			pCmd->buttons |= IN_ATTACK;

			if (g_Globals.m_nCurItemDefIndex == Soldier_m_TheBeggarsBazooka)
			{
				if (pWeapon->Clip1() > 0)
					pCmd->buttons &= ~IN_ATTACK;
			}

			else
			{
				if (pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW && pWeapon->m_flChargeBeginTime() > 0.0f)
					pCmd->buttons &= ~IN_ATTACK;
			}
		}

		bool bIsAttacking = IsAttacking(pCmd, pWeapon);

		Aim(pCmd, pWeapon, Target.m_vAngleTo);
	}
}