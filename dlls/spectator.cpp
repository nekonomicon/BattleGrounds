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
*	Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
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
// CBaseSpectator

// YWB:  UNDONE

// Spectator functions
// 
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"player.h"
#include	"gamerules.h"
#include	"bg_rules.h"
#include	"spectator.h"
#include	"pm_shared.h"

/*
===========
SpectatorConnect

called when a spectator connects to a server
============
*/
void CBaseSpectator::SpectatorConnect(void)
{
	pev->flags |= (FL_SPECTATOR | FL_PROXY);
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NOCLIP;
	pev->effects |= EF_NODRAW;
	
	m_pGoalEnt = NULL;

	m_bHasSpawnedBefore = false;
	g_pBGRules->HandleTeams(GetClassPtr((CBasePlayer*)pev), SPECS);
	pev->team = SPECS;
}

/*
===========
SpectatorDisconnect

called when a spectator disconnects from a server
============
*/
void CBaseSpectator::SpectatorDisconnect(void)
{
}

/*
================
SpectatorImpulseCommand

Called by SpectatorThink if the spectator entered an impulse
================
*/
void CBaseSpectator::SpectatorImpulseCommand(void)
{
	static edict_t	*pGoal		= NULL;
	edict_t         *pPreviousGoal;
	edict_t         *pCurrentGoal;
	bool			bFound;
	bool			loop = true;
	
	switch (pev->impulse)
	{
	case 1:
		// teleport the spectator to the next spawn point; note that if the spectator is
		// tracking, this doesn't do much
		pPreviousGoal = pGoal;
		pCurrentGoal  = pGoal;
		// Start at the current goal, skip the world, and stop if we looped back around

		bFound = false;
		while (loop)
		{
			pCurrentGoal = FIND_ENTITY_BY_CLASSNAME(pCurrentGoal, "info_player_spectator");
			// Looped around, failure
			if (pCurrentGoal == pPreviousGoal)
			{
				ALERT(at_console, "Could not find a spawn spot.\n");
				break;
			}
			// Found a non-world entity, set success, otherwise, look for the next one.
			if (!FNullEnt(pCurrentGoal))
			{
				bFound = true;
				break;
			}
		}

		if ( !bFound )  // Didn't find a good spot.
			break;
		
		pGoal = pCurrentGoal;
		UTIL_SetOrigin( pev, pGoal->v.origin );
		pev->angles = pGoal->v.angles;
		pev->fixangle = false;
		break;
	default:
		ALERT(at_console, "Unknown spectator impulse\n");
		break;
	}

	pev->impulse = 0;
}

/*
================
SpectatorThink

Called every frame after physics are run
================
*/
void  CBaseSpectator::SpectatorThink(void)
{
	if (!(pev->flags & FL_SPECTATOR))
	{
		pev->flags |= (FL_SPECTATOR | FL_PROXY);
	}

	pev->solid	   = SOLID_NOT;
	pev->movetype  = MOVETYPE_NOCLIP;

	if (pev->impulse)
		SpectatorImpulseCommand();
}

/*
===========
Spawn

  Called when spectator is initialized:
  UNDONE:  Is this actually being called because spectators are not allocated in normal fashion?
============
*/
void CBaseSpectator::Spawn()
{
	pev->flags |= (FL_SPECTATOR | FL_PROXY);
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NOCLIP;
	
	m_pGoalEnt = NULL;
}
