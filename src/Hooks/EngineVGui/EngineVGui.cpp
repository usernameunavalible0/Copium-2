#include "EngineVGui.h"

#include "../../Features/ESP/ESP.h"
#include "../../Features/Menu/Menu.h"
#include "../../Features/Visual/Visuals.h"

using namespace Hooks;

#define DBG(...) G::Draw.String(EFonts::DEBUG, 5, nY, COLOR_GREY, TXT_DEFAULT, __VA_ARGS__); nY += G::Draw.GetFontHeight(EFonts::DEBUG)

void __fastcall EngineVGui::Paint::Detour(void* ecx, void* edx, int mode)
{
	static const auto pfStartDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(g_Offsets.m_dwStartDrawing);
	static const auto pfFinishDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(g_Offsets.m_dwFinishDrawing);

	Table.Original<FN>(Index)(ecx, edx, mode);

	if (mode & PaintMode_t::PAINT_UIPANELS)
	{
		G::Draw.ReloadMatrix();

		pfStartDrawing(I::MatSystemSurface);
		{
			if (g_Globals.m_bIsInGame && !g_Globals.m_bIsGameUIVisible)
			{
				C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

				if (pLocal)
				{
					F::ESP.Render(pLocal);
				}

				if (Vars::ESP::Players::Bones.m_Var)
				{
					F::Visual.Skeleton({ 8, 7, 6, 4 }, COLOR_WHITE);
					F::Visual.Skeleton({ 11, 10, 9, 4 }, COLOR_WHITE);
					F::Visual.Skeleton({ 0, 4, 1 }, COLOR_WHITE);
					F::Visual.Skeleton({ 14, 13, 1 }, COLOR_WHITE);
					F::Visual.Skeleton({ 17, 16, 1 }, COLOR_WHITE);
				}

				//Current Active Aimbot FOV
				if (/*Vars::Visual::AimFOVAlpha.m_Var &&*/ g_Globals.m_flCurAimFOV)
				{
					if (const auto& pLocal = g_EntityCache.GetLocal())
					{
						float flFOV = static_cast<float>(Vars::Visual::FieldOfView.m_Var);
						float flR = tanf(DEG2RAD(g_Globals.m_flCurAimFOV) / 2.0f)
							/ tanf(DEG2RAD((pLocal->IsZoomed() && !Vars::Visual::RemoveZoom.m_Var) ? 30.0f : flFOV) / 2.0f) * g_Globals.m_nScreenWidht;
						Color clr = COLOR_WHITE;
						//clr.a = static_cast<byte>(Vars::Visuals::AimFOVAlpha.m_Var);
						G::Draw.OutlinedCircle(g_Globals.m_nScreenWidht / 2, g_Globals.m_nScreenHeight / 2, flR, 68, clr);
					}
				}

			}

			if (!F::Menu.m_bOpen)
			{
				G::Draw.String(EFonts::DEBUG, 5, 5, { 204, 204, 204, 255 }, TXT_DEFAULT, L"Copium-v2 TF2");
			}

			F::Menu.Run();

		}
		pfFinishDrawing(I::MatSystemSurface);
	}
}

void EngineVGui::Initialize()
{
	XASSERT(Table.Initialize(I::EngineVGui) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&Paint::Detour, Paint::Index) == FAILED_TO_HOOK);
}