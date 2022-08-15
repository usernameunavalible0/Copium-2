#include "Menu.h"

#include "InputHelper/InputHelper.h"

#define FONT_MENU EFonts::MENU_VERDANA

void CMenu::DrawTooltip()
{
	if (m_szCurTip.empty() || !Vars::Visual::ToolTips.m_Var)
		return;

	int w, h;
	I::MatSystemSurface->GetTextSize(G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_hFont, m_szCurTip.data(), w, h);

	G::Draw.OutlinedRect(g_InputHelper.m_nMouseX + 13, g_InputHelper.m_nMouseY + 13, w + 4, h + 4, { 0, 0, 0, 255 });
	G::Draw.Rect(g_InputHelper.m_nMouseX + 14, g_InputHelper.m_nMouseY + 14, w + 2, h + 2, Vars::Menu::Colors::WindowBackground);
	G::Draw.String(FONT_MENU, g_InputHelper.m_nMouseX + 15, g_InputHelper.m_nMouseY + 15, Vars::Menu::Colors::Text, TXT_DEFAULT, m_szCurTip.data());
}

void CMenu::Separator()
{
	int x = m_LastWidget.x + m_LastWidget.width + Vars::Menu::SpacingX;
	int y = Vars::Menu::Position.y;
	int w = 0;
	int h = 0;

	G::Draw.Line(x, y, x, (y + Vars::Menu::Position.height - 1), Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x + Vars::Menu::SpacingX;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;
}

bool CMenu::CheckBox(CVar<bool>& Var, const wchar_t* const szToolTip)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::CheckBoxW;
	int h = Vars::Menu::CheckBoxH;

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (g_InputHelper.IsPressed(VK_LBUTTON)) {
			callback = true;
			Var.m_Var = !Var.m_Var;
		}

		G::Draw.Rect(x, y, w, h, Vars::Menu::Colors::WidgetActive);
		m_szCurTip += szToolTip;
	}

	if (Var.m_Var)
	{
		G::Draw.GradientRect(
			x,
			y,
			x + w,
			y + h,
			Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive,
			false);
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CMenu::Button(const wchar_t* Label, bool Active, int WidthOverride, int HeightOverride)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = WidthOverride ? WidthOverride : Vars::Menu::ButtonW;
	int h = HeightOverride ? HeightOverride : Vars::Menu::ButtonH;

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (g_InputHelper.IsPressed(VK_LBUTTON))
			callback = true;

		G::Draw.Rect(x, y, w, h, Vars::Menu::Colors::WidgetActive);
	}

	if (Active)
		G::Draw.GradientRect(x, y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, false);

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, Label);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	if (callback)
		m_bReopened = true;

	return callback;
}

bool CMenu::ComboBox(CVar<int>& Var, const std::vector<CVar<int>>& List)
{
	auto FindCurItemName = [&]() -> const wchar_t*
	{
		for (const auto& Item : List)
		{
			if (Item.m_Var == Var.m_Var)
				return Item.m_szDisplayName;
		}

		return _(L"UNKNOWN_ITEM");
	};

	auto FindCurItemIndex = [&]() -> int
	{
		for (size_t n = 0; n < List.size(); n++)
		{
			if (List[n].m_Var == Var.m_Var)
				return n;
		}

		return 0;
	};

	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::ComboBoxW;
	int h = Vars::Menu::ComboBoxH;

	static std::map<CVar<int>*, int> indexes = {};
	static std::map<CVar<int>*, bool> inits = {};

	if (!inits[&Var] || m_bReopened) {
		indexes[&Var] = FindCurItemIndex();
		inits[&Var] = true;
	}

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (indexes[&Var] > 0)
		{
			if (g_InputHelper.IsPressed(VK_LBUTTON)) {
				Var.m_Var = List[--indexes[&Var]].m_Var;
				callback = true;
			}

			G::Draw.GradientRect(x, y, x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
		}
	}

	else if (g_InputHelper.m_nMouseX > x + (w / 2) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (indexes[&Var] < int(List.size() - 1))
		{
			if (g_InputHelper.IsPressed(VK_LBUTTON)) {
				Var.m_Var = List[++indexes[&Var]].m_Var;
				callback = true;
			}

			G::Draw.GradientRect(x + (w / 2), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
		}
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, FindCurItemName());
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CMenu::InputFloat(CVar<float>& Var, float Min, float Max, float Step, const wchar_t* Fmt)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	if (Var.m_Var != Min)
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var -= Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x, y, x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
		}
	}

	if (Var.m_Var != Max)
	{
		if (g_InputHelper.m_nMouseX > x + (w / 2) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var += Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x + (w / 2), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
		}
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, Fmt, Var.m_Var);
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}


