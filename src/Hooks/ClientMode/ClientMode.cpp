#include "ClientMode.h"

#include "../../Features/Vars.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Visual/Visuals.h"
#include "../../Features/Aimbot/Aimbot.h"
#include "../CL_Main/CL_Main.h"

using namespace Hooks;

void __fastcall ClientMode::OverrideView::Detour(void* ecx, void* edx, CViewSetup* pSetup)
{
	Table.Original<FN>(Index)(ecx, edx, pSetup);
	F::Visual.FOV(pSetup);
}

bool __fastcall ClientMode::CreateMove::Detour(void* ecx, void* edx, float flInputSampleTime, CUserCmd* cmd)
{
	g_Globals.m_bSilentTime = false;
	g_Globals.m_bAttacking = false;

	FN OriginalFN = Table.Original<FN>(Index);

	if (!cmd || cmd->command_number <= 0)
		return OriginalFN(ecx, edx, flInputSampleTime, cmd);

	uintptr_t _bp; __asm mov _bp, ebp;
	bool* pbSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

	CUtlFlags<int> nOldFlags = 0;
	QAngle vOldAngles = cmd->viewangles;
	float fOldSide = cmd->sidemove;
	float fOldForward = cmd->forwardmove;

	const auto& pLocal = g_EntityCache.GetLocal();

	if (pLocal && pLocal->IsAlive())
	{
		nOldFlags = pLocal->m_fFlags();

		if (const auto& pWeapon = g_EntityCache.GetWeapon())
		{
			g_Globals.m_nCurItemDefIndex = pWeapon->m_iItemDefinitionIndex();
			g_Globals.m_bWeaponCanHeadShot = pWeapon->CanHeadShot();
			g_Globals.m_bWeaponCanAttack = CanShoot(pLocal, pWeapon);
			g_Globals.m_WeaponType = GetWeaponType(pWeapon);

			if (pWeapon->GetSlot() != 2)
			{
				if (pWeapon->IsReloading())
					g_Globals.m_bWeaponCanAttack = true;

				if (g_Globals.m_nCurItemDefIndex != Soldier_m_TheBeggarsBazooka)
				{
					if (pWeapon->Clip1() == 0)
						g_Globals.m_bWeaponCanAttack = false;
				}
			}

			F::Prediction.Start(pLocal, cmd);
			{
				//Run aimbot, triggerbot etc. here
				F::Aimbot.Run(cmd);
			}
			F::Prediction.Finish(pLocal);
		}

		F::Misc.Run(pLocal, cmd);

	}

	static bool bWasSet = false;

	if (g_Globals.m_bSilentTime)
	{
		*pbSendPacket = false;
		bWasSet = true;
	}

	else
	{
		if (bWasSet)
		{
			*pbSendPacket = true;
			cmd->viewangles = vOldAngles;
			cmd->sidemove = fOldSide;
			cmd->forwardmove = fOldForward;
			bWasSet = false;
		}
	}

	//failsafe
	{
		static int nChoked = 0;

		if (!*pbSendPacket)
			nChoked++;

		else nChoked = 0;

		if (nChoked > 14)
			*pbSendPacket = true;
	}

	return g_Globals.m_bSilentTime || g_Globals.m_bHitscanSilentActive || g_Globals.m_bProjectileSilentActive ? false : OriginalFN;
}

bool __fastcall ClientMode::ShouldDrawViewModel::Detour(void* ecx, void* edx)
{
	if (const auto& pLocal = g_EntityCache.GetLocal())
	{
		if (pLocal->IsZoomed() && Vars::Visual::RemoveScope.m_Var && Vars::Visual::RemoveZoom.m_Var && !I::Input->CAM_IsThirdPerson())
			return true;
	}

	return Table.Original<FN>(Index)(ecx, edx);
}

float __fastcall ClientMode::GetViewModelFOV::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

bool __fastcall ClientMode::DoPostScreenSpaceEffects::Detour(void* ecx, void* edx, const CViewSetup* pSetup)
{
	return Table.Original<FN>(Index)(ecx, edx, pSetup);
}

bool __fastcall ClientMode::ShouldBlackoutAroundHUD::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

void ClientMode::Initialize()
{
	XASSERT(Table.Initialize(I::ClientMode) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&OverrideView::Detour, OverrideView::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&CreateMove::Detour, CreateMove::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&ShouldDrawViewModel::Detour, ShouldDrawViewModel::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&GetViewModelFOV::Detour, GetViewModelFOV::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&DoPostScreenSpaceEffects::Detour, DoPostScreenSpaceEffects::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&ShouldBlackoutAroundHUD::Detour, ShouldBlackoutAroundHUD::Index) == FAILED_TO_HOOK);
}