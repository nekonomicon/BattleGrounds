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

//This file was creatd on the 22/9/2K+2 at 16:05 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to define the CBaseWeapon class which will be the basics for all bg weaponry

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "weapon_data.h"

// Raiko Contrib - Stationary Reload	
bool inReload = false;
// Raiko End

//sets every variable to its default value
CWeaponData::CWeaponData()
{
	//models
	p_model = NULL;
	v_model = NULL;
	w_model = NULL;
	player_anim = "musket";
	bSwitchBodies = false;

	//sounds
	snd_prim_fire = NULL;
	snd_second_fire = NULL;
	snd_reload = NULL;
	snd_draw = NULL;
	snd_holster = NULL;
	snd_idle = NULL;
	snd_hit = NULL;
	snd_hitwall = NULL;

	//basic defines
	classname = MAKE_STRING("weapon_none");
	bBayonet = false;
	bUsesSecondaryFire = false;
	iId = WEAPON_NONE;	//things like this one are fairly unnessicary, but it is good to keep to the overall design
	iWeight = 20;		//a mediumish weight atm
	iWeaponType = GUN;	
	flSpeedModifier = 0.0;

	//event vars
	event_prim = "events/gun_fire.sc";
	event_second = "events/gun_fire.sc";
	event_anim = "events/animation.sc";
	m_usPrimaryAttack = 0;
	m_usSecondaryAttack = 0;
	m_usAnimation = 0;

	//delays
	flReloadTime = 7.5;
	flAttackDelay = 1.0;
	flUnsuccesfulAttackDelay = 0.2;
	flTimeToIdle = 0.0;
	flDeployDelay = 1.0;

	//damage
	flPrimaryDmgModifier = 5.0;
	flSecondaryDmgModifier = 5.0;

	//ammo info
	iMaxClip = GUN_MAX_CLIP;
	iDefaultGive = 25;
	iMaxAmmo = 45;
	ammo_name = "MusketShots";

	//accuracy
	flStandMoving = 16.0;
	flStandStill = 7.0;
	flCrouchMoving = 14.0;
	flCrouchStill = 5.0;

	//staminas
	flReloadStamina = 0;
	flShootStamina = MAX_STAMINA / 6;
}

void CBaseWeapon::Spawn()
{
	if(pData == NULL)
	{
		pData = new CWeaponData;
		SetAllVars();
	}

	pev->classname = pData->classname; // hack to allow for old names
	Precache();

	SET_MODEL(ENT(pev), pData->w_model);

	m_iId = pData->iId;
	m_iDefaultAmmo = pData->iDefaultGive;

	FallInit();// get ready to fall down.
}

void CBaseWeapon::Precache()
{
	if(pData == NULL)
	{
		pData = new CWeaponData;
		SetAllVars();
	}

	if(pData->p_model != NULL)
		PRECACHE_MODEL(pData->p_model);

	if(pData->v_model != NULL)
		PRECACHE_MODEL(pData->v_model);

	if(pData->w_model != NULL)
		PRECACHE_MODEL(pData->w_model);

	if(pData->event_prim != NULL)
		pData->m_usPrimaryAttack = PRECACHE_EVENT(1, pData->event_prim);

	if(pData->event_second != NULL)
		pData->m_usSecondaryAttack = PRECACHE_EVENT(1, pData->event_second);

	if(pData->event_anim != NULL)
		pData->m_usAnimation = PRECACHE_EVENT(1, pData->event_anim);

	if(pData->snd_prim_fire != NULL)
		PRECACHE_SOUND(pData->snd_prim_fire);

	if(pData->snd_second_fire != NULL)
		PRECACHE_SOUND(pData->snd_second_fire);

	if(pData->snd_reload != NULL)
		PRECACHE_SOUND(pData->snd_reload);

	if(pData->snd_idle != NULL)
		PRECACHE_SOUND(pData->snd_idle);

	if(pData->snd_holster != NULL)
		PRECACHE_SOUND(pData->snd_holster);

	if(pData->snd_draw != NULL)
		PRECACHE_SOUND(pData->snd_draw);

	if(pData->snd_hit != NULL)
		PRECACHE_SOUND(pData->snd_hit);

	if(pData->snd_hitwall != NULL)
		PRECACHE_SOUND(pData->snd_hitwall);
}

