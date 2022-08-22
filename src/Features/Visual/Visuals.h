#pragma once
#include "../../SDK/SDK.h"

class CVisuals
{
private:
	int m_nHudZoom = 0;

public:
	bool RemoveScope(int nPanel);
	void FOV(CViewSetup* pView);
	void ThirdPerson();
	void Hitboxes();
	void Skeleton(const std::vector<int>& vecBonex, Color clr);
};

namespace F { inline CVisuals Visual; }
