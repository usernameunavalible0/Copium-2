#include "Visuals.h"
#include "../Vars.h"
#include "../Menu/Menu.h"

void CVisuals::FOV(CViewSetup* pView)
{
	const auto& pLocal = g_EntityCache.GetLocal();

	if (pLocal && pView)
	{
		if (pLocal->GetObserverMode() == OBS_MODE_IN_EYE || (pLocal->IsZoomed() && !Vars::Visual::RemoveZoom.m_Var))
			return;

		pView->fov = Vars::Visual::FieldOfView.m_Var;

		if (pLocal->IsAlive())
			pLocal->m_iFOV() = Vars::Visual::FieldOfView.m_Var;
	}
}

bool CVisuals::RemoveScope(int nPanel)
{
	if (!m_nHudZoom && Hash::IsHudScope(I::VGuiPanel->GetName(nPanel)))
		m_nHudZoom = nPanel;

	return (Vars::Visual::RemoveScope.m_Var && nPanel == m_nHudZoom);
}

void CVisuals::ThirdPerson()
{
	if (const auto& pLocal = g_EntityCache.GetLocal())
	{
		if (Vars::Visual::ThirdPersonKey.m_Var)
		{
			if (!I::EngineVGui->IsGameUIVisible() && !I::MatSystemSurface->IsCursorVisible() && !F::Menu.m_bTyping)
			{
				static float flPressedTime = I::EngineClient->Time();
				float flElapsed = I::EngineClient->Time() - flPressedTime;

				if ((GetAsyncKeyState(Vars::Visual::ThirdPersonKey.m_Var) & 0x8000) && flElapsed > 0.2f) {
					Vars::Visual::ThirdPerson.m_Var = !Vars::Visual::ThirdPerson.m_Var;
					flPressedTime = I::EngineClient->Time();
				}
			}
		}

		bool bIsInThirdPerson = I::Input->CAM_IsThirdPerson();

		if (!Vars::Visual::ThirdPerson.m_Var
			|| ((!Vars::Visual::RemoveScope.m_Var || !Vars::Visual::RemoveZoom.m_Var) && pLocal->IsZoomed()))
		{
			if (bIsInThirdPerson)
				pLocal->m_nForceTauntCam() = 0;

			return;
		}

		if (!bIsInThirdPerson)
			pLocal->m_nForceTauntCam() = 1;
	}
}