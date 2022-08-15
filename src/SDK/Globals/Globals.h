#pragma once

class CGlobals
{
public:
	int m_nLocalIndex = 0;
	int m_nMaxClients = 0;
	int m_nMaxEntities = 0;
	int m_nScreenHeight = 0;
	int m_nScreenWidht = 0;
	int m_nCurItemDefIndex = 0;
	int m_nCurrentTargetIdx = 0;
	bool m_bHitscanRunning = false;
	bool m_bHitscanSilentActive = false;

	float m_flCurAimFOV = 0.0f;

	bool m_bAutoBackstabRunning = false;
	bool m_bWeaponCanAttack = false;
	bool m_bWeaponCanHeadShot = false;
	bool m_bIsInGame = false;
	bool m_bIsGameUIVisible = false;
};

inline CGlobals g_Globals;