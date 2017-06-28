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

/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
*/
// This file was created on the 10.2.2004 by Black Panther for the Battle Grounds modification of Half-Life
// commandermenu.cpp
// It handles commander auras and special abilities server sided.

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "nodes.h"
#include "gamerules.h"
#include "weapon_data.h"
#include "bg_defs.h"
#include "commandersmenu.h"
#include "bg_round.h"
#include <string.h>
#include <stdio.h>

extern int gmsgResComPoint;
extern globalvars_t  *gpGlobals;

float Length(const vec3_t v)
{
	int		i;
	float	length = 0.0f;
		
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrt (length);		// FIXME

	return length;
}

CBaseEntity	*UTIL_PlayerByID(char* WondID)
{
	CBaseEntity *pReturnPlayer = NULL;
	
	for(int iCount = 1; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pComparePlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);
		
		char compare[255];
		sprintf(compare,GETPLAYERAUTHID(pComparePlayer->edict()));
		
		if (!strcmp (WondID, compare))
		{
			edict_t *pPlayerEdict = INDEXENT( iCount );
			if ( pPlayerEdict && !pPlayerEdict->free )
			{
				pReturnPlayer = pComparePlayer;
			}			
			return pReturnPlayer;
		}
	}
	return false;
}

void CCommander::GetPlayersInCommanderRange(CBasePlayer *pPlayer, float Range)
{
	for (int j = 0; j <32; j++)
		for (int i = 0; i <255; i++)
			m_RushPlayerIndex.ID[j][i] = NULL;
	
	int index = 0;
	
	for(int iCount = 1; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pComparePlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pComparePlayer)
		{
			//we dont need spectators
			if(pComparePlayer->m_iTeamRespawningAs == SPECS)
				continue;

			vec3_t Distance;
			Distance[0] = pComparePlayer->pev->origin.x - pPlayer->pev->origin.x;
			Distance[1] = pComparePlayer->pev->origin.y - pPlayer->pev->origin.y;
			Distance[2] = pComparePlayer->pev->origin.z - pPlayer->pev->origin.z;

			float flDistance = Length(Distance);

			if (flDistance < Range)
			{
				strcpy(m_RushPlayerIndex.ID[index++], GETPLAYERAUTHID(pComparePlayer->edict()));		
			}
		}
	}
}

CCommander::CCommander(CBasePlayer *pPlayer)
{
	m_bRushActive = false;
	m_flRushEndTime = 0;

	m_iPointsPerSecond = 0;
	m_flTotalPoints = 0;
	m_flSpendablePoints = 0;
	
	m_iRush = 0;
	m_iArtillery = 0;
	
	m_iRush_Max = RUSH_COST;
	m_iArtillery_Max = 500; // dont forget to change

	m_bArtilleryFull = false;
	m_bRushFull = false;
	m_flLastTime = gpGlobals->time;

}

void CCommander::Init()
{
	m_bRushActive = false;
	m_flRushEndTime = 0;

	m_iPointsPerSecond = 0;
	m_flTotalPoints = 0;
	m_flSpendablePoints = 0;
	
	m_iRush = 0;
	m_iArtillery = 0;
	
	m_iRush_Max = RUSH_COST;
	m_iArtillery_Max = 500; // dont forget to change

	m_bArtilleryFull = false;
	m_bRushFull = false;
	m_flLastTime = gpGlobals->time;

}

CCommander::~CCommander()
{
	
}

