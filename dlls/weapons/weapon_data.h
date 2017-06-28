/*
    Copyright 2001 to 2004. The Battle Grounds Team and Contributors

    This file is part of the Battle Grounds Modification for Half-Life.

    The Battle Grounds Modification for Half-Life is free software;
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser General Public License as published by the Free
    Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    The Battle Grounds Modification for Half-Life is distributed in
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with The Battle Grounds Modification for Half-Life;
    if not, write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA  02111-1307  USA

    You must obey the GNU Lesser General Public License in all respects for
    all of the code used other than code distributed with the Half-Life
    SDK developed by Valve.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.
*/

// This file was created on the 22/9/2K+2 at 16:11 by Ben Banfield for the Battle Grounds modification of Half-Life
// Its purpose is to define the CWeaponData class

#ifndef WEAPON_DATA_H
#define WEAPON_DATA_H

// damage values for each attack on each weapon
const float BESS_FIRE_DAMAGE = 60.0;
const float BESS_BAYONET_DAMAGE = 43.0;

const float REVOL_FIRE_DAMAGE = 37.0;
const float REVOL_BAYONET_DAMAGE = 40.0;

const float CHARLE_FIRE_DAMAGE = 57.5;
const float CHARLE_BAYONET_DAMAGE = 41.0;

const float PENNY_FIRE_DAMAGE = 35.0;

const float PISTOLA_FIRE_DAMAGE = 40.0;
const float PISTOLB_FIRE_DAMAGE = 40.0;

const float KNIFE_DAMAGE = 30.0;
const float SABRE_DAMAGE = 38.0;

//speed modifiers
const float BESS_SPEED = 25;
const float REVOL_SPEED = 35;
const float CHARLE_SPEED = 23;
const float PENNY_SPEED = 30;
const float PISTOLA_SPEED = 15;
const float PISTOLB_SPEED = 15;
const float KNIFE_SPEED = 35;
const float SABRE_SPEED = 0;

//Weapon types
const int GUN = 1; // musket/rifle - there is probably some better name for this
const int PISTOL = 1;
const int MELEE = 2;

// hitgroup modifiers
const double HIT_GENERIC_DMG = 3.0;
const double HIT_HEAD_DMG = 3.4;
const double HIT_CHEST_DMG = 1.85;
const double HIT_STOMACH_DMG = 1.75;
const double HIT_ARM_DMG = 1.25;
const double HIT_LEG_DMG = 1.35;


//weapons data class
class CWeaponData
{
public:
	CWeaponData();
	//models
	char *p_model;
	char *v_model;
	char *w_model;
	char *player_anim;
	bool bSwitchBodies;

	//sounds
	char *snd_prim_fire;
	char *snd_second_fire;
	char *snd_reload;
	char *snd_draw;
	char *snd_holster;
	char *snd_idle;
	char *snd_hit;
	char *snd_hitwall;

	//basic defines
	string_t classname;
	bool bUsesSecondaryFire;
	int iId;
	int iWeight;
	int iWeaponType; //new thing for the hud. 1 main gun, 2 pistol, 3  sabre/knife/tomahawk, 4 other (maybe scope in the future)
	int iWeaponSlot;
	float flSpeedModifier;

	//event vars
	char *event_prim;
	char *event_second;
	char *event_anim;
	unsigned short m_usPrimaryAttack;
	unsigned short m_usSecondaryAttack;
	unsigned short m_usAnimation;

	//delays
	double flReloadTime;
	float flAttackDelay;
	double flUnsuccesfulAttackDelay;
	float flTimeToIdle;
	double flDeployDelay;

	//damage
	float flPrimaryDmgModifier;
	float flSecondaryDmgModifier;

	//ammo info
	int iMaxClip;
	int iDefaultGive;
	int iMaxAmmo;
	char *ammo_name;

	//accuracy cone sizes
	float flCrouchMoving;
	float flCrouchStill;
	float flStandMoving;
	float flStandStill;

	//staminas
	float flReloadStamina;
	float flShootStamina;

