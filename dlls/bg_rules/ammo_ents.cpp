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

//This file was created on the 15/6/03 at 17:18 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is to define all entities relating to ammo

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "animation.h"

#include "gamerules.h"
#include "bg_rules.h"

#include "weapon_data.h"

class CAmmoStrip : public CBaseEntity
{
	void Spawn( void );
	void KeyValue( KeyValueData *pKeyValueData );
	void EXPORT AmmoStrip_Touch( CBaseEntity *pEntity );

	int m_iAmmoToLose;
	int m_iTimeToLoseTheAmmo;
};

// set the entity up
void CAmmoStrip::Spawn( void )
{
	// - make and invisble box which will get triggered on entry
	pev->solid = SOLID_TRIGGER;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL (ENT(pev), STRING(pev->model));
	pev->effects |= EF_NODRAW;
	
	UTIL_SetOrigin( pev, pev->origin ); 		
	UTIL_SetSize( pev, pev->absmin, pev->absmax );

	SetTouch( &CAmmoStrip::AmmoStrip_Touch );
}

// sets the params made by the mapper
void CAmmoStrip::KeyValue( KeyValueData *pKeyValueData )
{
	if(FStrEq(pKeyValueData->szKeyName, "ammo_losing"))
	{
		m_iAmmoToLose = atoi(pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "time_losing"))
	{
		m_iTimeToLoseTheAmmo = atoi(pKeyValueData->szValue);
	}
	// - sends anything else off to get handled by CBaseEntity
	else
	{
		CBaseEntity::KeyValue( pKeyValueData );
	}
}

// remove the ammo on touching
void CAmmoStrip::AmmoStrip_Touch( CBaseEntity *pEntity )
{
	if(!pEntity->IsPlayer())
		return;

	CBasePlayer *pPlayer = GetClassPtr((CBasePlayer*)pEntity->pev);

	if(pPlayer->m_flLastAmmoStripTime == 0.0 || (pPlayer->m_flLastAmmoStripTime + m_iTimeToLoseTheAmmo) <= gpGlobals->time)
	{
		int iAmmoLost = pPlayer->AmmoInventory(pPlayer->GetAmmoIndex("MusketShots")) + 1;

		if(iAmmoLost > 0)
		{
			if(m_iAmmoToLose != 0)
			{
				iAmmoLost = m_iAmmoToLose;

				if((pPlayer->AmmoInventory(pPlayer->GetAmmoIndex("MusketShots")) - m_iAmmoToLose) < 0)
				{
					iAmmoLost = pPlayer->AmmoInventory(pPlayer->GetAmmoIndex("MusketShots"));
				}
			}

			if(iAmmoLost >= 0)
				iAmmoLost -= (2 * iAmmoLost);

			pPlayer->GiveAmmo(iAmmoLost, "MusketShots", pPlayer->AmmoInventory(pPlayer->GetAmmoIndex("MusketShots")) + 1);
			for(int i = 0; i <= MAX_ITEM_TYPES; i++)
			{
				if(pPlayer->m_rgpPlayerItems[i])
				{
					CBaseWeapon *pWeapon = GetClassPtr((CBaseWeapon*)pPlayer->m_rgpPlayerItems[i]->pev);
					if(pWeapon)
					{
						if(pPlayer->AmmoInventory(pPlayer->GetAmmoIndex("MusketShots")) == 0)
						{
							pWeapon->m_iClip = 0;
							pWeapon->m_iClientClip = 0;
						}
					}
				}
			}
		}
		pPlayer->m_flLastAmmoStripTime = gpGlobals->time;
	}
}

LINK_ENTITY_TO_CLASS( func_ammostripper, CAmmoStrip );

class CAmmoProvider : public CBaseEntity
{
public:
	void Spawn( void );
	void KeyValue( KeyValueData *pKeyValueData );
	void EXPORT AmmoProvide_Touch( CBaseEntity *pEntity );

private:
	int m_iAmmoToGive;
	float m_flTimeDelayBetweenGives;
};

// set the entity up
void CAmmoProvider::Spawn( void )
{
	Precache();

	pev->solid = SOLID_TRIGGER;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL (ENT(pev), STRING(pev->model));
	pev->effects |= EF_NODRAW;
	
	UTIL_SetOrigin( pev, pev->origin ); 		
	UTIL_SetSize( pev, pev->absmin, pev->absmax );

	SetTouch( &CAmmoProvider::AmmoProvide_Touch );
}

// sets the params made by the mapper
void CAmmoProvider::KeyValue( KeyValueData *pKeyValueData )
{
	if(FStrEq(pKeyValueData->szKeyName, "ammo_given"))
	{
		m_iAmmoToGive = atoi(pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "time_delay"))
	{
		m_flTimeDelayBetweenGives = atoi(pKeyValueData->szValue);
	}
	// - sends anything else off to get handled by CBaseEntity
	else
	{
		CBaseEntity::KeyValue( pKeyValueData );
	}
}

// remove the ammo on touching
void CAmmoProvider::AmmoProvide_Touch( CBaseEntity *pEntity )
{
	if(!pEntity->IsPlayer())
		return;

	CBasePlayer *pPlayer = GetClassPtr((CBasePlayer*)pEntity->pev);

	if(pPlayer->m_flLastAmmoGiveTime == 0.0 || (pPlayer->m_flLastAmmoGiveTime + m_flTimeDelayBetweenGives) <= gpGlobals->time)
	{
		int i = 0;
		if(pPlayer->m_rgpPlayerItems[i])
		{
			CBaseWeapon *pWeapon = GetClassPtr((CBaseWeapon*)pPlayer->m_rgpPlayerItems[i]->pev);
			if(pWeapon)
			{
				int iMaxAmmo = pWeapon->iMaxAmmo1();
				int iCurrent = pPlayer->AmmoInventory(pPlayer->GetAmmoIndex("MusketShots"));

				// give all ammo
				if(m_iAmmoToGive == 0)
				{
					pPlayer->GiveAmmo((iMaxAmmo - iCurrent), "MusketShots", iMaxAmmo + 1);
				}
				else
				{
					int iAmmo = 0;
					if((iCurrent + m_iAmmoToGive) > iMaxAmmo)
						iAmmo = (iMaxAmmo - iCurrent);

					else
						iAmmo = m_iAmmoToGive;

					pPlayer->GiveAmmo(iAmmo, "MusketShots", iMaxAmmo +1);
				}

				// only give a new clip straight off if we have ammo currently
				if(	iCurrent == 0)
				{
					pWeapon->m_iClip = 1;
					pWeapon->m_iClientClip = 1;
				}
			}
		}
		pPlayer->m_flLastAmmoGiveTime = gpGlobals->time;
	}
}

LINK_ENTITY_TO_CLASS( func_ammoprovider, CAmmoProvider );
