#include "SurfaceHook.h"

#include "../../Features/Menu/Menu.h"

void __fastcall Hooks::SurfaceHook::OnScreenSizeChanged::Func(void* ecx, void* edx, int OldWidth, int OldHeight)
{
	Hook.Original<fn>(Index)(ecx, edx, OldWidth, OldHeight);
	G::Draw.ReloadFonts();
	G::Draw.ReloadScreenSize();
}

void __fastcall Hooks::SurfaceHook::LockCursor::Detour(void* ecx, void* edx)
{
	F::Menu.m_bOpen ? I::VGuiSurface->UnlockCursor() : Hook.Original<FN>(Index)(ecx, edx);
}

void Hooks::SurfaceHook::Init()
{
	XASSERT(Hook.Initialize(I::VGuiSurface) == FAILED_TO_INITIALIZE);
	XASSERT(Hook.Hook(&OnScreenSizeChanged::Func, OnScreenSizeChanged::Index) == FAILED_TO_HOOK);
	XASSERT(Hook.Hook(&LockCursor::Detour, LockCursor::Index) == FAILED_TO_HOOK);
}