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
	namespace Aimbot
	{
		namespace Global
		{
			inline CVar<bool> Active{ true, L"Active" };
			inline CVar<int> AimKey{ VK_LSHIFT, L"Aim Key" };
			inline CVar<bool> AutoShoot{ true, L"Auto Shoot" };
			inline CVar<bool> AimPlayers{ true, L"Aim Players" };
			inline CVar<bool> AimBuildings{ true, L"Aim Buildings" };
			inline CVar<bool> IgnoreInvlunerable{ true, L"Ignore Invulnerable" };
			inline CVar<bool> IgnoreCloaked{ false, L"Ignore Cloaked" };
			inline CVar<bool> IgnoreFriends{ true, L"Ignore Friends" };
			inline CVar<bool> IgnoreTaunting{ true, L"Ignore Taunting" };
		}

		namespace Hitscan
		{
			inline CVar<bool> Active{ true, L"Active" };
			inline CVar<int> SortMethod{ 0, L"Sort Method" };	//0 - FOV,		1 - Distance
			inline CVar<int> AimMethod{ 0, L"Aim Method" };	//0 - Normal,	1 - Smooth, 2 - Silent
			inline CVar<int> AimHitbox{ 2, L"Aim Hitbox" };	//0 - Head,		1 - Body,	2 - Auto
			inline CVar<float> AimFOV{ 15.0f, L"Aim FOV" };
			inline CVar<float> SmoothingAmount{ 4.0f, L"Smoothing Amount" };
			inline CVar<int> TapFire{ 1, L"Tap Fire" };	//0 - Off, 1 - Distance, 2 - Always
			inline CVar<bool> ScanHitboxes{ true, L"Scan Body" };
			inline CVar<bool> ScanHead{ true, L"Scan Head" };
			inline CVar<bool> ScanBuildings{ true, L"Scan Buildings" };
			inline CVar<bool> WaitForHeadshot{ true, L"Wait For Headshot" };
			inline CVar<bool> WaitForCharge{ true, L"Wait For Charge" };
			inline CVar<bool> SpectatedSmooth{ false, L"Spectated Smooth" };
			inline CVar<bool> ScopedOnly{ false, L"Scoped Only" };
			inline CVar<bool> AutoScope{ false, L"Auto Scope" };
			inline CVar<bool> AutoRev{ false, L"Auto Rev" };
		}

		namespace Projectile
		{
			inline CVar<bool> Active{ true, L"Active" };
			inline CVar<bool> PerformanceMode{ true, L"Performance Mode" };
			inline CVar<int> SortMethod{ 0, L"Sort Method" };	//0 - FOV,		1 - Distance
			inline CVar<int> AimMethod{ 1, L"Aim Method" };	//0 - Normal,	1 - Silent
			inline CVar<int> AimPosition{ 2, L"Aim Position" };	//0 - Body,		1 - Feet,	2 - Auto
			inline CVar<float> AimFOV{ 25.0f, L"Aim FOV" };
		}

		namespace Melee
		{
			inline CVar<bool> Active{ true, L"Active" };
			inline CVar<int> SortMethod{ 1, L"Sort Method" };	//0 - FOV,		1 - Distance
			inline CVar<int> AimMethod{ 1, L"Aim Method" };	//0 - Normal,	1 - Smooth, 2 - Silent
			inline CVar<float> AimFOV{ 45.0f, L"Aim FOV" };
			inline CVar<float> SmoothingAmount{ 8.0f, L"Smoothing Amount" };
			inline CVar<bool> RangeCheck{ true, L"Range Check" };
			inline CVar<bool> PredictSwing{ true, L"Predict Swing" };
			inline CVar<bool> WhipTeam{ true, L"Whip Teammates" };
		}
	}

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
		inline CVar<bool> ThirdPerson{ false, L"ThirdPerson" };
	}

	namespace Misc
	{
		inline CVar<bool> Bunnyhop{ true, L"Bunnyhop"};
		inline CVar<bool> AutoStrafe{ true, L"Auto Strafe" };
		inline CVar<bool> BypassPure{ true, L"Bypass Pure" };
		inline CVar<bool> NoPush{ true, L"No Push" };
	}
}