#include "Backstab.h"
#include "../../Vars.h"

void CBackstab::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWep, CUserCmd* cmd)
{
	auto id = pWep->GetWeaponID();

	if (Vars::Auto::Backstab::Active.m_Var)
	{
		if (pLocal->m_iClass() == TF_CLASS_SPY && id == TF_WEAPON_KNIFE && pWep->m_bReadyToBackstab())
			cmd->buttons |= IN_ATTACK;
	}
}