	//melee extras
	float flBladeLength;
	bool bBayonet;
};
//ben - that was more than expected, but quite a lot won't be changed from the base weapon I think.


// sets the order of anims
// this should be the order for all guns
enum baseweapon_e
{
	BASE_IDLE1 = 0,
	BASE_IDLE1_EMPTY = 1,
	BASE_IDLE2 = 2,
	BASE_IDLE2_EMPTY = 3,
	BASE_DRAW = 4,
	BASE_DRAW_EMPTY = 5,
	BASE_HOLSTER = 6,
	BASE_RELOAD = 7,
	BASE_SHOOT1 = 8,
	BASE_SHOOT2 = 9,
	BASE_STAB = 10,
	BASE_STAB_EMPTY = 11
};

// melee order
enum melee_e {
	MELEE_IDLE1 = 0,
	MELEE_IDLE2 = 1,
	MELEE_DRAW = 2,
	MELEE_HOLSTER = 3,
	MELEE_ATTACK1 = 4,
	MELEE_ATTACK2 = 5,
	MELEE_ATTACK3 = 6,
	MELEE_ATTACK4 = 7
};

// hack to stop client dll files from accessing this if they aren't meant to
#ifndef WEAPON_CLASS_NO

// weapon class definitions using the new system

//base weapon class, from which all other weapons will inherit
class CBaseWeapon : public CBasePlayerWeapon
{
public:
	virtual void Fire (bool bIsPrimary);
	virtual void SetAimVars(Vector &vecAiming, Vector &vecAccuracy);

	virtual void Spawn( void );
	virtual void Precache( void );
	virtual int iItemSlot( void );
	virtual int GetItemInfo(ItemInfo *p);
	virtual int AddToPlayer( CBasePlayer *pPlayer );
	virtual void PrimaryAttack( void );
	virtual void SecondaryAttack( void );
	virtual bool Deploy( void );
	virtual void Holster( int skiplocal = 0 );
	virtual void Reload( void );
	virtual void WeaponIdle( void );

	// we don't want memory leaks
	virtual void Drop( void ) { Holster(); delete pData; CBasePlayerItem::Drop(); }
	virtual void Kill( void ) { Holster(); delete pData; CBasePlayerItem::Kill(); }

	virtual Vector Cone( float flConeSize );

	virtual bool UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return true;
#else
		return false;
#endif
	}

	virtual void SetAllVars( void ) { };
};

// bayonet middle-man class
class CBaseBayonet : public CBaseWeapon
{
public:
	virtual void SecondaryAttack( void );
	void FindHullIntersection( const Vector &vecSrc, TraceResult &tr, Vector mins, Vector maxs, edict_t *pEntity );
};
// brown bess
class CBrownBess : public CBaseBayonet
{
public:
	virtual void SetAllVars();
};

// charleville
class CCharleville : public CBaseBayonet
{
public:
	virtual void SetAllVars();
};

// pennsylvania
class CPennsylvania : public CBaseWeapon
{
public:
	virtual void SetAllVars();
};

// revolutionnaire
class CRevolutionnaire : public CBaseBayonet
{
public:
	virtual void SetAllVars();
};

// american pistol
class CPistolA : public CBaseWeapon
{
public:
	virtual void SetAllVars();
};

// british pistol
class CPistolB : public CBaseWeapon
{
public:
	virtual void SetAllVars();
};

// base melee weapon
class CBaseMelee : public CBaseWeapon
{
public:
	virtual bool Deploy( void );
	virtual void PrimaryAttack( void );
	virtual void SecondaryAttack( void );
	virtual void Fire( void );
	virtual void WeaponIdle( void );

	virtual void Swing();
	void FindHullIntersection( const Vector &vecSrc, TraceResult &tr, Vector mins, Vector maxs, edict_t *pEntity );
};

// knife weapon
class CKnife : public CBaseMelee
{
public:
	virtual void SetAllVars();
};

// sabre weapon
class CSabre : public CBaseMelee
{
public:
	virtual void SetAllVars();
};

#endif

#endif
