#pragma once
#include "../hl2_src/hl2_src.h"

enum class EEntGroup {
	PLAYERS_ALL, PLAYERS_ENEMIES, PLAYERS_TEAMMATES,
	BUILDINGS_ALL, BUILDINGS_ENEMIES, BUILDINGS_TEAMMATES, WORLD_PROJECTILES
};

class CEntityCache
{
private:
	std::map<EEntGroup, std::vector<IClientEntity*>> m_Groups;
	C_TFPlayer* m_pLocal = nullptr;
	C_TFWeaponBase* m_pWeapon = nullptr;

public:
	void Fill();
	void Clear();

public:
	const std::vector<IClientEntity*>& GetGroup(EEntGroup Group);
	C_TFPlayer* GetLocal();
	C_TFWeaponBase* GetWeapon();
};

inline CEntityCache g_EntityCache;