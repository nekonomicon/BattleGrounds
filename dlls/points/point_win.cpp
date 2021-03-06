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

// Created by SaintGreg (Greg Rogers)

// Used as a point entity, which when triggered will win the round.  
//  This is useful for maps which do not want to use the flag capturing method
//  of winning, or the deathmatch method with respawnstyle = 2.  Note:
//  you should be able to hybrid flag capping and objective-only map, 
//  where whichever gets triggered first - all flags, or one point_win 
//  will create the win conditions.

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "animation.h"

#include "point.h"
#include "point_win.h"



LINK_ENTITY_TO_CLASS(point_win, CPointWin);

void CPointWin::Spawn(void){
    isActive = false;
}

void CPointWin::ResetEnt(void){
    isActive = false;
}

TARGET_TEAM CPointWin::GetTargetTeam(void){
    return m_iTargetTeam;
}

void CPointWin::SetTargetTeam(TARGET_TEAM iTargetTeam){
	if(iTargetTeam <= EITHER || iTargetTeam > AMERICANS)
	{
		ALERT(at_console, "Bad target team being set - (%i)\n", (int)iTargetTeam);
		// -- remove this entity as it isn't valid
		UTIL_Remove(this);
		return;
	}
	m_iTargetTeam = iTargetTeam;
}

bool CPointWin::IsActivated(void){
    return isActive;
}

void CPointWin::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ){
    isActive = true;
}

void CPointWin::KeyValue( KeyValueData *pKeyValueData )
{
	if(FStrEq(pKeyValueData->szKeyName, "target_team"))
	{
		SetTargetTeam( (TARGET_TEAM) atoi(pKeyValueData->szValue) );
	}
    else
	{
		CBaseEntity::KeyValue( pKeyValueData );
	}

}
