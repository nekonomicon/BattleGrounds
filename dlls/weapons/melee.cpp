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
//Its purpose is to define the CMelee class, which will overwrite various weapon functions for melee

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"

#include "weapon_data.h"

#ifndef CLIENT_DLL
#include "gamerules.h"
#endif

#include "hulls.h"

extern int gmsgHintbox;

bool CBaseMelee::Deploy()
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + (float)pData->flDeployDelay;

	if(pData->snd_draw)
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, pData->snd_draw, 1, ATTN_NORM);

#ifdef CLIENT_DLL
	extern int g_iTeamNumber;
	extern int g_iUser1;
	if(g_iUser1 == 0)
		pev->body = g_iTeamNumber - 1;
	else {
		if(m_pPlayer->pev->team > UNDEFINED)
			pev->body = (m_pPlayer->pev->team - 1);
		else
			pev->body = 0;
	}
#else
	if(m_pPlayer->pev->team > UNDEFINED)
		pev->body = (m_pPlayer->pev->team - 1);
	else
		pev->body = 0;
#endif

	return DefaultDeploy( pData->v_model, pData->p_model, MELEE_DRAW, pData->player_anim, UseDecrement(), pev->body );
}

void CBaseMelee::PrimaryAttack( void )
{
	Fire();

#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgHintbox, NULL, m_pPlayer->pev );
        WRITE_SHORT(HINTBOX_MELEE);
    MESSAGE_END();
#endif
}

void CBaseMelee::SecondaryAttack( void )
{
	Fire();

#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgHintbox, NULL, m_pPlayer->pev );
        WRITE_SHORT(HINTBOX_MELEE2);
    MESSAGE_END();
#endif
}

void CBaseMelee::Fire()
{
	Swing();
}

void CBaseMelee::FindHullIntersection( const Vector &vecSrc, TraceResult &tr, Vector mins, Vector maxs, edict_t *pEntity )
{
	int			i = 0, j = 0, k = 0;
	float		distance = 0.0;
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

void CBaseMelee::Swing()
{
	if(m_pPlayer->pev->flags & FL_DUCKING || m_pPlayer->pev->button & IN_DUCK)
		return;
	
	//BP to prevent players from stabbing right after getting up
	if(m_fNextStabTime > UTIL_WeaponTimeBase())
		return;

	SetWeaponDamage(pData->flSecondaryDmgModifier);

	TraceResult tr;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	Vector vecSrc	= m_pPlayer->GetGunPosition( );
	Vector vecEnd	= vecSrc + gpGlobals->v_forward * this->pData->flBladeLength;

	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, ENT( m_pPlayer->pev ), &tr );

#ifndef CLIENT_DLL
	CBaseEntity *pHit = NULL;
	if ( tr.flFraction >= 1.0 )
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
	}

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
					ClearMultiDamage( );
					pHit->TraceAttack(m_pPlayer->pev, flDamage, gpGlobals->v_forward, &tr, DMG_CLUB | DMG_NEVERGIB ); 
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
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	// use event secondary so that the bayonet weapons can use there events
	// melee weapons can use the same event for primary and secondary
	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), pData->m_usSecondaryAttack, 0.0, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0, 0, m_pPlayer->pev->team, m_iId, 0, 0 );


	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + pData->flAttackDelay;
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + pData->flAttackDelay;
	m_pPlayer->BurnStamina(MAX_STAMINA / 10, (float)0.1);
}

//idle func
void CBaseMelee::WeaponIdle( void )
{
	// BP so the player can only stab after 0.5 seconds when he gets up
	if(m_pPlayer->pev->flags & FL_DUCKING || m_pPlayer->pev->button & IN_DUCK)
		m_fNextStabTime = UTIL_WeaponTimeBase() + 0.5f;
	
	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( (float)AUTOAIM_5DEGREES );

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	m_fInReload = false;

	int iAnim = 0;
	float flAnimTime = 0;
	switch(RANDOM_LONG(0, 1))
	{
	case 0:
		flAnimTime = 6.1;
		iAnim = MELEE_IDLE1;
		break;
	case 1:
		flAnimTime = 4.07;
		iAnim = MELEE_IDLE2;
		break;
	default:
		flAnimTime = 6.1;
		iAnim = MELEE_IDLE1;
		break;
	}

	if(pData->snd_idle)
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_STREAM, pData->snd_idle, 1, ATTN_NORM);

	SendWeaponAnim( iAnim, UseDecrement(), pev->body );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + flAnimTime;// how long till we do this again.
}
