#include "Visuals.h"
#include "../Vars.h"
#include "../Menu/Menu.h"

void CVisuals::Skeleton(const std::vector<int>& vecBones, Color clr)
{
	if (auto pLocal = g_EntityCache.GetLocal())
	{
		//if (pLocal->deadflag())
			//return;

		for (auto pEntity : g_EntityCache.GetGroup(EEntGroup::PLAYERS_ENEMIES))
		{
			auto pPlayer = pEntity->As<C_TFPlayer*>();

			if (pPlayer->deadflag())
				continue;

			if (pPlayer == pLocal)
				continue;

			size_t n, nMax = vecBones.size(), nLast = (nMax - 1);

			for (n = 0; n < nMax; n++)
			{
				if (n == nLast)
					continue;

				Vector vBone, vParent;
				pPlayer->GetHitboxPosition(vecBones[n], vBone);
				pPlayer->GetHitboxPosition(vecBones[n + 1], vParent);

				Vector2D vScreenBone, vScreenParent;

				if (GetVectorInScreenSpace(vBone, vScreenBone) && GetVectorInScreenSpace(vParent, vScreenParent))
					G::Draw.Line(vScreenBone.x, vScreenBone.y, vScreenParent.x, vScreenParent.y, clr);
			}
		}
	}
}

void CVisuals::Lines()
{
	if (auto pLocal = g_EntityCache.GetLocal())
	{
		for (auto pEntity : g_EntityCache.GetGroup(EEntGroup::PLAYERS_ENEMIES))
		{
			auto pPlayer = pEntity->As<C_TFPlayer*>();

			if (pPlayer->deadflag())
				continue;

			Vector2D vBody = {};

			Vector pelvis;
			pPlayer->GetHitboxPosition(HITBOX_PELVIS, pelvis);
			if (!GetVectorInScreenSpace(pelvis, vBody))
				continue;

			bool bIsLocal = pPlayer == pLocal;
			
			Color DrawColor = G::Util.GetTeamColor(pPlayer->m_iTeamNum());

			if (!bIsLocal)
			{
				Vector2D vOrigin = {};

				if (GetVectorInScreenSpace(pPlayer->GetAbsOrigin(), vOrigin))
				{
					G::Draw.Line(
						g_Globals.m_nScreenWidht / 2, g_Globals.m_nScreenHeight,
						static_cast<int>(vOrigin.x), static_cast<int>(vOrigin.y),
						DrawColor
					);
				}
			}
		}
	}
}

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