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

//This file was created on the 13/6/03 at 14:12 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is to define the point manager class which will handle all points

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

#include "point.h"

// create a point manager object
CPointManager::CPointManager( void )
{
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		m_apPoints[i] = new point_s;
		delete m_apPoints[i];
		m_apPoints[i] = NULL;
	}
	SetHasAnyTeamWon( false );
	SetTeamWon( EITHER );
}

// remove a point manager object
CPointManager::~CPointManager( void )
{
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		delete m_apPoints[i];
		m_apPoints[i] = NULL;
	}
	SetHasAnyTeamWon( false );
	SetTeamWon( EITHER );
}

// adds a point to the list
int CPointManager::AddPoint( CBasePoint *pPoint )
{
	if(!pPoint)
		return -1;

	int i = 0;
	float flOldestPoint = 0.0;
	// -check the point isn't already in the list
	for(i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = m_apPoints[i];
		if(!point)
			continue;
		
		if(point->pPoint == NULL || point->flTimeAdded == 0.0)
			continue;

		if(point->pPoint == pPoint)
			return i;

		// --record the oldest time, so we can find that point later
		if(point->flTimeAdded != 0.0 && point->flTimeAdded < flOldestPoint)
			flOldestPoint = point->flTimeAdded;
	}
	
	// -add the point into an empty gap
	for(i = 0; i <= MAX_POINTS; i++)
	{
		if(m_apPoints[i] == NULL)
		{
			m_apPoints[i] = new point_s;
			m_apPoints[i]->pPoint = pPoint;
			m_apPoints[i]->flTimeAdded = gpGlobals->time;
			m_apPoints[i]->bWinConditionsSatisfied = false;
			m_apPoints[i]->iTeam = EITHER;

			return i;
		}
	}
	
	// -use up the slot with the oldest point
	for(i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = m_apPoints[i];
		if(point && point->flTimeAdded == flOldestPoint)
		{
			point->pPoint->SUB_Remove();

			delete m_apPoints[i];
			m_apPoints[i] = new point_s;
			m_apPoints[i]->pPoint = pPoint;
			m_apPoints[i]->pPoint = pPoint;
			m_apPoints[i]->flTimeAdded = gpGlobals->time;
			m_apPoints[i]->bWinConditionsSatisfied = false;
			m_apPoints[i]->iTeam = EITHER;

			return i;
		}
	}
	return -1;
}

// updates the point's state and determines whether we have reached win conditions yet
void CPointManager::UpdatePointState( int iId )
{
	if(iId < 0 || iId > MAX_POINTS)
		return;

	point_s *pPoint = ReturnPointByID(iId);
	if(pPoint == NULL)
		return;

	if(!pPoint->pPoint)
		return;

	// -update whether this point has satisfied its win conditions
	POINT_STATE iState = pPoint->pPoint->GetPointStatus();
	if(iState == HELD || iState == DESTROYED || iState == ALL_REQUIRED_ESCAPED)
	{
		pPoint->bWinConditionsSatisfied = true;
	}
	else
	{
		pPoint->bWinConditionsSatisfied = false;
	}

	// -make sure the target team is set
	TARGET_TEAM iTeam = pPoint->pPoint->GetTargetTeam();
	pPoint->iTeam = iTeam;

	// check whether all win conditions are satisfied
	int iBritishOnlyTargets = 0;
	int iAmericanOnlyTargets = 0;
	int iTargetsForBoth = 0;

	int iBritishSuccededTargets = 0;
	int iAmericanSuccededTargets = 0;

	// --check for a win
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = ReturnPointByID(i);

		if(!point)
			continue;

		if(!point->pPoint)
			continue;

		if(pPoint->pPoint->pev->spawnflags & SF_NOT_NEEDED_FOR_WIN)
			continue;
		
		// -count the targets for each team and which both can capture
		if(point->iTeam == EITHER)
		{
			iTargetsForBoth++;
		}
		else if(point->iTeam == BRITISH)
		{
			iBritishOnlyTargets++;
		}
		else if(point->iTeam == AMERICANS)
		{
			iAmericanOnlyTargets++;
		}

		// -count how many targets each team holds
		if(point->bWinConditionsSatisfied == true)
		{
			TARGET_TEAM iTeamSucceded = point->pPoint->GetEngagingTeam();

			if(iTeamSucceded == BRITISH)
				iBritishSuccededTargets++;

			else if(iTeamSucceded == AMERICANS)
				iAmericanSuccededTargets++;
		}
	}

	// -check if one team has all their targets and all the targets available to all
	if(iBritishSuccededTargets == (iBritishOnlyTargets + iTargetsForBoth) && iBritishSuccededTargets > 0)
	{
		SetHasAnyTeamWon( true );
		SetTeamWon( BRITISH );
	}
	else if(iAmericanSuccededTargets == (iAmericanOnlyTargets + iTargetsForBoth) && iAmericanSuccededTargets > 0)
	{
		SetHasAnyTeamWon( true );
		SetTeamWon( AMERICANS );
	}
	// -no win yet
	else
	{
		SetHasAnyTeamWon( false );
		SetTeamWon( EITHER );
	}
}