bool CMenu::InputInt(CVar<int>& Var, int Min, int Max, int Step)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	if (Var.m_Var != Min)
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var -= Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x, y, x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
		}
	}

	if (Var.m_Var != Max)
	{
		if (g_InputHelper.m_nMouseX > x + (w / 2) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var += Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x + (w / 2), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
		}
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%d", Var.m_Var);
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CMenu::InputColor(Color& Var, const wchar_t* Label)
{
	// sorry bucko no colors for u!
	// real talk the color class in this base is all inline so i cant edit atm
	return false;
}

bool CMenu::InputString(const wchar_t* Label, std::wstring& output)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW * 2;
	int h = Vars::Menu::ButtonH;

	static bool active = false;
	static std::wstring active_str = {};

	if (g_InputHelper.IsPressed(VK_LBUTTON))
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			active = !active;

		else active = false;
	}

	if (active)
	{
		m_bTyping = true;

		if (g_InputHelper.IsPressed(VK_INSERT)) {
			active = false;
			return false;
		}

		if (active_str.length() < 21)
		{
			if (g_InputHelper.IsPressed(VK_SPACE))
				active_str += char(VK_SPACE);

			for (int16_t key = L'A'; key < L'Z' + 1; key++)
			{
				if (g_InputHelper.IsPressed(key))
					active_str += wchar_t(key);
			}
		}

		if (g_InputHelper.IsPressedAndHeld(VK_BACK) && !active_str.empty())
			active_str.erase(active_str.end() - 1);

		if (g_InputHelper.IsPressed(VK_RETURN))
		{
			active = false;

			if (!active_str.empty())
			{
				output = active_str;
				callback = true;
			}
		}

		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%ws", active_str.empty() ? _(L"Enter a Name") : active_str.c_str());
	}

	else
	{
		active_str = {};
		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%ws", Label);
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CMenu::InputKey(CVar<int>& output, bool bAllowNone)
{
	auto VK2STR = [&](const short key) -> std::wstring
	{
		switch (key) {
		case VK_LBUTTON: return _(L"LButton");
		case VK_RBUTTON: return _(L"RButton");
		case VK_MBUTTON: return _(L"MButton");
		case VK_XBUTTON1: return _(L"XButton1");
		case VK_XBUTTON2: return _(L"XButton2");
		case 0x0: return _(L"None");
		default: break;
		}

		WCHAR output[16] = { L"\0" };

		if (const int result = GetKeyNameTextW(MapVirtualKeyW(key, MAPVK_VK_TO_VSC) << 16, output, 16))
			return output;

		return _(L"VK2STR_FAILED");
	};

	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	static bool active = false, old_active = active;
	static float time = I::EngineClient->Time();
	float elapsed = I::EngineClient->Time() - time;

	if (old_active != active) {
		time = I::EngineClient->Time();
		old_active = active;
	}

	if (!active && elapsed > 0.1f && g_InputHelper.IsPressed(VK_LBUTTON))
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h) {
			active = true;
			g_InputHelper.NullKey(VK_LBUTTON);
		}
	}

	static float time_notactive = 0.0f;

	if (active)
	{
		m_bTyping = true;

		if (I::EngineClient->Time() - time_notactive > 0.1f)
		{
			for (short n = 0; n < 256; n++)
			{
				if ((n > 0x0 && n < 0x7)
					|| (n > L'A' - 1 && n < L'Z' + 1)
					|| n == VK_LSHIFT || n == VK_RSHIFT || n == VK_SHIFT
					|| n == VK_ESCAPE || n == VK_INSERT)
				{
					if (g_InputHelper.IsPressed(n))
					{
						if (n == VK_INSERT) {
							active = false;
							break;
						}

						if (n == VK_ESCAPE && bAllowNone) {
							output.m_Var = 0x0;
							active = false;
							break;
						}

						output.m_Var = n;
						active = false;
						break;
					}
				}
			}
		}

		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%ws", _(L"Press a Key"));
	}

	else
	{
		time_notactive = I::EngineClient->Time();
		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%ws", VK2STR(output.m_Var).c_str());
	}

	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, output.m_szDisplayName);
	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

void CMenu::GroupBoxStart()
{
	m_LastGroupBox.x = m_LastWidget.x;
	m_LastGroupBox.y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;

	m_LastWidget.x += Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY * 2;
}

