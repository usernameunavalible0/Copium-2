#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace SurfaceHook
	{
		inline Hook::CTable Hook;

		namespace OnScreenSizeChanged
		{
			using fn = void(__fastcall*)(void*, void*, int, int);
			constexpr uint32 Index = 111u;
			void __fastcall Func(void* ecx, void* edx, int OldWidht, int OldHeight);
		}

		namespace LockCursor
		{
			using FN = void(__fastcall*)(void*, void*);
			constexpr uint32 Index = 62u;
			void __fastcall Detour(void* ecx, void* edx);
		}

		void Init();
	}
}