// returns the players by the index given us
point_s *CPointManager::ReturnPointByID( int iPoint )
{
	// -check the number is valid
	if(iPoint < 0 || iPoint > MAX_POINTS)
	{
		return NULL;
	}

	point_s *point = m_apPoints[iPoint];

	if(!point)
		return NULL;

	return point;
}

// update or clear the player(s) hud
void CPointManager::UpdatePlayersHUD(CBasePlayer *pPlayer, bool bClearHud)
{
	extern int gmsgPoints;

	// -send the clear hud signal
	if(bClearHud)
	{
		if(pPlayer == NULL)
		{
			MESSAGE_BEGIN(MSG_ALL, gmsgPoints);
		}
		else
		{
			MESSAGE_BEGIN(MSG_ONE, gmsgPoints, NULL, pPlayer->pev);
		}
		WRITE_SHORT(-1);

		MESSAGE_END();
		return;
	}

	// -send every point to the client
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = ReturnPointByID(i);
		if(point && point->pPoint)
			point->pPoint->UpdateClientHUD(pPlayer);
	}
}

// -reset all points to default values
void CPointManager::ResetAllPoints( void )
{
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = ReturnPointByID(i);
		if(point && point->pPoint)
			point->pPoint->ResetPoint();
	}
}

// lets perform some maintainance
void CPointManager::DestroyAllPoints( void )
{
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = ReturnPointByID(i);
		if(point && point->pPoint) {
			point->pPoint->ClearMemory();
			point->pPoint = NULL;
			m_apPoints[i] = NULL;
		}
	}
}

// returns if we are ready for a win
bool CPointManager::EnoughPointsReadyForWin( void )
{
	return m_bHasAnyTeamWon;
}

// sets whether any team has won
void CPointManager::SetHasAnyTeamWon( bool bHasAnyTeamWon )
{
	m_bHasAnyTeamWon = bHasAnyTeamWon;
}

// returns the winning team
int CPointManager::GetWinningTeam( void )
{
	return (int)m_iTeamWon;
}

// sets the team that has won
void CPointManager::SetTeamWon( TARGET_TEAM iTeam )
{
	m_iTeamWon = iTeam;
}

// updates all flags which rely on a certain point
void CPointManager::UpdateAllFlags( int iPointId )
{
	const char *szName = STRING(ReturnPointByID(iPointId)->pPoint->pev->targetname);

		// -reseter ent
	CPointModel *pFlag = NULL;
	pFlag = (CPointModel*)UTIL_FindEntityByClassname( pFlag, "env_flag" );

	// -this allows us to reset the ents the mapper wants
	while (pFlag)
	{ 
		if(pFlag)
		{
			if(FStrEq(STRING(pFlag->pev->target),szName))
			{
				pFlag->SetModel((int)(ReturnPointByID(iPointId)->pPoint->GetEngagingTeam()));
			}
		}
		pFlag = (CPointModel*)UTIL_FindEntityByClassname( pFlag, "env_flag" );
	}
}

// sends that a player has died to all points
void CPointManager::PlayerDied( CBasePlayer *pPlayer )
{
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = ReturnPointByID(i);
		if(point && point->pPoint)
		{
			if(point->pPoint->pev->spawnflags & SF_STAY_CAPTURED_ON_DEAD && point->pPoint->GetPointStatus() == HELD) {
				if(pPlayer->pev->team == pPlayer->m_iTeamRespawningAs)
					continue;
			}

			point->pPoint->PlayerDied(pPlayer);
		}
	}
}

// handles a player disconnecting from the server
void CPointManager::PlayerDisconnected( CBasePlayer *pPlayer )
{
	for(int i = 0; i <= MAX_POINTS; i++)
	{
		point_s *point = ReturnPointByID(i);
		if(point && point->pPoint)
			point->pPoint->PlayerDied(pPlayer);
	}
}