int CBaseWeapon::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pData->classname);
	p->pszAmmo1 = pData->ammo_name;
	p->iMaxAmmo1 = pData->iMaxAmmo;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = pData->iMaxClip;
	p->iSlot = pData->iWeaponType - 1;
	p->iPosition = pData->iId;
	p->iFlags = ITEM_FLAG_EXHAUSTIBLE | ITEM_FLAG_NOAUTORELOAD;
	p->iId = m_iId = pData->iId;
	p->iWeight = pData->iWeight;

	return 1;
}

int CBaseWeapon::iItemSlot()
{
	return pData->iWeaponType - 1;
}

//adds this weapon to the player
int CBaseWeapon::AddToPlayer( CBasePlayer *pPlayer )
{
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
		WRITE_BYTE( pData->iId );
		MESSAGE_END();

		// set the body is we are using multi sub model weapons
		if(pData->bSwitchBodies == true)
		{
			pev->body = (m_pPlayer->pev->team - 1);
		}

		return true;
	}
	return false;
}

//deploys the gun
bool CBaseWeapon::Deploy( )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + (float)pData->flDeployDelay;

	if(pData->snd_draw)
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_STREAM, pData->snd_draw, 1, ATTN_NORM);

	// check for whether gun is full or empty and play appropriate draw anim
	int iAnim = BASE_DRAW;
	if(m_iClip <= 0)
		iAnim++;

	return DefaultDeploy( pData->v_model, pData->p_model, iAnim, pData->player_anim, UseDecrement(), pev->body );
}

//holsters the guns
void CBaseWeapon::Holster( int skiplocal )
{
	if(m_fInReload == true)
	{
		m_iClip = 0;
		m_fInReload = false;// cancel any reload in progress.
	}

	if(pData->snd_holster)
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_STREAM	, pData->snd_holster, 1, ATTN_NORM);
}

//fire function. handles firing for both primary and seconary
void CBaseWeapon::Fire(bool bIsPrimary)
{
	// RAIKO Contrib - Melee Only
#ifdef CLIENT_DLL
	extern int g_iMeleeOnly;
	if(g_iMeleeOnly == 1)
#else
	if(CVAR_GET_FLOAT("mp_meleeonly") == 1.0)
#endif
	{
		// Ben Addition to Raiko Contrib - Melee Only
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		// Ben End
		return;
	}
	// RAIKO Contrib End

	if(bIsPrimary)
		SetWeaponDamage(pData->flPrimaryDmgModifier);
	else
		SetWeaponDamage(pData->flSecondaryDmgModifier);

	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		return;
	}

	if (m_iClip <= 0)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (float)pData->flUnsuccesfulAttackDelay;
		return;
	}


	//this reverses any hack I used to make the bayonet kill sprite work
	pev->classname = pData->classname;

	m_iClip--;

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector vecSrc = m_pPlayer->GetGunPosition( );
	Vector vecAccuracy, vecDir, vecAiming = Vector(0, 0, 0);

	SetAimVars(vecAiming, vecAccuracy);

	vecDir = m_pPlayer->FireBulletsPlayer( 1, vecSrc, vecAiming, vecAccuracy, 8192, BULLET_SHOT, 2, 0, m_pPlayer->pev, pev, m_pPlayer->random_seed );
	
	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	unsigned short m_usFire = (bIsPrimary ? pData->m_usPrimaryAttack : pData->m_usSecondaryAttack);
	int iIsPrimary = (bIsPrimary ? 1 : 0);

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, iIsPrimary, m_iId, 0, 0 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + pData->flAttackDelay;
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + pData->flAttackDelay;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + pData->flReloadTime;

	m_pPlayer->BurnStamina( (m_pPlayer->pev->fuser4 > pData->flShootStamina ) ? pData->flShootStamina : m_pPlayer->pev->fuser4, (float)0.1);
}

//normal firing mode
void CBaseWeapon::PrimaryAttack()
{
	Fire(true);
}