void CCommander::Think(entvars_t *pev) //executed every frame
{
	int iCount = 0;
	
		// temp solution, getflags() required
	int captflagcount = 3;
	int totalflagcount = 5;
	
	if (totalflagcount < 1)	// to prevent "divided by zero" error
		totalflagcount = 1;
	
	m_iPointsPerSecond = 5 + (int)((float)captflagcount/(float)totalflagcount * 15.0f);
	m_flTotalPoints += m_iPointsPerSecond * (gpGlobals->time - m_flLastTime);
	m_flSpendablePoints += m_iPointsPerSecond * (gpGlobals->time - m_flLastTime); 
	m_flLastTime = gpGlobals->time;
	
	bool allfull = false;
	while (((int)m_flSpendablePoints > 0) && (!allfull))
	{
		if ((int)m_flSpendablePoints > 0)
		{
			if (!m_bAddedRush)
			{
				m_iRush++;
				if (m_iRush > m_iRush_Max)
					m_iRush = m_iRush_Max;
				else 
					m_flSpendablePoints--;
				m_bAddedRush = true;
			}
		}
		
		if ((int)m_flSpendablePoints > 0)
		{
			if (!m_bAddedArtillery)
			{
				m_iArtillery++;
				if (m_iArtillery > m_iArtillery_Max)
					m_iArtillery = m_iArtillery_Max;
				else 
					m_flSpendablePoints--;
				m_bAddedArtillery = true;
			}
		}	
		
		if(m_bAddedArtillery && m_bAddedRush)
		{		
				m_bAddedRush = false;
				m_bAddedArtillery = false;
		}

		if ((m_iRush == m_iRush_Max) && (m_iArtillery == m_iArtillery_Max))
		{
			allfull = true;
			m_flSpendablePoints = 0;
		}
	}

	if (m_iRush == m_iRush_Max) //ready to rush?
	{
		m_bRushFull = true;
		MESSAGE_BEGIN( MSG_ONE, gmsgResComPoint, NULL, pev );
		WRITE_SHORT(iRushFull);
		MESSAGE_END();
	}

	if (m_iArtillery == m_iArtillery_Max) //ready to strike?
	{	
		m_bArtilleryFull = true;
		MESSAGE_BEGIN( MSG_ONE, gmsgResComPoint, NULL, pev );
		WRITE_SHORT(iArtilleryFull);
		MESSAGE_END();
	}

	//if an aura has been activated make sure to end if after the proper time
	if ((m_flRushEndTime < gpGlobals->time) && m_bRushActive)
	{
		while (strcmp(m_RushPlayerIndex.ID[iCount], ""))
		{
			CBasePlayer *pEndAuraPlayer = (CBasePlayer*)UTIL_PlayerByID(m_RushPlayerIndex.ID[iCount++]);

			if(pEndAuraPlayer)
			{
				//we dont need spectators
				if(pEndAuraPlayer->m_iTeamRespawningAs == SPECS)
					continue;

				pEndAuraPlayer->m_fInRush = false;
				
				hudtextparms_t textparms;
				textparms.x = 10;
				textparms.y = 300;
				textparms.r1 = 255;
				textparms.g1 = 255;
				textparms.b1 = 255;
				textparms.r2 = 255;
				textparms.g2 = 255;
				textparms.b2 = 255;
				textparms.effect = 1;
				textparms.holdTime = 3;
				textparms.fadeoutTime = 3;
				textparms.fadeinTime = 1;
				textparms.channel = 0;		
				UTIL_HudMessage( pEndAuraPlayer, textparms, "Rush Aura ended\n" );
				
				pEndAuraPlayer->ResetSpeed();
			}
		}
	m_bRushActive = false;
	}
}

void CCommander::Rush(CBasePlayer *pCommanderPlayer)
{
//	if((strcmp(pCommanderPlayer->pClass->class_code, "light_a")) || (strcmp(pCommanderPlayer->pClass->class_code, "light_b")))
		if (pCommanderPlayer->m_bHasDied)
			return;

	if (!CCommander::Execute(iRush, pCommanderPlayer->pev))
		return;

	int iCount = 0;
	
	GetPlayersInCommanderRange(pCommanderPlayer, RUSH_AURA_RANGE);
	
	while (strcmp(m_RushPlayerIndex.ID[iCount], ""))
	{
		CBasePlayer *pRushPlayer = (CBasePlayer*)UTIL_PlayerByID(m_RushPlayerIndex.ID[iCount++]);
	
		if(pRushPlayer)
		{
 			pRushPlayer->m_fInRush = true;
			
			hudtextparms_t textparms;
			textparms.x = 10;
			textparms.y = 300;
			textparms.r1 = 255;
			textparms.g1 = 255;
			textparms.b1 = 255;
			textparms.r2 = 255;
			textparms.g2 = 255;
			textparms.b2 = 255;
			textparms.effect = 1;
			textparms.holdTime = 3;
			textparms.fadeoutTime = 3;
			textparms.fadeinTime = 1;
			textparms.channel = 0;

			UTIL_HudMessage( pRushPlayer, textparms, "Rush Aura started\n" );

			pRushPlayer->ResetSpeed();
		}
	}
	m_flRushEndTime = gpGlobals->time + RUSH_DURATION;
	m_bRushActive = true;
}

int CCommander::Execute(int special, entvars_t* pevToBeNotified)
{
	if ((special == iRush) && (m_bRushFull))
	{
		m_iRush = 0;
		m_bRushFull = false;
		
		//tell the client
		MESSAGE_BEGIN( MSG_ONE, gmsgResComPoint, NULL, pevToBeNotified );
		WRITE_SHORT(iRush);
		MESSAGE_END();
		
		return 1;
	}
	
	if ((special == iArtillery) && (m_bArtilleryFull))
	{
		m_iArtillery = 0;
		m_bArtilleryFull = false;

		//tell the client
		MESSAGE_BEGIN( MSG_ONE, gmsgResComPoint, NULL, pevToBeNotified );
		WRITE_SHORT(iArtillery);
		MESSAGE_END();

		return 1;
	}
	
	return 0;
}

/*
todo

  send clientcommand if local specialpoints are full
  wait for server->client command to reset points





*/