void CMenu::GroupBoxEnd(const wchar_t* Label, int Width)
{
	int h = m_LastWidget.y - m_LastGroupBox.y + m_LastWidget.height + Vars::Menu::SpacingY;

	int label_w, label_h;
	I::MatSystemSurface->GetTextSize(G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_hFont, Label, label_w, label_h);

	int label_x = m_LastGroupBox.x + Vars::Menu::SpacingText;
	int label_y = m_LastGroupBox.y - (label_h / 2);

	G::Draw.Line(m_LastGroupBox.x, m_LastGroupBox.y, label_x, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(label_x + label_w, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x + Width, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x + Width, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);

	G::Draw.String(FONT_MENU, label_x, label_y, Vars::Menu::Colors::Text, TXT_DEFAULT, Label);

	m_LastWidget.x -= Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY * 2;
	m_LastGroupBox.height = h;
}

void CMenu::Run()
{
	m_bReopened = false;
	m_bTyping = false;

	static bool bOldOpen = m_bOpen;

	if (bOldOpen != m_bOpen)
	{
		bOldOpen = m_bOpen;

		if (m_bOpen)
			m_bReopened = true;
	}

	if (I::EngineClient->IsDrawingLoadingImage())
	{
		m_bOpen = false;
		return;
	}

	static float flTimeOnChange = 0.0f;

	if (/*Utils::IsGameWindowInFocus() &&*/ (GetAsyncKeyState(VK_INSERT) & 1)) {
		I::MatSystemSurface->SetCursorAlwaysVisible(m_bOpen = !m_bOpen);
		flTimeOnChange = I::EngineClient->Time();
	}

	m_flFadeElapsed = I::EngineClient->Time() - flTimeOnChange;

	if (m_flFadeElapsed < m_flFadeDuration) {
		m_flFadeAlpha = RemapValClamped(m_flFadeElapsed, 0.0f, m_flFadeDuration, !m_bOpen ? 1.0f : 0.0f, m_bOpen ? 1.0f : 0.0f);
		I::MatSystemSurface->DrawSetAlphaMultiplier(m_flFadeAlpha);
	}

	if (m_bOpen || m_flFadeElapsed < m_flFadeDuration)
	{
		m_szCurTip = L"";
		g_InputHelper.Update();

		//Do the Watermark
		{
			G::Draw.Rect(20, 25, 160, 5, Vars::Menu::Colors::TitleBar);
			G::Draw.Rect(20, 30, 160, (G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_nTall * 3) + 5, Vars::Menu::Colors::WindowBackground);
			G::Draw.String(FONT_MENU, 22, 32, Vars::Menu::Colors::Text, TXT_DEFAULT, _("Copium v2"));
			G::Draw.String(FONT_MENU, 22, 32 + G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_nTall, Vars::Menu::Colors::Text, TXT_DEFAULT, _("Build: " __DATE__));
			G::Draw.String(FONT_MENU, 22, 32 + (G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_nTall * 2), Vars::Menu::Colors::Text, TXT_DEFAULT, _("Developers: DepressedPotato"));
		}

		//Do the Window
		{
			g_InputHelper.Drag(
				g_InputHelper.m_nMouseX,
				g_InputHelper.m_nMouseY,
				Vars::Menu::Position.x,
				Vars::Menu::Position.y,
				Vars::Menu::Position.width,
				Vars::Menu::TitleBarH,
				Vars::Menu::TitleBarH);

			G::Draw.Rect(
				Vars::Menu::Position.x,
				Vars::Menu::Position.y,
				Vars::Menu::Position.width,
				Vars::Menu::Position.height,
				Vars::Menu::Colors::WindowBackground);

			G::Draw.Rect(
				Vars::Menu::Position.x,
				Vars::Menu::Position.y - Vars::Menu::TitleBarH,
				Vars::Menu::Position.width,
				Vars::Menu::TitleBarH,
				Vars::Menu::Colors::TitleBar);

			G::Draw.String(FONT_MENU,
				Vars::Menu::Position.x + (Vars::Menu::Position.width / 2),
				Vars::Menu::Position.y - (Vars::Menu::TitleBarH / 2),
				Vars::Menu::Colors::Text,
				TXT_CENTERXY,
				"%ls", _(L"Team Fortress 2"));
		}

		//Do the Widgets
		{
			enum struct EMainTabs { TAB_AIM, TAB_VISUALS, TAB_MISC };
			enum struct EVisualTabs { TAB_ESP, TAB_MISC };

			m_LastWidget = { Vars::Menu::Position.x + Vars::Menu::SpacingX, Vars::Menu::Position.y, 0, 0 };

			static EMainTabs Tab = EMainTabs::TAB_AIM;
			{
				if (Button(_(L"Aim"), Tab == EMainTabs::TAB_AIM))
					Tab = EMainTabs::TAB_AIM;

				if (Button(_(L"Visuals"), Tab == EMainTabs::TAB_VISUALS))
					Tab = EMainTabs::TAB_VISUALS;

				if (Button(_(L"Misc"), Tab == EMainTabs::TAB_MISC))
					Tab = EMainTabs::TAB_MISC;

			}

			Separator();

			switch (Tab)
			{
				case EMainTabs::TAB_AIM:
				{
					Rect_t checkpoint = m_LastWidget;

					GroupBoxStart();
					{
						CheckBox(Vars::Aimbot::Global::Active, _(L"Aimbot master switch"));
						InputKey(Vars::Aimbot::Global::AimKey);
						CheckBox(Vars::Aimbot::Global::AutoShoot, _(L"Automatically shoot when target is found"));
						CheckBox(Vars::Aimbot::Global::AimPlayers, _(L"Aim at players"));
						CheckBox(Vars::Aimbot::Global::AimBuildings, _(L"Aim at buildings"));
						CheckBox(Vars::Aimbot::Global::IgnoreInvlunerable, _(L"Ignore players who can't be damaged"));
						CheckBox(Vars::Aimbot::Global::IgnoreCloaked, _(L"Ignore cloaked spies"));
						CheckBox(Vars::Aimbot::Global::IgnoreFriends, _(L"Ignore Steam friends"));
						CheckBox(Vars::Aimbot::Global::IgnoreTaunting, _(L"Ignore taunting players"));
					}
					GroupBoxEnd(_(L"Global"), 160);

					checkpoint.x += 160 + Vars::Menu::SpacingX;
					m_LastWidget = checkpoint;

					GroupBoxStart();
					{
						CheckBox(Vars::Aimbot::Hitscan::Active, _(L"Hitscan master switch"));
						ComboBox(Vars::Aimbot::Hitscan::SortMethod, { { 0, _(L"FOV") }, { 1, _(L"Distance") } });
						ComboBox(Vars::Aimbot::Hitscan::AimMethod, { { 0, _(L"Normal") }, { 1, _(L"Smooth") }, { 2, _(L"Silent") } });
						ComboBox(Vars::Aimbot::Hitscan::AimHitbox, { { 0, _(L"Head") }, { 1, _(L"Body") }, { 2, _(L"Auto") } });
						InputFloat(Vars::Aimbot::Hitscan::AimFOV, 1.0f, 180.0f, 1.0f, L"%.0f");
						InputFloat(Vars::Aimbot::Hitscan::SmoothingAmount, 1.0f, 10.0f, 1.0f, L"%.0f");
						ComboBox(Vars::Aimbot::Hitscan::TapFire, { { 0, _(L"Off") } , { 1, _(L"Distance") }, { 2, _(L"Always") } });
						CheckBox(Vars::Aimbot::Hitscan::AutoRev, _(L"Automatically rev minigun when aimbot active"));
						CheckBox(Vars::Aimbot::Hitscan::ScanHitboxes, _(L"Scan body edges"));
						CheckBox(Vars::Aimbot::Hitscan::ScanHead, _(L"Scan head edges"));
						CheckBox(Vars::Aimbot::Hitscan::ScanBuildings, _(L"Scan building edges"));
						CheckBox(Vars::Aimbot::Hitscan::WaitForHeadshot, _(L"Wait untill weapon can headshot"));
						CheckBox(Vars::Aimbot::Hitscan::WaitForCharge, _(L"Wait untill sniper can one shot target"));
						CheckBox(Vars::Aimbot::Hitscan::SpectatedSmooth, _(L"Toggle smooth aim when spectated"));
						CheckBox(Vars::Aimbot::Hitscan::ScopedOnly, _(L"Sniper only aims when scoped"));
						CheckBox(Vars::Aimbot::Hitscan::AutoScope, _(L"Sniper automatically scopes when target found"));
						//CheckBox(Vars::Misc::CL_Move::Doubletap, _(L"Shift tickbase while aimbotting when it's possible. Check the misc tab for more."));
					}
					GroupBoxEnd(_(L"Hitscan"), 215);

					checkpoint.x += 215 + Vars::Menu::SpacingX;
					m_LastWidget = checkpoint;

					GroupBoxStart();
					{
						CheckBox(Vars::Aimbot::Projectile::Active, _(L"Projectile master switch"));
						CheckBox(Vars::Aimbot::Projectile::PerformanceMode, _(L"Only target enemy closest to crosshair"));
						ComboBox(Vars::Aimbot::Projectile::SortMethod, { { 0, _(L"FOV") }, { 1, _(L"Distance") } });
						ComboBox(Vars::Aimbot::Projectile::AimMethod, { { 0, _(L"Normal") }, { 1, _(L"Silent") } });
						ComboBox(Vars::Aimbot::Projectile::AimPosition, { { 0, _(L"Body") }, { 1, _(L"Feet") }, { 2, _(L"Auto") } });
						InputFloat(Vars::Aimbot::Projectile::AimFOV, 1.0f, 180.0f, 1.0f, L"%.0f");
					}
					GroupBoxEnd(_(L"Projectile"), 215);

					GroupBoxStart();
					{
						CheckBox(Vars::Aimbot::Melee::Active, _(L"Melee master switch"));
						ComboBox(Vars::Aimbot::Melee::SortMethod, { { 0, _(L"FOV") }, { 1, _(L"Distance") } });
						ComboBox(Vars::Aimbot::Melee::AimMethod, { { 0, _(L"Normal") }, { 1, _(L"Smooth") }, { 2, _(L"Silent") } });
						InputFloat(Vars::Aimbot::Melee::AimFOV, 1.0f, 180.0f, 1.0f, L"%.0f");
						InputFloat(Vars::Aimbot::Melee::SmoothingAmount, 1.0f, 10.0f, 0.5f, L"%.0f");
						CheckBox(Vars::Aimbot::Melee::RangeCheck, _(L"Only aim when target in range"));
						CheckBox(Vars::Aimbot::Melee::PredictSwing, _(L"Predict melee attack"));
						CheckBox(Vars::Aimbot::Melee::WhipTeam, _(L"Hit teammates when holding the Disciplinary action"));
					}
					GroupBoxEnd(_(L"Melee"), 215);

					break;
				}

				case EMainTabs::TAB_VISUALS:
				{
					static EVisualTabs Tab = EVisualTabs::TAB_ESP;
					{
						Rect_t checkpoint_line = m_LastWidget;
						checkpoint_line.x -= Vars::Menu::SpacingX;
						checkpoint_line.y += Vars::Menu::ButtonHSmall + (Vars::Menu::SpacingY * 2);
						Rect_t checkpoint_move = m_LastWidget;

						if (Button(_(L"ESP"), Tab == EVisualTabs::TAB_ESP, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
							Tab = EVisualTabs::TAB_ESP;

						checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::SpacingX;
						m_LastWidget = checkpoint_move;

						if (Button(_(L"Misc"), Tab == EVisualTabs::TAB_MISC, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
							Tab = EVisualTabs::TAB_MISC;

						checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::SpacingX;
						m_LastWidget = checkpoint_move;

						m_LastWidget = checkpoint_line;
						G::Draw.Line(checkpoint_line.x, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.width - 1, checkpoint_line.y, Vars::Menu::Colors::OutlineMenu);
						checkpoint_line.x += Vars::Menu::SpacingX;
						checkpoint_line.y += Vars::Menu::SpacingY;
						m_LastWidget = checkpoint_line;
					}

					switch (Tab)
					{
						case EVisualTabs::TAB_ESP:
						{
							Rect_t checkpoint = m_LastWidget;

							GroupBoxStart();
							{
								CheckBox(Vars::ESP::Enabled, L"ESP Enabled?");
							}
							GroupBoxEnd(_(L"Main"), 210);

							checkpoint.x += 210 + Vars::Menu::SpacingX;
							m_LastWidget = checkpoint;

							GroupBoxStart();
							{
								CheckBox(Vars::ESP::Players::Enabled, L"Player ESP?");
								CheckBox(Vars::ESP::Players::IgnoreTeam, L"Ignore Local Team for Player ESP?");
								CheckBox(Vars::ESP::Players::ActiveWeapon, L"ESP for the players Active Weapon?");
								CheckBox(Vars::ESP::Players::HealthText, L"Text to show Player health?");
								CheckBox(Vars::ESP::Players::Name, L"Text to show Player Name");
								CheckBox(Vars::ESP::Players::Box, L"2D Box around the player?");
								CheckBox(Vars::ESP::Players::HealthBar, L"Bar to the side of the player to show health");
							}
							GroupBoxEnd(L"Players", 210);

							checkpoint.x += 210 + Vars::Menu::SpacingX;
							m_LastWidget = checkpoint;

							GroupBoxStart();
							{
								CheckBox(Vars::ESP::Buildings::Enabled, L"Building ESP?");
								CheckBox(Vars::ESP::Buildings::IgnoreTeam, L"Ignore Local Team?");
								CheckBox(Vars::ESP::Buildings::HealthText, L"Text to show building health?");
								CheckBox(Vars::ESP::Buildings::Name, L"Text to show building type?");
								CheckBox(Vars::ESP::Buildings::Info, L"Text to show Misc info about the building?");
							}
							GroupBoxEnd(L"Buildings", 210);

							break;
						}

						case EVisualTabs::TAB_MISC:
						{
							Rect_t checkpoint = m_LastWidget;

							GroupBoxStart();
							{
								CheckBox(Vars::Visual::RemoveVisualRecoil, L"Remove visual recoil?");
								CheckBox(Vars::Visual::Tracers, L"Bullet Tracers?");
								CheckBox(Vars::Visual::ToolTips, L"Menu ToolTips?");
								CheckBox(Vars::Visual::Snow, L"Menu Snow?");
								InputInt(Vars::Visual::FieldOfView, 70, 140);
								CheckBox(Vars::Visual::RemoveScope, L"Remove Sniper Scope?");
								CheckBox(Vars::Visual::RemoveZoom, L"Remove Sniper Scope Zoom");
								InputKey(Vars::Visual::ThirdPersonKey, L"Third Person Activation Key");
								CheckBox(Vars::Visual::ThirdPerson, L"Active the use of this thirdperson key");
							}
							GroupBoxEnd(L"Main", 210);

							break;
						}
					}

					break;
				}

				case EMainTabs::TAB_MISC:
				{
					GroupBoxStart();
					{
						CheckBox(Vars::Misc::Bunnyhop, L"Automatic Bunnyhop?");
						CheckBox(Vars::Misc::AutoStrafe, L"Automatic Air Strafing");
						CheckBox(Vars::Misc::BypassPure, L"Bypass sv_pure?");
					}
					GroupBoxEnd(L"Main", 210);

					break;
				}
			}

		}

		DrawTooltip();

		if (Vars::Visual::Snow.m_Var)
		{
			struct SnowFlake_t {
				float x = 0.0f, y = 0.0f, fall = 0.0f, drift = 0.0f;
				int size = 1;
			};

			static std::vector<SnowFlake_t> SnowFlakes;
			static const int Count = 500;
			static bool Init = false;

			if (!Init)
			{
				for (int n = 0; n < Count; n++)
				{
					SnowFlake_t Flake = {};
					Flake.x = static_cast<float>(RandIntSimple(0, g_Globals.m_nScreenWidht));
					Flake.y = -static_cast<float>(RandIntSimple(50, 500));
					Flake.fall = static_cast<float>(RandIntSimple(50, 100));
					Flake.drift = static_cast<float>(RandIntSimple(5, 20));
					Flake.size = RandIntSimple(1, 4) == 4 ? 2 : 1;
					SnowFlakes.push_back(Flake);
				}

				Init = true;
			}

			for (auto& Flake : SnowFlakes)
			{
				Flake.x += ((sinf(I::GlobalVars->curtime) * Flake.drift) * I::GlobalVars->interval_per_tick);
				Flake.y += (Flake.fall * I::GlobalVars->interval_per_tick);

				float Alpha = MapFloat(Flake.y, 0.0f, static_cast<float>(g_Globals.m_nScreenHeight - 200), 1.0f, 0.0f);

				if (Alpha <= 0.0f)
				{
					Flake.x = static_cast<float>(RandIntSimple(0, g_Globals.m_nScreenWidht));
					Flake.y = -static_cast<float>(RandIntSimple(50, 500));
					Flake.fall = static_cast<float>(RandIntSimple(50, 100));
					Flake.drift = static_cast<float>(RandIntSimple(5, 20));
					Flake.size = RandIntSimple(1, 4) == 4 ? 2 : 1;
				}

				Color Color = { 255, 255, 255, static_cast<byte>(Alpha * 255.0f) };
				G::Draw.Rect(static_cast<int>(Flake.x), static_cast<int>(Flake.y), Flake.size, Flake.size, Color);
			}
		}
	}

	I::MatSystemSurface->DrawSetAlphaMultiplier(1.0f);
}

