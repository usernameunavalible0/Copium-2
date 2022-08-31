#include "AimbotMedic.h"

void CAimbotMedic::Run(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	C_TFPlayer* pEntity = GetBestTarget(pLocal)->As<C_TFPlayer*>();

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity;
	pEntity->GetHitboxPosition(iBestHitbox, vEntity);

	Vector vLocal = pLocal->Weapon_ShootPosition();

	QAngle angs;
	VectorAngles((vEntity - vLocal), angs);

	ClampAngles(angs);
	cmd->viewangles = angs;

	I::EngineClient->SetViewAngles(cmd->viewangles);

	cmd->buttons |= IN_ATTACK;
}

IClientEntity* CAimbotMedic::GetBestTarget(C_TFPlayer* pLocal)
{
	//this num could be smaller 
	float flDistToBest = 99999.f;

	Vector vLocal = pLocal->Weapon_ShootPosition();

	for (auto Entity : g_EntityCache.GetGroup(EEntGroup::PLAYERS_TEAMMATES))
	{
		auto pEntity = Entity->As<C_TFPlayer*>();

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity;
		pEntity->GetHitboxPosition(iBestHitbox, vEntity);

		if (pEntity->IsInvulnerable())
			continue;

		float flDistToTarget = (vLocal - vEntity).Length();

		if (flDistToTarget < flDistToBest)
		{
			flDistToBest = flDistToTarget;
			return Entity;
		}
	}

}

int CAimbotMedic::GetBestHitbox(C_TFPlayer* pLocal, C_TFPlayer* pEntity)
{
	int iBestHitbox = -1;

	iBestHitbox = HITBOX_BODY;

	Vector vEntity;
	pEntity->GetHitboxPosition(iBestHitbox, vEntity);

	if (vEntity.IsZero())
		return -1;

	if (!G::Util.IsVisible(pLocal->Weapon_ShootPosition(), vEntity))
		return -1;

	return iBestHitbox;
}