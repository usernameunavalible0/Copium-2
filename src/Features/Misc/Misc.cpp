#include "Misc.h"

#include "../Vars.h"

void CFeatures_Misc::Run(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	if (Vars::Misc::Bunnyhop.m_Var)
		Bunnyhop(pLocal, cmd);

	AutoStrafe(cmd);
}

void CFeatures_Misc::Bunnyhop(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	static bool s_bState = false;

	if (cmd->buttons & IN_JUMP)
	{
		if (!s_bState && !pLocal->m_fFlags().IsFlagSet(FL_ONGROUND))
		{
			cmd->buttons &= ~IN_JUMP;
		}
		else if (s_bState)
		{
			s_bState = false;
		}
	}
	else if (!s_bState)
	{
		s_bState = true;
	}
}

void CFeatures_Misc::AutoStrafe(CUserCmd* pCmd)
{
	if (Vars::Misc::AutoStrafe.m_Var)
	{
		if (const auto& pLocal = g_EntityCache.GetLocal())
		{
			if (pLocal->IsAlive() && !pLocal->IsSwimming() && !pLocal->m_fFlags().IsFlagSet(FL_ONGROUND) && (pCmd->mousedx > 1 || pCmd->mousedx < -1))
				pCmd->sidemove = pCmd->mousedx > 1 ? 450.f : -450.f;
		}
	}
}