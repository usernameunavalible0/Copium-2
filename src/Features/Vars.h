#pragma once

#include "../SDK/SDK.h"

template <class T>
class CVar
{
public:
	T m_Var;
	const wchar_t* m_szDisplayName;
};

namespace Vars
{
	namespace Menu
	{
		inline Rect_t Position = { 100, 100, 800, 420 };
		inline const auto Title = XorStr(L"Team Fortress 2");
		inline int TitleBarH = 16;

		inline int SpacingX = 8;
		inline int SpacingY = 4;
		inline int SpacingText = 4;

		inline int CheckBoxW = 14;
		inline int CheckBoxH = 14;
		inline int CheckBoxFillSize = 3;

		inline int ButtonW = 90;
		inline int ButtonH = 20;

		inline int ButtonWSmall = 70;
		inline int ButtonHSmall = 16;

		inline int ComboBoxW = 90;
		inline int ComboBoxH = 14;

		inline int InputBoxW = 90;
		inline int InputBoxH = 14;
		inline int InputColorBoxW = 30;

		namespace Colors
		{
			inline Color WindowBackground = { 30, 30, 30, 255 };
			inline Color TitleBar = { 50, 50, 50, 255 };
			inline Color Text = { 255, 255, 255, 190 };
			inline Color Widget = { 20, 20, 20, 255 };
			inline Color WidgetActive = { 70, 70, 70, 255 };
			inline Color OutlineMenu = { 255, 255, 255, 20 };
		}
	}

	namespace ESP
	{
		inline CVar<bool> Enabled{ true, L"Active" };

		namespace Players
		{
			inline CVar<bool> Enabled{ true, L"Active" };
			inline CVar<bool> IgnoreTeam{ true, L"Ignore Team" };
			inline CVar<bool> ActiveWeapon{ true, L"Active Weapon" };
			inline CVar<bool> HealthText{ true, L"Health Text" };
			inline CVar<bool> Name{ true, L"Name" };
			inline CVar<bool> Box{ false, L"2D Box" };
			inline CVar<bool> HealthBar{ true, L"Health Bar" };
		}

		namespace Buildings
		{
			inline CVar<bool> Enabled{ true, L"Active" };
			inline CVar<bool> IgnoreTeam{ false, L"Ignore Team" };
			inline CVar<bool> HealthText{ true, L"Health Text" };
			inline CVar<bool> Name{ true, L"Name" };
			inline CVar<bool> Info{ true, L"Info" };
		}
	}

	namespace Visual
	{
		inline CVar<bool> RemoveVisualRecoil{ true, L"Remove Visual Recoil" };
		inline CVar<bool> Tracers{ true, L"Bullet Tracers" };
		inline CVar<bool> ToolTips{ true, L"ToolTips" };
		inline CVar<bool> Snow{ true, L"Snow" };
		inline CVar<int> FieldOfView{ 110, L"Field Of View" };
		inline CVar<bool> RemoveZoom{ true, L"Remove Zoom" };
		inline CVar<bool> RemoveScope{ true, L"Remove Scope" };
		inline CVar<int> ThirdPersonKey{ 0x52, L"Third Person Key" };
		inline CVar<bool> ThirdPerson{ true, L"ThirdPerson" };
	}

	namespace Misc
	{
		inline CVar<bool> Bunnyhop{ true, L"Bunnyhop"};
		inline CVar<bool> AutoStrafe{ true, L"Auto Strafe" };
		inline CVar<bool> BypassPure{ true, L"Bypass Pure" };
	}
}