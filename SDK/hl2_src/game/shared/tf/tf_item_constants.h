//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#ifndef TFITEMCONSTANTS_H // ECON_ITEM_CONSTANTS_H is used by src/common/econ_item_view.h
#define TFITEMCONSTANTS_H
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Weapon Types
//-----------------------------------------------------------------------------
enum
{
	TF_WPN_TYPE_PRIMARY = 0,
	TF_WPN_TYPE_SECONDARY,
	TF_WPN_TYPE_MELEE,
	TF_WPN_TYPE_GRENADE,
	TF_WPN_TYPE_BUILDING,
	TF_WPN_TYPE_PDA,
	TF_WPN_TYPE_ITEM1,
	TF_WPN_TYPE_ITEM2,
	TF_WPN_TYPE_HEAD,
	TF_WPN_TYPE_MISC,
	TF_WPN_TYPE_MELEE_ALLCLASS,
	TF_WPN_TYPE_SECONDARY2,
	TF_WPN_TYPE_PRIMARY2,

	//
	// ADD NEW ITEMS HERE TO AVOID BREAKING DEMOS
	//

	TF_WPN_TYPE_COUNT,
};

//-----------------------------------------------------------------------------
// Purpose: Slots for items within loadouts
//-----------------------------------------------------------------------------
enum loadout_positions_t
{
	LOADOUT_POSITION_INVALID = -1,

	// Weapons & Equipment
	LOADOUT_POSITION_PRIMARY = 0,
	LOADOUT_POSITION_SECONDARY,
	LOADOUT_POSITION_MELEE,
	LOADOUT_POSITION_UTILITY,
	LOADOUT_POSITION_BUILDING,
	LOADOUT_POSITION_PDA,
	LOADOUT_POSITION_PDA2,

	// Wearables. If you add new wearable slots, make sure you add them to IsWearableSlot() below this.
	LOADOUT_POSITION_HEAD,
	LOADOUT_POSITION_MISC,

	// other
	LOADOUT_POSITION_ACTION,

	// More wearables, yay!
	LOADOUT_POSITION_MISC2,

	// taunts
	LOADOUT_POSITION_TAUNT,
	LOADOUT_POSITION_TAUNT2,
	LOADOUT_POSITION_TAUNT3,
	LOADOUT_POSITION_TAUNT4,
	LOADOUT_POSITION_TAUNT5,
	LOADOUT_POSITION_TAUNT6,
	LOADOUT_POSITION_TAUNT7,
	LOADOUT_POSITION_TAUNT8,

#ifdef STAGING_ONLY
	// Extra PDA mod slots 
	LOADOUT_POSITION_PDA_ADDON1,
	LOADOUT_POSITION_PDA_ADDON2,

	LOADOUT_POSITION_PDA3,
	//LOADOUT_POSITION_MISC3,
	//LOADOUT_POSITION_MISC4,
	//LOADOUT_POSITION_MISC5,
	//LOADOUT_POSITION_MISC6,
	//LOADOUT_POSITION_MISC7,
	//LOADOUT_POSITION_MISC8,
	//LOADOUT_POSITION_MISC9,
	//LOADOUT_POSITION_MISC10,
	LOADOUT_POSITION_BUILDING2,
#endif // STAGING_ONLY

	CLASS_LOADOUT_POSITION_COUNT,
};

enum account_loadout_positions_t
{
	ACCOUNT_LOADOUT_POSITION_ACCOUNT1,
	ACCOUNT_LOADOUT_POSITION_ACCOUNT2,
	ACCOUNT_LOADOUT_POSITION_ACCOUNT3,

	ACCOUNT_LOADOUT_POSITION_COUNT,
};

