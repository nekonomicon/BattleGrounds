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
//Its purpose is to define the CBaseBayonet class which allows other guns to have their secondary attack as a bayonet

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "weapon_data.h"
#include "gamerules.h"
#include "hulls.h"

void CBaseBayonet::FindHullIntersection( const Vector &vecSrc, TraceResult &tr, Vector mins, Vector maxs, edict_t *pEntity )
{
	int			i, j, k;
	float		distance;
	float		*minmaxs[2] = {mins, maxs};
	TraceResult tmpTrace;
	Vector		vecHullEnd = tr.vecEndPos;
	Vector		vecEnd;

	distance = 1e6f;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
	UTIL_TraceLine( vecSrc, vecHullEnd, dont_ignore_monsters, pEntity, &tmpTrace );
	if ( tmpTrace.flFraction < 1.0 )
	{
		tr = tmpTrace;
		return;
	}

	for ( i = 0; i < 2; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			for ( k = 0; k < 2; k++ )
			{
				vecEnd.x = vecHullEnd.x + minmaxs[i][0];
				vecEnd.y = vecHullEnd.y + minmaxs[j][1];
				vecEnd.z = vecHullEnd.z + minmaxs[k][2];

				UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, pEntity, &tmpTrace );
				if ( tmpTrace.flFraction < 1.0 )
				{
					float thisDistance = (tmpTrace.vecEndPos - vecSrc).Length();
					if ( thisDistance < distance )
					{
						tr = tmpTrace;
						distance = thisDistance;
					}
				}
			}
		}
	}
}

// bayonet attack
void CBaseBayonet::SecondaryAttack( void )
{
	if(m_pPlayer->pev->flags & FL_DUCKING || m_pPlayer->pev->button & IN_DUCK)
		return;

	//BP to prevent players from stabbing right after getting up
	if(m_fNextStabTime > UTIL_WeaponTimeBase())
		return;

	SetWeaponDamage(pData->flSecondaryDmgModifier);

	TraceResult tr;


	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	Vector vecSrc	= m_pPlayer->GetGunPosition();
	Vector vecEnd	= vecSrc + gpGlobals->v_forward * this->pData->flBladeLength;

	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, ENT( m_pPlayer->pev ), &tr );

#ifndef CLIENT_DLL
	CBaseEntity *pHit = NULL;
/*	if ( tr.flFraction >= 1.0 )
	{
		UTIL_TraceHull( vecSrc, vecEnd, dont_ignore_monsters, head_hull, ENT( m_pPlayer->pev ), &tr );
		if ( tr.flFraction < 1.0 )
		{
			// Calculate the point of intersection of the line (or hull) and the object we hit
			// This is and approximation of the "best" intersection
			pHit = CBaseEntity::Instance( tr.pHit );
			if ( !pHit || pHit->IsBSPModel() )
			{
				FindHullIntersection( vecSrc, tr, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, m_pPlayer->edict() );
			}
			vecEnd = tr.vecEndPos;	// This is the point on the actual surface (the hull could have hit space)
		}
	}*/

	if(tr.flFraction < 1.0)
	{
		pHit = CBaseEntity::Instance( tr.pHit );

		if (pHit && pHit->pev->takedamage)
		{
			float flDamage = m_pPlayer->GetDamage(m_pPlayer->pev, gpGlobals->v_forward, &tr);

			if(pHit->IsPlayer())
			{
				Vector vecOrg = tr.vecEndPos - gpGlobals->v_forward * 4;

				if ( g_pGameRules->FPlayerCanTakeDamage( m_pPlayer, pHit ) && flDamage != 0.0)
				{
					pev->classname = MAKE_STRING("weapon_Bayonet");
					ClearMultiDamage( );
					pHit->TraceAttack(m_pPlayer->pev, flDamage, gpGlobals->v_forward, &tr, DMG_CLUB | DMG_NEVERGIB ); 
					
					pev->iuser3 = tr.iHitgroup; // hack to get the hitgroup used

					ApplyMultiDamage( pev, m_pPlayer->pev );
					EMIT_SOUND(ENT(pev), CHAN_WEAPON, pData->snd_hit, 1, ATTN_NORM);
					SpawnBlood( vecOrg, BLOOD_COLOR_RED, flDamage * 4 ); // Make a lot of Blood!
				}
			}
			else
			{
				ClearMultiDamage( );
				pHit->TraceAttack(m_pPlayer->pev, flDamage, gpGlobals->v_forward, &tr, DMG_CLUB | DMG_NEVERGIB ); 
				ApplyMultiDamage( pev, m_pPlayer->pev );
			}
		}
		else
		{
			if(pData->snd_hitwall)
				EMIT_SOUND(ENT(pev), CHAN_AUTO, pData->snd_hitwall, 1, ATTN_NORM);
	
			DecalGunshot( &tr, BULLET_BLADE );
		}
	}

#endif
	m_pPlayer->SetAnimation( PLAYER_BAYONET );

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif
	int iIsEmpty = (m_iClip > 0 ? 0 : 1);

	// use event secondary so that the bayonet weapons can use there events
	// melee weapons can use the same event for primary and secondary
	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), pData->m_usSecondaryAttack, 0.0, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0, 0, m_pPlayer->pev->team, m_iId, iIsEmpty, 0 );
	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + pData->flAttackDelay;
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + pData->flAttackDelay;
	m_pPlayer->BurnStamina(MAX_STAMINA / 5, (float)0.1);
}
