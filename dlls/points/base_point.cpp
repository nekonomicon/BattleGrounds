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

//This file was created on the 17/5/03 at 16:32 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is define the base point class from which the capture, escape and destruction points will inherit off

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "animation.h"
#include "bg_rules.h"

#include "point.h"

#include "../../cl_dll/message_defs.h"

extern CPointManager *pPointManager;

void CBasePoint::Spawn( void )
{
	Precache( );

	// - make and invisble box which will get triggered on entry
	pev->solid = SOLID_TRIGGER;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL (ENT(pev), STRING(pev->model));
	pev->effects |= EF_NODRAW;
	pev->flags |= FL_ALWAYSTHINK;
	
	UTIL_SetOrigin( pev, pev->origin ); 		
	UTIL_SetSize( pev, pev->absmin, pev->absmax );

	SetID(pPointManager->AddPoint( this ));
}

// saves settings which were defined by the mapper
void CBasePoint::KeyValue( KeyValueData *pKeyValueData )
{
	// -saves the target team
	if(FStrEq(pKeyValueData->szKeyName, "team"))
	{
		SetTargetTeam((TARGET_TEAM)atoi(pKeyValueData->szValue));
	}
	// -1.x team
	else if(FStrEq(pKeyValueData->szKeyName, "cap_team"))
	{
		SetTargetTeam((TARGET_TEAM)atoi(pKeyValueData->szValue));
	}
	// - sends anything else off to get handled by CBaseEntity
	else
	{
		CBaseEntity::KeyValue( pKeyValueData );
	}
}

// saves which type of point this point is
void CBasePoint::SetPointType( POINT_TYPE iPointType )
{
	// - check we have been given a valid point type
	if(iPointType <= NO_TYPE || iPointType > ESCAPE_POINT)
	{
		ALERT(at_console, "Point being spawned with bad point type - (%i)\n", (int)iPointType);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point Spawned\" (point_type \"%i\") (previous_type \"%i\")\n", (int)iPointType, (int)m_iPointType);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return;
	}
	m_iPointType = iPointType;
}

// saves which type of point this point is
POINT_TYPE CBasePoint::GetPointType( void )
{
	// - check we have been given a valid point type
	if(m_iPointType <= NO_TYPE || m_iPointType > ESCAPE_POINT)
	{
		ALERT(at_console, "Point returning bad point type - (%i)\n", (int)m_iPointType);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point with corrupted point_type\" (point_id \"%i\") (point_type \"%i\")\n", GetPointID(), (int)m_iPointType);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return NO_TYPE;
	}
	return m_iPointType;
}

// saves the point's id that is used in the point manager
void CBasePoint::SetID( int iID )
{
	// - check we are setting this point with a valid id
	if(iID < 0 || iID > MAX_POINTS)
	{
		ALERT(at_console, "Point being spawned with bad ID - (%i)\n", iID);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point Spawned\" (point_id \"%i\") (previous_id \"%i\") (point_type \"%i\") (max_points \"%i\")\n", iID, m_iID, (int)GetPointType(), MAX_POINTS);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return;
	}
	m_iID = iID;
}

// returns the point's id as used in the point manager
int CBasePoint::GetPointID( void )
{
	// - check this point still has a valid id
	if(m_iID < 0 || m_iID >= MAX_POINTS)
	{
		ALERT(at_console, "Point returning bad id - (%i)\n", m_iID);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point with corrupted ID\" (point_id \"%i\") (name \"%s\")\n", m_iID, STRING(pev->targetname));
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return 0;
	}
	return m_iID;
}

// sets the point's status
void CBasePoint::SetPointStatus( POINT_STATE iPointStatus )
{
	// - ben notes: All point types end in either 0, 1 or 2 regardless of state
	int iPointStatusType = iPointStatus % 10;

	// - ben notes: These checks will make sure the point state is 0, 1, 2, 10, 11, 12, 20, 21 or 22
	if((iPointStatusType < 0 || iPointStatusType > 2)
		|| ((iPointStatus / 10) > 2 || (iPointStatus / 10) < 0)
		&& iPointStatus != HUD_RESET)
	{
		ALERT(at_console, "Bad Point status being set - (%i)\n", (int)iPointStatus);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point Status being set\" (point_id \"%i\") (point_type \"%i\") (point_status \"%i\") (previous_point_status \"%i\")\n", GetPointID(), (int)GetPointType(), (int)iPointStatus, (int)GetPointStatus());
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return;		
	}
	m_iPointStatus = iPointStatus;

	if(pPointManager != NULL)
		pPointManager->UpdatePointState( this->GetPointID() );
}