// We use this to determine the maximum number of wearable instances we'll send from the server down to
// connected clients. This was previously hardcoded to be 8 and because of the way RecvPropUtlVector works
// we can't easily change this without doing some kludgy work and breaking network/demo compatibility. In
// the shorter term, we'll break compatibility in staging where no-one cares but leave public unchanged.
#ifdef STAGING_ONLY
#define	LOADOUT_MAX_WEARABLES_COUNT ( CLASS_LOADOUT_POSITION_COUNT )
#else
#define	LOADOUT_MAX_WEARABLES_COUNT ( 8 /* !!! -- LOADOUT_POSITION_COUNT - 3 */ )
#endif

inline bool IsMiscSlot(int iSlot)
{
	return iSlot == LOADOUT_POSITION_MISC
		|| iSlot == LOADOUT_POSITION_MISC2
		|| iSlot == LOADOUT_POSITION_HEAD
#ifdef STAGING_ONLY
		//|| iSlot == LOADOUT_POSITION_MISC3
		//|| iSlot == LOADOUT_POSITION_MISC4
		//|| iSlot == LOADOUT_POSITION_MISC5
		//|| iSlot == LOADOUT_POSITION_MISC6
		//|| iSlot == LOADOUT_POSITION_MISC7
		//|| iSlot == LOADOUT_POSITION_MISC8
		//|| iSlot == LOADOUT_POSITION_MISC9
		//|| iSlot == LOADOUT_POSITION_MISC10
#endif // STAGING_ONLY
		;
}

inline bool IsBuildingSlot(int iSlot)
{
	return iSlot == LOADOUT_POSITION_BUILDING
#ifdef STAGING_ONLY
		|| iSlot == LOADOUT_POSITION_BUILDING2
#endif // STAGING_ONLY
		;
}

inline bool IsTauntSlot(int iSlot)
{
	return iSlot == LOADOUT_POSITION_TAUNT
		|| iSlot == LOADOUT_POSITION_TAUNT2
		|| iSlot == LOADOUT_POSITION_TAUNT3
		|| iSlot == LOADOUT_POSITION_TAUNT4
		|| iSlot == LOADOUT_POSITION_TAUNT5
		|| iSlot == LOADOUT_POSITION_TAUNT6
		|| iSlot == LOADOUT_POSITION_TAUNT7
		|| iSlot == LOADOUT_POSITION_TAUNT8;
}

inline bool IsWearableSlot(int iSlot)
{
	return iSlot == LOADOUT_POSITION_HEAD
		|| iSlot == LOADOUT_POSITION_MISC
		|| iSlot == LOADOUT_POSITION_ACTION
		|| IsMiscSlot(iSlot)
		|| IsTauntSlot(iSlot);
}

inline bool IsQuestSlot(int iSlot)
{
	return iSlot == ACCOUNT_LOADOUT_POSITION_ACCOUNT1
		|| iSlot == ACCOUNT_LOADOUT_POSITION_ACCOUNT2
		|| iSlot == ACCOUNT_LOADOUT_POSITION_ACCOUNT3;
}

inline bool IsValidItemSlot(int iSlot)
{
	return iSlot > LOADOUT_POSITION_INVALID && iSlot < CLASS_LOADOUT_POSITION_COUNT;
}

inline bool IsValidPickupWeaponSlot(int iSlot)
{
	return iSlot == LOADOUT_POSITION_PRIMARY
		|| iSlot == LOADOUT_POSITION_SECONDARY
		|| iSlot == LOADOUT_POSITION_MELEE;
}

// The total number of loadouts to track for each player.
// Right now, hardcoded to match TF's 10 classes.
#define LOADOUT_COUNT					(10+1)	// 0th class is undefined

// Halloween! (Shared by GC and game client.)
enum EHalloweenMap
{
	kHalloweenMap_MannManor,
	kHalloweenMap_Viaduct,
	kHalloweenMap_Lakeside,
	kHalloweenMap_Hightower,

	kHalloweenMapCount
};

enum EHalloweenGiftSpawnMetaInfo
{
	kHalloweenGiftMeta_IsUnderworldOnViaduct_DEPRECATED = 0x01,
};

#endif // TFITEMCONSTANTS_H