// secondary fire. usually disabled. check for special seocndary fire cases here
void CBaseWeapon::SecondaryAttack()
{
	if(pData->bUsesSecondaryFire == false)
	{
		return;
	}
	Fire(false);
}

//relaod func
void CBaseWeapon::Reload( void )
{
	if (m_pPlayer->pev->waterlevel == 3)
		return;

	if((pData->iWeaponType != GUN) && (pData->iWeaponType != PISTOL))
		return;

	if(m_fInReload == true)
		return;

	if ( m_iClip > 0 )
		return;

	bool bSucceded = DefaultReload( pData->iMaxClip, BASE_RELOAD, (float)pData->flReloadTime, pev->body );

	//Raiko Contrib - Stationary Reload	
	inReload = m_fInReload;
	// Raiko End

	if(bSucceded == false)
		return;

	if(pData->snd_reload)
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_STREAM, pData->snd_reload, 1, ATTN_NORM);

	m_pPlayer->SetAnimation( PLAYER_RELOAD );

	m_pPlayer->BurnStamina( ((m_pPlayer->pev->fuser4 > pData->flReloadStamina) ? pData->flReloadStamina : m_pPlayer->pev->fuser4), (float)0.1); 
}

//idle func
void CBaseWeapon::WeaponIdle( void )
{
	ResetEmptySound( );

	// BP so the player can only stab after 0.5 seconds when he gets up
	if(m_pPlayer->pev->flags & FL_DUCKING || m_pPlayer->pev->button & IN_DUCK)
		m_fNextStabTime = UTIL_WeaponTimeBase() + 0.5f;

	m_pPlayer->GetAutoaimVector( (float)AUTOAIM_5DEGREES );

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	m_fInReload = false;

	int iAnim;
	switch(RANDOM_LONG(0, 1))
	{
	case 0:
		iAnim = BASE_IDLE1;
		break;
	case 1:
		iAnim = BASE_IDLE2;
		break;
	default:
		iAnim = BASE_IDLE1;
		break;
	}

	// use the empty anim instead 
	if(m_iClip <= 0)
		iAnim++;

	if(pData->snd_idle)
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_STREAM, pData->snd_idle, 1, ATTN_NORM);

	SendWeaponAnim( iAnim, UseDecrement(), pev->body );

	float flAnimTime = (pData->flTimeToIdle == 0.0 ? 3.1 : pData->flTimeToIdle);
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + flAnimTime;// how long till we do this again.
}

//sets the accuracy and aiming cone depending on the players movement and crouching status
void CBaseWeapon::SetAimVars(Vector &vecAiming, Vector &vecAccuracy)
{
	//moving
	if(!FBitSet( m_pPlayer->pev->flags, FL_ONGROUND ) || m_pPlayer->CheckMoving() == true)
	{
		//crouching
		if(m_pPlayer->pev->flags & FL_DUCKING)
		{
			vecAiming = m_pPlayer->GetAutoaimVector((float)AUTOAIM_5DEGREES);
			vecAccuracy = Cone(pData->flCrouchMoving);
		}
		//standing
		else
		{
			vecAiming = m_pPlayer->GetAutoaimVector((float)AUTOAIM_5DEGREES);
			vecAccuracy = Cone(pData->flStandMoving);
		}
	}
	//still
	else
	{
		//crouching
		if(m_pPlayer->pev->flags & FL_DUCKING)
		{
			vecAiming = m_pPlayer->GetAutoaimVector((float)AUTOAIM_2DEGREES);
			vecAccuracy = Cone(pData->flCrouchStill);
		}
		//standing
		else
		{
			vecAiming = m_pPlayer->GetAutoaimVector((float)AUTOAIM_2DEGREES);
			vecAccuracy = Cone(pData->flStandStill);
		}
	}
}

//works out the accuracy cone, so we can use cones of varying sizes
Vector CBaseWeapon::Cone(float flConeSize)
{
	const double flBaseSize = 0.008725;
	float flCone = (float)flBaseSize * flConeSize;

	return Vector(flCone, flCone, flCone);
}