// returns the point's current status
POINT_STATE CBasePoint::GetPointStatus( void )
{
	// - ben notes: All point types end in either 0, 1 or 2 regardless of state
	int iPointStatusType = m_iPointStatus % 10;

	// - ben notes: These checks will make sure the point state is 0, 1, 2, 10, 11, 12, 20, 21 or 22
	if((iPointStatusType < 0 || iPointStatusType > 2)
		|| ((m_iPointStatus / 10) > 2 || (m_iPointStatus / 10) < 0)
		&& m_iPointStatus != HUD_RESET)	
	{
		ALERT(at_console, "Point returning bad status - (%i)\n", (int)m_iPointStatus);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point with corrupted status\" (point_id \"%i\") (point_type \"%i\") (point_status \"%i\")\n", GetPointID(), (int)GetPointType(), (int)m_iPointStatus);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return NO_STATE;		
	}
	return m_iPointStatus;
}

// saves the target team - the only team which can use this point
void CBasePoint::SetTargetTeam( TARGET_TEAM iTargetTeam )
{
	if(iTargetTeam < EITHER || iTargetTeam > AMERICANS)
	{
		ALERT(at_console, "Bad target team being set - (%i)\n", (int)iTargetTeam);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Point being set with bad target team\" (point_id \"%i\") (point_type \"%i\") (point_status \"%i\") (target_team \"%i\") (previous_target_team \"%i\")\n", GetPointID(), (int)GetPointType(), (int)GetPointStatus(), iTargetTeam, m_iTargetTeam);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return;
	}
	m_iTargetTeam = iTargetTeam;
}

// returns the target team
TARGET_TEAM CBasePoint::GetTargetTeam( void )
{
	if(m_iTargetTeam < EITHER || m_iTargetTeam > AMERICANS)
	{
		ALERT(at_console, "Point returning bad target team - (%i)\n", (int)m_iTargetTeam);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point returning bad target team\" (point_id \"%i\") (point_type \"%i\") (point_status \"%i\") (target_team \"%i\")\n", GetPointID(), (int)GetPointType(), (int)GetPointStatus(), m_iTargetTeam);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return EITHER;
	}
	return m_iTargetTeam;
}

// sets the team which is currently engaged in the point
void CBasePoint::SetEngagingTeam( TARGET_TEAM iEngagingTeam )
{
	if(iEngagingTeam < EITHER || iEngagingTeam > AMERICANS)
	{
		ALERT(at_console, "Bad team engaging point - (%i)\n", (int)iEngagingTeam);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad team engaging point\" (point_id \"%i\") (point_type \"%i\") (point_status \"%i\") (engaging_team \"%i\") (previous_engaging_team \"%i\")\n", GetPointID(), (int)GetPointType(), (int)GetPointStatus(), iEngagingTeam, m_iEngagingTeam);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return;
	}
	m_iEngagingTeam = iEngagingTeam;
}

// returns the team currently engaging in the point
TARGET_TEAM CBasePoint::GetEngagingTeam( void )
{
	if(m_iEngagingTeam < EITHER || m_iEngagingTeam > AMERICANS)
	{
		ALERT(at_console, "Point returning bad team engaging - (%i)\n", (int)m_iEngagingTeam);
		UTIL_LogPrintf(POINT_LOGGING, "World triggered \"Bad Point returning bad engaging team\" (point_id \"%i\") (point_type \"%i\") (point_status \"%i\") (engaging_team \"%i\")\n", GetPointID(), (int)GetPointType(), (int)GetPointStatus(), m_iEngagingTeam);
	
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return EITHER;
	}
	return m_iEngagingTeam;
}

// is this player allowed to use this point
bool CBasePoint::CanPlayerUsePoint( CBasePlayer *pPlayer )
{
	// - check whether player is alive
	if(pPlayer->m_iDead == DEAD)
		return false;

	// - check whether player isn't a spectator
	if(pPlayer->pev->team == SPECS)
		return false;

	// - check whether player is in a team which is allowed to use this point
	int iTeam = (int)GetTargetTeam();

	if(iTeam == (int)EITHER)
		return true;

	if(iTeam == pPlayer->pev->team)
		return true;

	// - the point isn't open to both teams and the player isn't on the correct team, so they can't use this point
	return false;
}

// sends a text message to all clients detailing them of a change in the point
void CBasePoint::SendTextMessageToAllClients( int iPlayerId, int iPlayersInvolved, int iPointEvent )
{
	if(pev->spawnflags & SF_SUPRESS_MESSAGES)
		return;

	int iPointId = GetPointID();
	
	int iTeamId = (int)GetEngagingTeam();


	// message is created client side for internationalisation support
	extern int gmsgPredefinedMessage;

	MESSAGE_BEGIN(MSG_ALL, gmsgPredefinedMessage);

	WRITE_SHORT(iCapturePointMessages);
	WRITE_SHORT(iPointId);
	WRITE_SHORT(iPlayerId);
	WRITE_SHORT(iTeamId);
	WRITE_SHORT(iPointEvent);
	WRITE_SHORT(iPlayersInvolved);

	MESSAGE_END();
}

// requests an update on all flags which get their state from this point
void CBasePoint::UpdateAllFlags( void )
{
	if(pPointManager != NULL)
		pPointManager->UpdateAllFlags( GetPointID() );
}
