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

//This file was created by Ben Banfield on the 05/09/03 at 16:43 for the Battle Grounds Modification of Half-Life
//Its purpose is to allow for progressive spawning

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

// a spawn point entity
class CSpawnPoint : public CBaseEntity
{
public:
	virtual void Spawn( void );
	virtual void KeyValue( KeyValueData *pkvd );
	virtual char* GetContainer() { return szContainer; }

private:
	char szContainer[256];
};

LINK_ENTITY_TO_CLASS( info_player_british, CSpawnPoint);
LINK_ENTITY_TO_CLASS( info_player_american, CSpawnPoint);
LINK_ENTITY_TO_CLASS( info_player_spectator, CSpawnPoint);

// hide our spawn points
void CSpawnPoint::Spawn( void )
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL (ENT(pev), STRING(pev->model));
	pev->effects |= EF_NODRAW;
	
	UTIL_SetOrigin( pev, pev->origin ); 		
	UTIL_SetSize( pev, pev->absmin, pev->absmax );
	pev->iuser1 = 1;
}

// grab the name of our container
void CSpawnPoint::KeyValue( KeyValueData *pKeyValueData )
{
	if(FStrEq(pKeyValueData->szKeyName, "container")) {
		sprintf(szContainer, "%s", pKeyValueData->szValue);
	}
	else
		CBaseEntity::KeyValue(pKeyValueData);
}

const int SF_START_ON = 1;
const int SF_DUMMY = 2;
void CSpawnContainer::Spawn( void )
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL (ENT(pev), STRING(pev->model));
	pev->effects |= EF_NODRAW;
	
	UTIL_SetOrigin( pev, pev->origin ); 		
	UTIL_SetSize( pev, pev->absmin, pev->absmax );
	if(pev->spawnflags & SF_START_ON) {
		bTriggeredOn = true;
		bActive = true;
	} else {
		bTriggeredOn = false;
		bActive = false;
	}

	// save off the type of spawn point we'll be dealing with	
	if(pev->team == BRITS)
		sprintf(szSpawnPointClassName, "info_player_british");
	else if(pev->team == AMERS)
		sprintf(szSpawnPointClassName, "info_player_american");
	else {
		ALERT(at_console, "OMFG SET THE BLUMIN TEAM FOR THIS SPAWN CONTAINER\n");
		return;
	}
}

// -stores the target entities
void CSpawnContainer::KeyValue( KeyValueData *pKeyValueData )
{
	if(bInitValues == false) {
		szParentContainer[0] = '\0';
		bInitValues = true;
	}
	if( FStrEq(pKeyValueData->szKeyName, "parent_container")) {
		sprintf(szParentContainer, "%s", pKeyValueData->szValue);
	}
}

// turns ours on an all others off
void CSpawnContainer::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( !CanFireForActivator( pActivator ) )
		return;

	// flip our triggered state
	bTriggeredOn = !bTriggeredOn;

	// check all our parents are on
	bool bTreeOn = CheckParentContainersOn();

	// finally set whether this point is actually on
	if(bTreeOn && bTriggeredOn) {
		bActive = true;
	} else {
		bActive = false;
	}
	
	if(!(pev->spawnflags & SF_DUMMY)) {
		// actually do the setting of the spawns
		SetOurSpawnPoints(bActive);
	}

	// our parents need to be set to the inverse of us
	UpdateParentContainers(!bActive);

	// lets see if any of our children are on and awaiting turning on
	UpdateChildContainers(bActive);
}

// determine whether we have the full family tree on
bool CSpawnContainer::CheckParentContainersOn( void ) {

	// search for our parent
	if(szParentContainer[0] != '\0') {
		CSpawnContainer *pParent = NULL;
		pParent = (CSpawnContainer*)UTIL_FindEntityByTargetname( pParent, szParentContainer );

		if(pParent) {
			// if we've been triggered on, check to see if our parents have been also
			if(bTriggeredOn == true) {
				return pParent->CheckParentContainersOn();
			}
			// we haven't been triggered so the family tree isn't complete
			else
				return false;
		// couldn't find our parent so scupper the family tree
		} else {
			ALERT(at_console, "No parent container\n");
			return false;
		}
	// no parent so return our triggered state
	} else {
		return bTriggeredOn;
	}
}

// updates the state of this spawn containers children
// if we're turning one of our children on, then our spawns need to be turned off
void CSpawnContainer::UpdateChildContainers( bool bOn ) {
	CSpawnContainer *pChild = NULL;
	pChild = (CSpawnContainer*)UTIL_FindEntityByClassname( pChild, "info_spawn_container" );

	// loop through all spawn containers
	while(pChild) {
		// check its the right team
		if(pChild->pev->team == pev->team) {
			// determine whether this point is our child
			if(FStrEq(pChild->szParentContainer, STRING(pev->targetname))) {
				// only set the point on if its been triggered
				if(pChild->bTriggeredOn && bActive == true) {
					pChild->bActive = true;
					pChild->SetOurSpawnPoints(true);
					// see if any of its children are active
					pChild->UpdateChildContainers(true);
					SetOurSpawnPoints( false );
				} else {
					// if our child isn't active or we're not, make sure all their children are off
					pChild->bActive = false;
					pChild->UpdateChildContainers(false);
				}
			}
		}
		pChild = (CSpawnContainer*)UTIL_FindEntityByClassname( pChild, "info_spawn_container" );
	}
}

// sets our parent containers spawns, on/off depending on whether we're off/on
void CSpawnContainer::UpdateParentContainers( bool bOn ) {
	// search for our parent
	if(szParentContainer[0] != '\0') {
		CSpawnContainer *pParent = NULL;
		pParent = (CSpawnContainer*)UTIL_FindEntityByTargetname( pParent, szParentContainer );
		if(pParent) {
			if(!(pev->spawnflags & SF_DUMMY)) {
				if(bOn) {
					bool bTreeOn = pParent->CheckParentContainersOn();

					if(bTreeOn) {
						pParent->bActive = true;
						pParent->SetOurSpawnPoints( true );
					} else {
						pParent->UpdateParentContainers( true );
					}
				} else {
					pParent->bActive = false;
					pParent->SetOurSpawnPoints( false );
				}
			} else {
				pParent->UpdateParentContainers( bOn );
			}
		}
	}
}

// sets the state of the spawn points we control
void CSpawnContainer::SetOurSpawnPoints( bool bOn ) {
	int iOurPoints = 0;

	// we want our points on, so make it so
	if( bOn ) 
		iOurPoints = 1;

	// lets find all spawn points for this team
	CSpawnPoint *pSpawnPoint = NULL;
	pSpawnPoint = (CSpawnPoint*)UTIL_FindEntityByClassname( pSpawnPoint, szSpawnPointClassName );

	// loop through the spawns setting on/off accordingly
	while (pSpawnPoint)
	{
		// if we own this spawn point, then set it to the desired state
		if(!strcmp(pSpawnPoint->GetContainer(), STRING(pev->targetname))) {
			pSpawnPoint->pev->iuser1 = iOurPoints;
		}

		pSpawnPoint = (CSpawnPoint*)UTIL_FindEntityByClassname( pSpawnPoint, szSpawnPointClassName );
	}
}

LINK_ENTITY_TO_CLASS( info_spawn_container, CSpawnContainer );