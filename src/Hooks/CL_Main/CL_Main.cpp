#include "CL_Main.h"

#include "../../Features/Vars.h"

using namespace Hooks;

CPureServerWhitelist* __cdecl CL_Main::CL_LoadWhitelist::Detour(INetworkStringTable* pTable, const char* pName)
{
	return Vars::Misc::BypassPure.m_Var ? NULL : Func.Original<FN>()(pTable, pName);
}

void __cdecl CL_Main::CL_Move::Detour(float accumulated_extra_samples, bool bFinalTick)
{
	if (Vars::CL_Move::Enabled.m_Var)
	{
		if (Vars::CL_Move::TeleportKey.m_Var && (GetAsyncKeyState(Vars::CL_Move::TeleportKey.m_Var)) && !g_Globals.m_nShifted) //teleport
		{
			while (g_Globals.m_nShifted < MAX_NEW_COMMANDS_HEAVY)
			{
				g_Globals.m_nShifted++;
				Func.Original<FN>()(accumulated_extra_samples, (g_Globals.m_nShifted == (MAX_NEW_COMMANDS_HEAVY - 1))); //this teleports you
				//g_GlobalInfo.m_nShifted++;
			}

			return;
		}

		if (GetAsyncKeyState(Vars::CL_Move::RechargeKey.m_Var)) //recharge key
			g_Globals.m_bRecharging = true;
	}

	if (g_Globals.m_bRecharging && g_Globals.m_nShifted) //recharge
	{
		g_Globals.m_nShifted--; //goes from 15 to 0
		g_Globals.m_nWaitForShift = DT_WAIT_CALLS;
		return;
	}
	else
		g_Globals.m_bRecharging = false;




	Func.Original<FN>()(accumulated_extra_samples, (g_Globals.m_bShouldShift && !g_Globals.m_nWaitForShift) ? true : bFinalTick);

	if (g_Globals.m_nWaitForShift)
	{
		g_Globals.m_nWaitForShift--;
		return;
	}

	if (g_Globals.m_bShouldShift)
	{
		const auto& pLocal = g_EntityCache.GetLocal();
		int nClass = pLocal->m_iClass();
		int dtTicks;
		if (nClass == TF_CLASS_HEAVYWEAPONS) {
			dtTicks = MAX_NEW_COMMANDS_HEAVY;
		}
		else {
			dtTicks = MAX_NEW_COMMANDS;
		}
		if (GetAsyncKeyState(Vars::CL_Move::DoubletapKey.m_Var)) {
			while (g_Globals.m_nShifted < dtTicks)
			{

				if (!Vars::CL_Move::NotInAir.m_Var) {
					Func.Original<FN>()(accumulated_extra_samples, (g_Globals.m_nShifted == (dtTicks - 1))); //this doubletaps
					g_Globals.m_nShifted++;
				}
				if (Vars::CL_Move::NotInAir.m_Var) {

					if (pLocal->m_fFlags().IsFlagSet(FL_ONGROUND)) {
						g_Globals.fast_stop = true;
						Func.Original<FN>()(accumulated_extra_samples, (g_Globals.m_nShifted == (dtTicks - 1))); //this doubletaps
						g_Globals.m_nShifted++;
					}
					else {
						return;
					}
				}
			}
		}

		g_Globals.m_bShouldShift = false;
	}
}

void __fastcall CL_Main::CL_SendMove::Detour(void* ecx, void* edx)
{
	Func.Original<FN>()(ecx, edx);
}

void CL_Main::Initialize()
{
	//CL_LoadWhitelist
	{
		using namespace CL_LoadWhitelist;

		const FN pfCLLoadWhitelist = reinterpret_cast<FN>(g_Offsets.m_dwCLLoadWhitelist);
		XASSERT(pfCLLoadWhitelist == nullptr);

		if (pfCLLoadWhitelist)
			XASSERT(Func.Initialize(pfCLLoadWhitelist, &Detour) == FAILED_TO_INITIALIZE);
	}

	//CL_Move
	{
		using namespace CL_Move;

		const FN pfCLMove = reinterpret_cast<FN>(g_Offsets.m_dwCLMove);
		XASSERT(pfCLMove == nullptr);

		if (pfCLMove)
			XASSERT(Func.Initialize(pfCLMove, &Detour) == FAILED_TO_INITIALIZE);
	}

	//CL_SendMove
	{
		//using namespace CL_SendMove;

		//const FN pfCLSendMove = reinterpret_cast<FN>(g_Offsets.m_dwCLSendMove);
		//XASSERT(pfCLSendMove == nullptr);

		//if (pfCLSendMove)
		//	XASSERT(Func.Initialize(pfCLSendMove, &Detour) == FAILED_TO_INITIALIZE);
	}
}